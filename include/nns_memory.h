//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "nns_os.h"
#include "nns_macros.h"

//////////////////////////////////////////////////////////////////////////////
// GLOBALS                                                                  //
//////////////////////////////////////////////////////////////////////////////

#if defined(WIN32)
#	define PROT_READ	0x1
#	define PROT_WRITE	0x2
#	define PROT_EXEC	0x4 
#	define PROT_NONE	0x0
#elif defined(NNS_ISUNIX)
#	define NNS_PAGESIZE	4096
#	define NNS_ALIGN(x) (void*)((uintptr_t)(x) & ~(NNS_PAGESIZE - 1))
#	define NNS_MODALIGN(x) ((uintptr_t)(x) % NNS_PAGESIZE)
#endif

//////////////////////////////////////////////////////////////////////////////
// MEMORY FUNCTIONS                                                         //
//////////////////////////////////////////////////////////////////////////////

// Returns a pointer, otherwise exits gracefully.
STOCK static inline void* NNS_malloc(size_t size)
{
	// Using calloc() will set the memory and can be faster than malloc().
	void* ptr = calloc(1, size);
	RUNTIME_ERROR(ptr, "calloc() failed: %s\n", exit(EXIT_FAILURE), strerror(errno));
	return ptr;
}

// Set the memory protection of a memory block. Returns true on success, otherwise
// false.
STOCK static inline bool NNS_MemoryProtect(void* address, size_t length, int access)
{
#if defined(WIN32)
	DWORD buffer, protection;
	switch (access)
	{
	case PROT_NONE:
		protection = PAGE_NOACCESS;
		break;
	case PROT_READ:
		protection = PAGE_READONLY;
		break;
	case PROT_EXEC:
		protection = PAGE_EXECUTE;
		break;
	case PROT_READ | PROT_WRITE:
		protection = PAGE_READWRITE;
		break;
	case PROT_READ | PROT_EXEC:
		protection = PAGE_EXECUTE;
		break;
	case PROT_READ | PROT_WRITE | PROT_EXEC:
		protection = PAGE_EXECUTE_READWRITE;
		break;
	default:
		return false;
	}
	return (VirtualProtect(address, length, protection, &buffer) ? true : false);
#elif defined(NNS_ISUNIX)
	return ((mprotect(NNS_ALIGN(address), length + NNS_MODALIGN(address), access) == 0) ? true : false);
#endif

	// Unsupported operating system?
	return false;
}