gcc main.c -o main -I../include/ -Wall -Wextra
gcc testshared.c -o testshared.so -shared -Wall -Wextra
gcc testprocess.c -o testprocess -Wall -Wextra