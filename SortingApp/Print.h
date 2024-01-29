#pragma once

#include <sstream>

template<class InputIt>
std::string print(InputIt begin, InputIt end);

template<class InputIt>
inline std::string print(InputIt begin, InputIt end)
{
	if (std::distance(begin, end) <= 0)
	{
		return {};
	}

	std::ostringstream oss;

	auto it = begin;

	for (; it + 1 != end; ++it)
	{
		oss << *it << ", ";
	}

	oss << *it;

	return oss.str();
}
