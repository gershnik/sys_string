# Building and configuration

## Building

### CMake

With modern CMake the easiest way to use this library is

```cmake
include(FetchContent)
FetchContent_Declare(sys_string
    GIT_REPOSITORY git@github.com:gershnik/sys_string.git
    GIT_TAG        <desired tag like v2.10>
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sys_string)
```

Alternatively, if you prefer to store the sources locally clone this repository and 
add its `lib` sub-directory as subdirectory of your CMake project. Something like

```cmake
add_subdirectory(PATH_TO_SYS_STRING_REPO/lib, sys_string)
```

In either case you can now use this library by doing something like

```cmake
target_link_library(your_target PRIVATE sys_string)
```

Note that you need to have your compiler to default to at least C++17 or set `CMAKE_CXX_STANDARD` to at least 17 in order for build to succeed.

### Other build systems

If you use a different build system then:

* Set your include path to `lib/inc` 
* No special preprocessor flags are required except on Windows where `_CRT_SECURE_NO_WARNINGS` must be defined to avoid MSVC bogus warnings.
* Compile the sources under `lib/cpp` into a static library and add it to your link step
* You will need to link with the following libraries:
  * Mac: `CoreFoundation` framework
  * Windows: `runtimeobject.lib`
  * Emscripten: `embind`


## Configuration options

Whichever build system you use you can set the following macros (either on command line or _before_ including any library headers) to control the library behavior:

* `SYS_STRING_NO_S_MACRO` - set it to 1 to disable short `S()` macro. See [Usage](doc/Usage.md#basics) for details
* `SYS_STRING_WIN_BSTR` - set it to 1 to use `BSTR` as default `sys_string` storage on Windows. It has no effect on other platforms.
* `SYS_STRING_WIN_HSTRING` - set it to 1 to use `HSTRING` as default `sys_string` storage on Windows. It has no effect on other platforms.
* `SYS_STRING_USE_GENERIC` - set it to 1 to use `const char *` as default `sys_string` storage on MacOS (similar to Linux). It has no effect on other platforms.
* `SYS_STRING_ENABLE_PYTHON` - set it to 1 to enable Python support. This requires header `<Python.h>` to be available on the include path. If enabled the `sys_string_pystr` and related classes become available but the default `sys_string` will still be your platform default one.
* `SYS_STRING_USE_PYTHON` - set it to 1 to make Python strings the default `sys_string` storage. This automatically enables `SYS_STRING_ENABLE_PYTHON` and has the same requirements.
