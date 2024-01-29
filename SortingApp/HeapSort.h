#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> heapify(InputIt begin, size_t size, size_t i);

template<class InputIt>
std::experimental::generator<bool> heapSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> heapify(InputIt begin, size_t size, size_t i)
{
    size_t largest = i;

    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    if (l < size && *(begin + l) > *(begin + largest))
    {
        largest = l;
    }

    if (r < size && *(begin + r) > *(begin + largest))
    {
        largest = r;
    }

    if (largest != i)
    {
        std::swap(*(begin + i), *(begin + largest));

        co_yield false;

        for (auto e : heapify(begin, size, largest)) 
        { 
            co_yield false; 
        }
    }
}

template<class InputIt>
inline std::experimental::generator<bool> heapSort(InputIt begin, InputIt end)
{
    auto size = std::distance(begin, end);

    for (int i = size / 2 - 1; i >= 0; --i)
    {
        for (auto e : heapify(begin, size, i)) 
        {
            co_yield false; 
        }
    }

    for (size_t i = size - 1; i > 0; --i)
    {
        std::swap(*begin, *(begin + i));

        co_yield false;

        for (auto e : heapify(begin, i, 0)) 
        {
            co_yield false;
        }
    }
}
