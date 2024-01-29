#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<InputIt> partition(InputIt begin, InputIt end);

template<class InputIt>
std::experimental::generator<bool> quickSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<InputIt> partition(InputIt begin, InputIt end)
{
	auto value = *std::prev(end);

	InputIt output = begin;

	for (InputIt it = begin; it != end; ++it)
	{
		if (*it < value)
		{
			std::swap(*it, *output);
			++output;
			co_yield{};
		}
	}

	std::swap(*output, *std::prev(end));

	co_yield output;
}

template<class InputIt>
inline std::experimental::generator<bool> quickSort(InputIt begin, InputIt end)
{
	if (std::distance(begin, end) <= 1)
	{
		co_return;
	}

	InputIt pivot{};

	for (auto e : partition(begin, end))
	{
		pivot = e;
		co_yield false;
	}

	for (auto e : quickSort(begin, pivot)) { co_yield false; }
	for (auto e : quickSort(pivot + 1, end)) { co_yield false; }
}
