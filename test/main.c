#include <stdio.h>
#include <nns_macros.h>
#include <nns_sharedlib.h>
#include <nns_process.h>

void pipeout_func(const char* buffer, STOCK void* data)
{
    printf("READ FROM PIPE: %s\n", buffer);
}

#if defined(_WIN32)
#   define MODULE "./testshared.dll"
#elif defined(__linux__)
#   define MODULE "./testshared.so"
#else
#   define MODULE
#endif

int main()
{
    // sharedlib test
    sharedlib test;
    NNS_LoadLibrary(&test, MODULE);
    void (*horrible)() = NNS_GetProcAddress(&test, "the");
    horrible();
    NNS_FreeLibrary(&test);

    // executing process test
    processdata data;
    data.pipeout = pipeout_func;
    NNS_StartProcess((char*[]){ "./testprocess", "--none", NULL }, 0, NULL);

    // exit
    return 0;
}