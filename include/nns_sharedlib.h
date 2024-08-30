//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

// TODO: linux/macos library code

#pragma once

#include "nns_utility.h"
#include "nns_os.h"

//////////////////////////////////////////////////////////////////////////////
// GLOBALS                                                                  //
//////////////////////////////////////////////////////////////////////////////

// A basic object representing a loaded shared library.
typedef struct s_sharedlib
{
#ifdef _WIN32
    HMODULE module;
#endif
} sharedlib;

//////////////////////////////////////////////////////////////////////////////
// SHARED LIBRARY FUNCTIONS                                                 //
//////////////////////////////////////////////////////////////////////////////

// Load a new library into the current process, to be written to an existing
// lib object. Returns true on success, otherwise false.
static bool NNS_LoadLibrary(sharedlib* lib, const char* path)
{
    // Check if lib is NULL.
    if (!lib)
        return false;
    memset(lib, 0, sizeof(*lib));

    // Load the library.
#ifdef _WIN32
    UINT old = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
    lib->module = LoadLibraryA(path);
    SetErrorMode(old);
    return (lib->module != NULL);
#endif
}

// Read a function address from a loaded shared library. Returns a valid pointer
// on success, otherwise NULL.
static void* NNS_GetProcAddress(sharedlib* lib, const char* name)
{
    // Check if lib is NULL.
    if (!lib)
        return NULL;

    // Read a function from the library.
#ifdef _WIN32
    if (!lib->module)
        return NULL;
    return GetProcAddress(lib->module, name);
#endif
}

// Free a loaded library. Returns true on success, otherwise false.
static bool NNS_FreeLibrary(sharedlib* lib)
{
    // Check if lib is NULL.
    if (!lib)
        return false;

    // Free the library.
#ifdef _WIN32
    return FreeLibrary(lib->module);
#endif
}
