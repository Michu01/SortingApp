#pragma once

#include <algorithm>
#include <experimental/generator>

#include "CountSort.h"

template<class InputIt>
std::experimental::generator<bool> MSD_RadixSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> MSD_RadixSort(InputIt begin, InputIt end)
{
    auto max = *std::max_element(begin, end);

    int exp = 1;

    while (exp < max) 
    {
        exp *= 10;
    }

    for (; exp > 0; exp /= 10)
    {
        for (auto e : countSort(begin, end, exp))
        {
            co_yield false;
        }
    }
}