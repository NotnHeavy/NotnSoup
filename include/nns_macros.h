//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#define NOTNSOUP_MAJOR  1
#define NOTNSOUP_MINOR  3
#define NOTNSOUP_PATCH  2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef __cplusplus
#include <stdbool.h>
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

#ifdef __GNUC__
#	define STOCK __attribute__((unused))
#else
#	define STOCK
#endif

#if defined(__unix__) || defined(__APPLE__)
#	define NNS_ISUNIX
#endif

//////////////////////////////////////////////////////////////////////////////
// WARNINGS                                                                 //
//////////////////////////////////////////////////////////////////////////////

// Comment out the macro below to turn off warning suppression (via nns_warnings.h).
#define NNS_SUPPRESS_WARNINGS

#ifdef NNS_SUPPRESS_WARNINGS
#include "nns_warnings.h"
#endif