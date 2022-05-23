# Building and configuration



## Building

If you use CMake clone this repository and add the `lib` directory as subdirectory. Something like

```cmake
add_subdirectory(PATH_TO_SYS_STRING_REPO/lib, sys_string)
```

Alternatively with modern CMake you can just do

```cmake
include(FetchContent)
FetchContent_Declare(sys_string
    GIT_REPOSITORY git@github.com:gershnik/sys_string.git
    GIT_TAG        <desired tag like v1.2>
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  lib
)
FetchContent_MakeAvailable(sys_string)
```

You need to have your compiler to default to at least C++17 or set `CMAKE_CXX_STANDARD` to at least 17 in order for build to succeed.

If you use a different build system all you need is to set your include path to `lib/inc` and compile the sources under `lib/cpp`. 

No special compilation flags are required except on Windows where `_CRT_SECURE_NO_WARNINGS` must be defined to avoid MSVC bogus warnings.
On Mac you need to link with `CoreFoundation` framework and on Windows with `runtimeobject.lib`.

### Configuration options

* `SYS_STRING_NO_S_MACRO` - set to 1 to disable short `S()` macro. See [Usage](doc/Usage.md#basics) for details
* `SYS_STRING_WIN_BSTR` - set to 1 to use `BSTR` as native `sys_string` type on Windows
* `SYS_STRING_WIN_HSTRING` - set to 1 to use `HSTRING` as native `sys_string` type on Windows
* `SYS_STRING_USE_GENERIC` - set to 1 to use `const char *` as native `sys_string` type on MacOS (similar to Linux)