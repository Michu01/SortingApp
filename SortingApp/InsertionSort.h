#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> insertionSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> insertionSort(InputIt begin, InputIt end)
{
	for (InputIt it1 = begin; it1 != end; ++it1)
	{
		for (InputIt it2 = it1; it2 != begin && *std::prev(it2) > *it2; --it2)
		{
			std::swap(*std::prev(it2), *it2);

			co_yield false;
		}
	}
}


