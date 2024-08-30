# notnsoup
This is a collection of CMake scripts, C and C++ headers that you can drag into your repository. I would highly recommend that you don't include this as a submodule in the case of function changes breaking compatibility (unless you detach the HEAD).

# Directory tree
- `/cmake/` - CMake scripts
- `/include/` - headers that can be used in either C or C++ code
- `/include/c/` - C-only headers
- `/include/cpp/` - C++-only headers