#pragma once

#include "driver.hpp"

namespace test_utils
{

namespace detail
{

template <typename T> 
std::string get_result(std::string_view file_name)
{
	int res = 0;

    Driver drv;

	res = drv.parse(file_name);

	EXPEXT_EQ(res, 0);

    return result.str();
}

inline std::string get_answer(std::string_view file_name)
{
    std::ifstream answer_file;

    answer_file.open(std::string(file_name));

    if (!answer_file.is_open())
    {
        LOG("Can't open {}\n", file_name);
        throw;
    }

    std::string answer((std::istreambuf_iterator<char>(answer_file)),
                       std::istreambuf_iterator<char>());

    return answer;
}

}; // namespace detail

}; // namespace test_utils
