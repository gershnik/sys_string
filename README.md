## Intro 

This library provides a C++ string class `sys_string` that is optimized for **interoperability with platform-native string type**. It is **immutable**, **Unicode-first** and exposes convenient **operations similar to Python or ECMAScript strings**. It uses a separate `sys_string_builder` class to construct strings. It provides fast concatenation via `+` operator that **does not allocate temporary strings**. 
The library exposes bidirectional UTF-8/UTF-16/UTF-32 views of `sys_string` as well as of any random access containers 
of characters.

## What does it mean?

* Interoperability with platform-native string type means that `sys_string` makes conversions to and from native string types (`NSString *` or `CFStringRef` on macOS/iOS, Java `String` on Android, `HSTRING` or `const wchar_t *` on Windows and `const char *` on Linux) as efficient as possible and ideally 0 cost operations. For example on Apple's platforms it stores `NSString *` internally allowing zero cost conversion. On Android where no-op conversions to Java strings are impossible for technical reasons, the internal storage is such that it makes conversions as cheap as possible.
* Immutable. String instances cannot be modified. To do modifications you use a separate "builder" class. This is similar to how many other languages do it and results in improved performance and elimination of whole class of errors. 
* Unicode-first. Instances of `sys_string` always store Unicode characters in either UTF-8, UTF-16 or UTF-32, depending on platform. Iteration can be done in all of these encodings and all operations (case conversion, case insensitive comparisons, trimming) are specified as actions on sequence of Unicode codepoints using Unicode algorithms. 
* Operations similar to Python or ECMAScript strings means that you can do things like `rtrim`, `split`, `join`, `starts_with` etc. in a way proven to be natural and productive in those languages.
* Concatenation does not allocate temporaries and copies addends once means that `result = s1 + s2 + s3` has one memory allocation and one copy of each of `s1`, `s2` and `s3` content into the result. Not 2 allocations and 5 copies like in other languages or with `std::string`.
* Bidirectional UTF-8/UTF-16/UTF-32 views. You can view `sys_string` as a sequence of UTF-8/16/32 characters and iterate forward or `backward` equally efficiently. Consider trying to find last instance of Unicode whitespace in UTF-8 data. Doing it as fast as finding the first instance is non-trivial. The views also work on any random access containers (C array, `std::array`, `std::vector`, `std::string`) of characters. Thus you can iterate in UTF-8 over `std::vector<char16_t>` etc.

## Why bother? Doesn't `std::string` work well?

An `std::string` storing UTF-8 (or `std::wstring` storing UTF-16 on Windows) works very well for some scenarios but fails miserably for others. `sys_string` class is an attempt to create something that works well where `std::string` would be a bad choice.
Specifically, `std::basic_string` is an STL container of a character type that owns its memory and controls it via a user-supplied allocator. These design choices make it very fast for direct character access but create the following problems:
* They rule out (efficient) reference-counted implementations. Which means that when you copy an `std::string` instance it must copy its content. Some of the penalty of that is alleviated by modern [small string optimization](https://akrzemi1.wordpress.com/2014/04/14/common-optimizations/) but this is, at best, a band-aid. There are workarounds, of course, such as using `std::shared_ptr<std::string>>` "when it matters" but they result in even more complexity for something that is quite fundamental to any data processing.
* They foreclose any ability to efficiently interchange data with some other string type. It becomes problematic if your code needs to frequently ping-pong data between C++ and your OS string abstraction. Consider Apple's platforms (macOS, iOS). Applications written for these platforms often have to extensively interoperate with code that requires usage of `NSString *` native string type. If you have to ping-pong string data a lot and/or store the same string data on both sides, using `std::string` will mean a large performance and memory penalty. 
* They make `std::basic_string` Unicode hostile. By being oblivious to difference between "storage unit" and a "character", `std::basic_string` cannot really handle encodings such as `UTF-8` or `UTF-16` where the two differ. Yes you can store data in these encodings in it but you need to be extremely careful how you use it. What will `erase(it)` do if the iterator points in the middle of 4-byte UTF-8 sequence? 

Finally, and unrelatedly to the above, `std::string` lacks some simple things that are taken for granted these days by users of pretty much all other languages. There is case insensitive comparisons, no "trim" or "split" etc. It is possible to write those yourself of course but here the Unicode-unfriendliness raises its ugly head. To do any of these correctly you need to be able to handle a string as a sequence of Unicode characters and doing so with `std::string` is cumbersome.


## Non-goals

The following requirements which other string classes often have are specifically non-goals of this library. 

* Support C++ allocators. Since `sys_string` is meant to interoperate with system string class/types, it necessarily has to use the same allocation mechanisms as those. 
* Have an efficient `const char * c_str()` method on all platforms. The goal of the library is to provide an efficient conversion to the native string types rather than specifically `char *`. While ability to get `char *` *is* provided everywhere it might involve additional memory allocations and other overhead. Note that on Linux `char *` is the system type so it can be obtained with 0 cost.
* Make `sys_string` an STL container. Conceptually a string is not a container. You can **view** contents of a string as a sequence of UTF-8 or UTF-16 or UTF-32 codepoints and the library provides such views which function as STL ranges. 
* Support non-Unicode "narrow" and "wide" character encodings. `sys_string` only understands Unicode. Conversions to/from non-Unicode encodings are a job for a different library. Specifically `char *` in any of the library's methods is required to be in UTF-8.
* Provide locale-dependent functionality. Properly supporting locales with Unicode is an important area but it belongs to another library, not this one. This library is focused on locale-independent behavior that works the same everywhere. For example `to_lower` methods implements locale-independent part of Unicode specification. (Final uppercase Σ transforms to ς but I always transforms to i)

## Performance

In general `sys_string` aims to have the same performance as hand-crafted code that uses corresponding native string types on every platforms. For example on macOS code using `sys_string` should be as fast as code manually using `NSString *`/`CFStringRef`. 
This needs to be kept in mind when evaluating whether `sys_string` is a better choice for your application that `std::string`. Continuing Apple's example an `std::string` is generally faster for direct character access than `NSString *` and thus `sys_string`. If your code rarely transfers data from `NSString *` to `std::string` and spends most of the time iterating over `std::string` characters then using `std::string` might be the right choice.

## Compatibility

This library has been tested with
* Xcode 12 on x86_64 and arm64
* MSVC 16.9 on x86_64
* Clang 11.0.5 under Android NDK on x86, x86_64, armeabi-v7a and arm64-v8a architectures
* GCC 9.3 on x86_64 Ubuntu 20.04


## Building

If you use CMake clone this repository and add the `lib` directory as subdirectory. Something like

```cmake
add_subdirectory(PATH_TO_SYS_STRING\lib, sys_string)
```

You need to have your compiler to default to at least C++17 or set `CMAKE_CXX_STANDARD` to at least 17 in order for build to succeed.

If you use a different build system all you need is to set your include path to `lib/inc` and compile the sources under `lib/cpp`. 

No special compilation flags are required except on Windows where `_CRT_SECURE_NO_WARNINGS` must be defined to avoid MSVC bogus warnings.
On Mac you need to link with `CoreFoundation` framework and on Windows with `runtimeobject.lib`.

### Configuration options

* `SYS_STRING_NO_S_MACRO` - set to 1 to disable short `S()` macro. See [Usage](doc/Usage.md#basics) for details

## Usage

See [Usage](doc/usage.md)

