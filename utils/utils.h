#pragma once 

namespace matrix
{

namespace utils
{

const double fp_tolerance = 1e-8;

inline int cmp_double(double lhs, double rhs)
{
    if (std::isnan(lhs) && std::isnan(rhs))
    {
        return 0;
    }
    if (fabs(lhs - rhs) < fp_tolerance)
    {
        return 0;
    }
    else if ((lhs - rhs) > fp_tolerance)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

}; // namespace utils

}; // namespace matrix

