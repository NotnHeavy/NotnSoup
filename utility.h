//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "macros.h"

// Returns a pointer, otherwise exits gracefully.
static void* safe_malloc(size_t size)
{
	// Using calloc() will set the memory and can be faster than malloc().
	void* ptr = calloc(1, size);
	RUNTIME_ERROR(ptr, "calloc() failed: %s\n", exit(EXIT_FAILURE), strerror(errno));
	return ptr;
}

#ifdef __cplusplus
}
#endif