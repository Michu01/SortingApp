#pragma once

#include <algorithm>
#include <experimental/generator>

#include "QuickSort.h"
#include "InsertionSort.h"

template<class InputIt>
std::experimental::generator<bool> hybridSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> hybridSort(InputIt begin, InputIt end)
{
	auto distance = std::distance(begin, end);

	if (distance <= 1)
	{
		co_return;
	}

	if (distance < 16)
	{
		for (auto e : insertionSort(begin, end))
		{
			co_yield false;
		}

		co_return;
	}

	InputIt pivot{};

	for (auto e : partition(begin, end))
	{
		pivot = e;
		co_yield false;
	}

	for (auto e : hybridSort(begin, pivot)) { co_yield false; }
	for (auto e : hybridSort(pivot + 1, end)) { co_yield false; }
}
