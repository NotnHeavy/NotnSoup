//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define NOTNSOUP_MAJOR  1
#define NOTNSOUP_MINOR  0
#define NOTNSOUP_PATCH  1

#ifdef __cplusplus
#include <iostream>
#else
#include <stdio.h>
#endif

// TODO: macos, linux code
#if defined(_MSC_VER)
#include <Windows.h>
#elif

#endif

// Standardising variadic argument macros is impossible, apparently.
#if defined(_MSC_VER)
#	define VA_ARGS(...) , __VA_ARGS__
#else
#	define VA_ARGS(...) , ##__VA_ARGS__
#endif

#define RUNTIME_ERROR(condition, message, ret, ...)		\
	if (!(condition))									\
	{													\
		fprintf(stderr, message VA_ARGS(__VA_ARGS__));	\
		ret;											\
	}	

#ifdef _MSC_VER
#pragma warning(disable : 6255)
#endif
