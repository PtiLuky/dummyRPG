#pragma once

#include <algorithm>
#include <vector>

template <typename T> static bool vec_contains(std::vector<T> v, T e)
{
    return (std::find(v.begin(), v.end(), e) != v.end());
}
