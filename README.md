# notnsoup
This is a collection of CMake scripts, C and C++ headers that you can drag into your repository. I would highly recommend that you don't include this as a submodule in the case of function changes breaking compatibility (unless you detach the HEAD).

# Language versions supported
- CMake: CMake 3.8
- C: C99
- C++: C++14

# Current platforms supported
- Windows
- Linux
- macOS (hopefully)

# Directory tree
- `/cmake/` - CMake scripts
- `/include/` - headers that can be used in either C or C++ code
- `/include/c/` - headers designed for C code specifically (although you *can* use them in C++ code)
- `/include/cpp/` - C++-only headers