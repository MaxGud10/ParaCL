#pragma once

#include "lib.h"
#include "log.h"

namespace test_utils
{

template <typename T> void run_test(const std::string &test_name)
{
    std::string test_folder = "data";

    std::string test_path =
        std::string(TEST_DATA_DIR) + test_folder + test_name;

    std::string result = detail::get_result<T>(test_path + ".dat");
    std::string answer = detail::get_answer(test_path + ".ans");

    EXPECT_EQ(result, answer);
}

} // namespace test_utils

