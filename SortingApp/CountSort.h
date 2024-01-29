#pragma once

#include <array>
#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> countSort(InputIt begin, InputIt end, int exp);

template<class InputIt>
inline std::experimental::generator<bool> countSort(InputIt begin, InputIt end, int exp)
{
    std::vector<typename InputIt::value_type> output(std::distance(begin, end));
    std::array<int, 10> count{ 0 };

    for (InputIt it = begin; it != end; ++it)
    {
        count[(*it / exp) % 10]++;
    }

    for (size_t i = 1; i < 10; i++)
    {
        count[i] += count[i - 1];
    }

    for (int i = std::distance(begin, end) - 1; i >= 0; --i)
    {
        auto value = *(begin + i);
        output[count[(value / exp) % 10] - 1] = value;
        count[(value / exp) % 10]--;
    }

    size_t n = 0;

    for (InputIt it = begin; it != end; ++it, ++n)
    {
        *it = output[n];

        co_yield false;
    }
}