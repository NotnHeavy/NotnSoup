//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "nns_macros.h"

static inline std::string ltrim(std::string str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c) 
	{
		return !std::isspace(c);
	}));
	return str;
}

static inline std::string rtrim(std::string str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char c)
	{
		return !std::isspace(c);
	}).base(), str.end());
	return str;
}

static inline std::string trim(std::string str)
{
	return ltrim(rtrim(str));
}

static std::vector<std::string> splitstrings(std::string str, const char* const delimiter)
{
	std::vector<std::string> tokens;
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		tokens.push_back(str.substr(0, pos));
		str.erase(0, pos + strlen(delimiter));
	}
	tokens.push_back(str);
	return tokens;
}

static std::string joinstrings(const std::vector<std::string>& strs, const char* const delimiter)
{
	std::ostringstream os;
	auto begin = std::begin(strs), end = std::end(strs);
	if (begin != end)
	{
		std::copy(begin, std::prev(end), std::ostream_iterator<std::string>(os, delimiter));
		begin = std::prev(end);
	}
	if (begin != end)
		os << *begin;
	return os.str();
}