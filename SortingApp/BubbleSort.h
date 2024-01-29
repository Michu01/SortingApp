#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> bubbleSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> bubbleSort(InputIt begin, InputIt end)
{
	auto count = std::distance(begin, end);

	for (size_t m = 0; m != count; ++m)
	{
		InputIt it = begin;

		for (size_t n = 0; n + m + 1 != count; ++n, ++it)
		{
			if (*it > *std::next(it))
			{
				std::swap(*it, *std::next(it));

				co_yield false;
			}
		}
	}
}
