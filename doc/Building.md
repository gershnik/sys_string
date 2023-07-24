# Integrating and configuring SysString

## Integration

### CMake via FetchContent

With modern CMake the easiest way to use this library is

```cmake
include(FetchContent)
FetchContent_Declare(sys_string
    GIT_REPOSITORY git@github.com:gershnik/sys_string.git
    GIT_TAG        <desired tag like v2.10>
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(sys_string)
...
target_link_libraries(mytarget
PRIVATE
  sys_string::sys_string
)
```
> ℹ&#xFE0F; _[What is FetchContent?](https://cmake.org/cmake/help/latest/module/FetchContent.html)_

### CMake from download

Alternatively, you can clone this repository somewhere and do this:

```cmake
add_subdirectory(PATH_WHERE_YOU_DOWNALODED_IT_TO, sys_string)
...
target_link_libraries(mytarget
PRIVATE
  sys_string::sys_string
)
```

Note that you need to have your compiler to default to at least C++17 or set `CMAKE_CXX_STANDARD` to at least 17 in order for build to succeed.

### Building and installing on your system

You can also build and install this library on your system using CMake.

1. Download or clone this repository into SOME_PATH
2. On command line:
```bash
cd SOME_PATH
cmake -S . -B build 
cmake --build build

#Optional
#cmake --build build --target run-test

#install to /usr/local
sudo cmake --install build
#or for a different prefix
#cmake --install build --prefix /usr
```

Once the library has been installed it can be used int the following ways:

#### Basic use 

Set the include directory to `<prefix>/include` where `<prefix>` is the install prefix from above.
Add `<prefix>/lib/libsys_string.a` to your link libraries.

#### CMake package

```cmake
find_package(sys_string)

target_link_libraries(mytarget
PRIVATE
  sys_string::sys_string
)
```

#### Via `pkg-config`

Add the output of `pkg-config --cflags --libs sys_string` to your compiler flags.

Note that the default installation prefix `/usr/local` might not be in the list of places your
`pkg-config` looks into. If so you might need to do:
```bash
export PKG_CONFIG_PATH=/usr/local/share/pkgconfig
```
before running `pkg-config`


### Other build systems

If you use a different build system then:

* Set your include path to `lib/inc` 
* No special preprocessor flags are required except on Windows where `_CRT_SECURE_NO_WARNINGS` must be defined to avoid MSVC bogus warnings.
* Compile the sources under `lib/cpp` into a static library and add it to your link step
* You will need to link with the following libraries:
  * Mac: `CoreFoundation` framework
  * Windows: `runtimeobject.lib`
  * Emscripten: `embind`

Note that you need to have your compiler to use at least C++17 mode in order for build to succeed.

## Configuration options

Whichever method you use you can set the following macros (either on command line or _before_ including any library headers) to control the library behavior:

* `SYS_STRING_NO_S_MACRO` - set it to 1 to disable short `S()` macro. See [Usage](doc/Usage.md#basics) for details
* `SYS_STRING_WIN_BSTR` - set it to 1 to use `BSTR` as default `sys_string` storage on Windows. It has no effect on other platforms.
* `SYS_STRING_WIN_HSTRING` - set it to 1 to use `HSTRING` as default `sys_string` storage on Windows. It has no effect on other platforms.
* `SYS_STRING_USE_GENERIC` - set it to 1 to use `const char *` as default `sys_string` storage on MacOS (similar to Linux). It has no effect on other platforms.
* `SYS_STRING_ENABLE_PYTHON` - set it to 1 to enable Python support. This requires header `<Python.h>` to be available on the include path. If enabled the `sys_string_pystr` and related classes become available but the default `sys_string` will still be your platform default one.
* `SYS_STRING_USE_PYTHON` - set it to 1 to make Python strings the default `sys_string` storage. This automatically enables `SYS_STRING_ENABLE_PYTHON` and has the same requirements.
