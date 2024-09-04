#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "--none") == 0)
        printf("hello :)\n");
    printf("yr'oue Mother\n");
    return 0;
}