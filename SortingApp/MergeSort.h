#pragma once

#include <algorithm>
#include <experimental/generator>

template<class InputIt>
std::experimental::generator<bool> merge(InputIt begin, InputIt mid, InputIt end);

template<class InputIt>
std::experimental::generator<bool> mergeSort(InputIt begin, InputIt end);

template<class InputIt>
inline std::experimental::generator<bool> merge(InputIt begin, InputIt mid, InputIt end)
{
	std::vector<typename InputIt::value_type> left(begin, mid);
	std::vector<typename InputIt::value_type> right(mid, end);

	auto leftIt = left.cbegin();
	auto rightIt = right.cbegin();
	auto it = begin;

	while (leftIt != left.cend() && rightIt != right.cend())
	{
		if (*leftIt <= *rightIt)
		{
			*it = *leftIt;
			++leftIt;
		}
		else
		{
			*it = *rightIt;
			++rightIt;
		}

		++it;

		co_yield false;
	}

	while (leftIt != left.cend())
	{
		*it = *leftIt;
		++leftIt;
		++it;

		co_yield false;
	}

	while (rightIt != right.cend())
	{
		*it = *rightIt;
		++rightIt;
		++it;

		co_yield false;
	}
}

template<class InputIt>
inline std::experimental::generator<bool> mergeSort(InputIt begin, InputIt end)
{
	if (begin + 1 >= end)
	{
		co_return;
	}
		
	InputIt mid = begin + (end - begin) / 2;

	for (auto e : mergeSort(begin, mid)) { co_yield false; }
	for (auto e : mergeSort(mid, end)) { co_yield false; }

	for (auto e : merge(begin, mid, end)) { co_yield false; };
}