#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> selectionSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> selectionSort(InputIt begin, InputIt end)
{
	for (InputIt it = begin; it != end; ++it)
	{
		auto min = std::min_element(it, end);

		std::swap(*it, *min);

		co_yield false;
	}
}
