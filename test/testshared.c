#include <stdio.h>

#if defined(_MSC_VER)
#   define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#   define EXPORT __attribute__((visibility("default")))
#else
#   define EXPORT
#endif

EXPORT void the()
{
    puts("plexus p/20");
}