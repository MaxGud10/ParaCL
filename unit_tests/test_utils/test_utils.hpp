#pragma once

#include "driver.hpp"
#include "log.h"

#include <fstream>
#include <sstream>

#include <gtest/gtest.h>

namespace test_utils
{

namespace detail
{

std::string get_result(std::string_view file_name)
{
    int status = 0;

    std::stringstream result;

    Driver drv(result);

    status = drv.parse(std::string(file_name));

    drv.ast.eval();

    EXPECT_EQ(status, 0);

    return result.str();
}

inline std::string get_answer(std::string_view file_name)
{
    std::ifstream answer_file{std::string(file_name)};

    if (!answer_file.is_open())
    {
        LOG("Can't open {}\n", file_name);
        throw;
    }

    std::string answer((std::istreambuf_iterator<char>(answer_file)), std::istreambuf_iterator<char>());

    answer.erase(std::remove(answer.begin(), answer.end(), '\n'), answer.end());

    return answer;
}

}; // namespace detail

inline void run_test(const std::string &test_name)
{
    std::string test_folder = "data";
    std::string test_path   = std::string(TEST_DATA_DIR) + test_folder + test_name;

    std::string result = detail::get_result(test_path + ".dat");
    std::string answer = detail::get_answer(test_path + ".ans");

    EXPECT_EQ(result, answer);
}

}; // namespace test_utils
