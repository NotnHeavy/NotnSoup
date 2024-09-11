//////////////////////////////////////////////////////////////////////////////
// MADE BY NOTNHEAVY.                                                       //
//////////////////////////////////////////////////////////////////////////////

// General OS-specific code.

#pragma once

#include "nns_macros.h"

// TODO: macos, linux code
#if defined(_WIN32)
#   include <Windows.h>
#elif defined(NNS_ISUNIX)
#   include <unistd.h>
#   include <dlfcn.h>
#   include <spawn.h>
#   include <sys/wait.h>
#   include <sys/mman.h>

    extern char** environ;
#endif