#pragma once

#include "driver.hpp"
#include "semantic_analyzer.hpp"
#include "log.h"
#include "tree_traverse.hpp"
#include "llvm_printer.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <array>

#include <gtest/gtest.h>

namespace test_utils
{

namespace detail
{

std::string get_result(std::string_view file_name);

std::string get_result(std::string_view file_name)
{
    int status = 0;

    std::stringstream result;

    Driver drv(result);


    status = drv.parse(std::string(file_name));
    EXPECT_EQ(status, 0);

    if (status != 0)
        throw std::runtime_error("Parser failed");

    SemanticAnalyzer semantic(drv.node_locations);
    semantic.analyze(drv.ast);

    TreeTraverse traverse(drv.ast.getCtx());
    drv.ast.accept(traverse);

    std::string result_str;
    std::getline(result, result_str, '\0');

    result_str.erase(std::remove(result_str.begin(), result_str.end(), '\n'), result_str.end());

    return result_str;
}

inline std::string get_answer(std::string_view file_name)
{
    std::ifstream answer_file{std::string(file_name)};

    if (!answer_file.is_open())
    {
        LOG("Can't open {}\n", file_name);
        throw std::runtime_error("Can't open answer file: " + std::string(file_name));
    }

    std::string answer((std::istreambuf_iterator<char>(answer_file)), std::istreambuf_iterator<char>());

    answer.erase(std::remove(answer.begin(), answer.end(), '\n'), answer.end());

    return answer;
}

}; // namespace detail


namespace codegen_detail
{

inline std::string read_all_from_pipe(FILE* pipe)
{
    if (!pipe)
        throw std::runtime_error("read_all_from_pipe(): pipe is null");

    std::array<char, 4096> buffer{};
    std::string result;

    while (true)
    {
        size_t n = std::fread(buffer.data(), 1, buffer.size(), pipe);
        if (n > 0)
            result.append(buffer.data(), n);

        if (n < buffer.size())
        {
            if (std::feof(pipe))
                break;

            if (std::ferror(pipe))
                throw std::runtime_error("read_all_from_pipe(): fread failed");
        }
    }

    return result;
}

inline std::string shell_escape(const std::string& s)
{
    std::string out = "'";
    for (char c : s)
    {
        if (c == '\'')
            out += "'\\''";
        else
            out += c;
    }
    out += "'";
    return out;
}

inline std::string get_codegen_result(std::string_view file_name)
{
    namespace fs = std::filesystem;

    fs::path temp_dir = fs::temp_directory_path() / "paracl_codegen_tests";
    fs::create_directories(temp_dir);

    std::string stem = fs::path(file_name).stem().string();

    fs::path ll_path  = temp_dir / (stem + ".ll");
    fs::path exe_path = temp_dir / (stem + ".out");

    Driver drv;
    int status = drv.parse(std::string(file_name));
    EXPECT_EQ(status, 0);

    if (status != 0)
        throw std::runtime_error("Parser failed for codegen test");

    SemanticAnalyzer semantic(drv.node_locations);
    semantic.analyze(drv.ast);

    llvm::LLVMContext llvm_ctx;
    LLVMPrinter printer(llvm_ctx);
    printer.generate(drv.ast);

    {
        std::error_code ec;
        llvm::raw_fd_ostream ir_out(ll_path.string(), ec);
        if (ec)
            throw std::runtime_error("Can't open .ll output file: " + ec.message());

        printer.dump(ir_out);
    }

    std::string compile_cmd =
        "clang++ "
        + shell_escape(ll_path.string()) + " "
        + shell_escape(std::string(PROJECT_SOURCE_DIR) + "/paralib/paraio.cpp") + " "
        + "-std=c++17 -O0 -o "
        + shell_escape(exe_path.string())
        + " 2>&1";

    FILE* compile_pipe = popen(compile_cmd.c_str(), "r");
    if (!compile_pipe)
        throw std::runtime_error("Failed to start clang++");

    std::string compile_output = read_all_from_pipe(compile_pipe);
    int compile_rc = pclose(compile_pipe);

    if (compile_rc != 0)
    {
        throw std::runtime_error(
            "Codegen compile failed.\nCommand:\n" + compile_cmd +
            "\nCompiler output:\n" + compile_output
        );
    }

    std::string run_cmd = shell_escape(exe_path.string()) + " 2>&1";

    FILE* run_pipe = popen(run_cmd.c_str(), "r");
    if (!run_pipe)
        throw std::runtime_error("Failed to run generated executable");

    std::string run_output = read_all_from_pipe(run_pipe);
    int run_rc = pclose(run_pipe);

    if (run_rc != 0)
    {
        throw std::runtime_error(
            "Generated program exited with non-zero code.\nOutput:\n" + run_output
        );
    }

    run_output.erase(std::remove(run_output.begin(), run_output.end(), '\r'), run_output.end());
    run_output.erase(std::remove(run_output.begin(), run_output.end(), '\n'), run_output.end());

    return run_output;
}

} // namespace codegen_detail

inline void run_codegen_test(const std::string& test_name)
{
    std::string test_folder = "data";
    std::string test_path   = std::string(TEST_DATA_DIR) + test_folder + test_name;

    try
    {
        std::string result = codegen_detail::get_codegen_result(test_path + ".dat");
        std::string answer = detail::get_answer(test_path + ".ans");

        EXPECT_EQ(result, answer);
    }
    catch (const std::exception& e)
    {
        FAIL() << "Exception in run_codegen_test(" << test_name << "): " << e.what();
    }
    catch (...)
    {
        FAIL() << "Unknown non-std exception in run_codegen_test(" << test_name << ")";
    }
}

inline void run_test(const std::string &test_name)
{
    std::string test_folder = "data";
    std::string test_path   = std::string(TEST_DATA_DIR) + test_folder + test_name;

    try
    {
        std::string result = detail::get_result(test_path + ".dat");
        std::string answer = detail::get_answer(test_path + ".ans");

        EXPECT_EQ(result, answer);
    }
    catch (const std::exception& e)
    {
        FAIL() << "Exception in run_test(" << test_name << "): " << e.what();
    }
    catch (...)
    {
        FAIL() << "Unknown non-std exception in run_test(" << test_name << ")";
    }
}


}; // namespace test_utils
