#pragma once

#include <limits>
#include <cmath>

namespace numeric {

using limits = std::numeric_limits<float>;

inline float max()
{
    return limits::max();
}

inline float lowest()
{
    return limits::lowest();
}

inline float epsilon()
{
    return limits::epsilon();
}

inline bool is_close(float a, float b)
{
    return std::fabs(a - b) < limits::epsilon();
}

inline bool is_less_or_close(float a, float b)
{
    return a < b || is_close(a, b);
}

inline bool is_great_or_close(float a, float b)
{
    return a > b || is_close(a, b);
}

inline bool is_less(float a, float b)
{
    return a < b && !is_close(a, b);
}

inline bool is_great(float a, float b)
{
    return a > b && !is_close(a, b);
}

}  // namespace numeric
