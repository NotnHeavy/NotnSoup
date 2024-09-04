@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

cl main.c /I../include/ /W4 /D_CRT_SECURE_NO_WARNINGS -link -out:main.exe
cl testshared.c /W4 /D_CRT_SECURE_NO_WARNINGS -link -dll -out:testshared.dll
cl testprocess.c /W4 /D_CRT_SECURE_NO_WARNINGS -link -out:testprocess.exe