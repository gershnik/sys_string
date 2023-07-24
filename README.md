## SysString

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-BSD-brightgreen.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Tests](https://github.com/gershnik/sys_string/actions/workflows/test.yml/badge.svg)](https://github.com/gershnik/sys_string/actions/workflows/test.yml)

This library provides a C++ string class template `sys_string_t` that is optimized for **interoperability with external native string type**. It is **immutable**, **Unicode-first** and exposes convenient **operations similar to Python or ECMAScript strings**. It uses a separate `sys_string_builder_t` class template to construct strings. It provides fast concatenation via `+` operator that **does not allocate temporary strings**. 
The library exposes bidirectional UTF-8/UTF-16/UTF-32 views of `sys_string_t` as well as of any random access containers 
of characters.

## What does it mean?

* **Interoperability with external native string types** means that SysString makes conversions to and from such types as efficient as possible and, ideally, zero-cost operations. Native string types are things like:

    * `NSString *` or `CFStringRef` on macOS/iOS
    * Java `String` on Android
    * Python `str` in Python extensions
    * JavaScript `String` on Emscripten/WebAssembly
    * `const wchar_t *`, `HSTRING` or `BSTR` on Windows 
    * `const char *` on Linux.  

    `sys_string_t<Storage>` and  `sys_string_builder_t<Storage>` are parametrized on `Storage` which defines what kind of native string type to use internally and interoperate with. Different `Storage` implementations are provided for all the external types above.

    For example the storage for Apple's platforms uses `NSString *` internally, allowing zero cost conversions between C++ and native sides. 

    On Android and Emscripten/WebAssembly no-op conversions from C++ to native strings are impossible for technical reasons. 
    The storage for these platforms' strings still makes conversions as cheap as possible (avoiding UTF conversions for example).

    The library also provides typedefs `sys_string`/`sys_string_builder` that use the "default" storage type on each platform (you can change which one it is via compilation options). Regardless of which storage is the default you can always directly use other specializations in your code if necessary.


* **Immutability.** String instances cannot be modified. To do modifications you use a separate "builder" class. This is similar to how many other languages do it and results in improved performance and elimination of whole class of errors. 

* **Unicode-first.** Instances of `sys_string_t` always store Unicode characters in either UTF-8, UTF-16 or UTF-32, depending on their storage. Iteration can be done in all of these encodings and all operations (case conversion, case insensitive comparisons, trimming) are specified as actions on sequence of Unicode codepoints using Unicode algorithms. 

* **Operations similar to Python or ECMAScript strings.** You can do things like `rtrim`, `split`, `join`, `starts_with` etc. on `sys_string_t` in a way proven to be natural and productive in those languages.

* **Concatenation does not allocate temporaries.** You can safely do things like `result = s1 + s2 + s3`. It will result in **one** memory allocation and one `memcpy` of `s1`, `s2` and `s3` content into the final result. Not 2 allocations and 5 copies like in other languages or with `std::string`.

* **Bidirectional UTF-8/UTF-16/UTF-32 views**. You can view `sys_string_t` as a sequence of UTF-8/16/32 characters and iterate forward or __backward__ equally efficiently. Consider trying to find last instance of Unicode whitespace in UTF-8 data. Doing it as fast as finding the first instance is non-trivial. The views also work on any random access containers (C array, `std::array`, `std::vector`, `std::string`) of characters. Thus you can iterate in UTF-8 over `std::vector<char16_t>` etc.

## Why bother? Doesn't `std::string` work well?

An `std::string` storing UTF-8 (or `std::wstring` storing UTF-16 on Windows) works very well for some scenarios but fails miserably for others. `sys_string` class is an attempt to create something that works well in situations `std::string` would be a bad choice.
Specifically, `std::basic_string` is an STL container of a character type that owns its memory and controls it via a user-supplied allocator. These design choices make it very fast for direct character access but create the following problems:

* They rule out (efficient) reference-counted implementations. Which means that when you copy an `std::string` instance it must copy its content. Some of the penalty of that is alleviated by modern [small string optimization](https://akrzemi1.wordpress.com/2014/04/14/common-optimizations/) but this is, at best, a band-aid. There are workarounds, of course, such as using `std::shared_ptr<std::string>>` "when it matters" but they result in even more complexity for something that is quite fundamental to any data processing.

* They foreclose any ability to efficiently interchange data with some other string type. It becomes problematic if your code needs to frequently ping-pong data between C++ and your OS string abstraction. Consider Apple's platforms (macOS, iOS). Applications written for these platforms often have to extensively interoperate with code that requires usage of `NSString *` native string type. If you have to ping-pong string data a lot and/or store the same string data on both sides, using `std::string` will mean a large performance and memory penalty. 

* They make `std::basic_string` Unicode hostile. By being oblivious to difference between "storage unit" and a "character", `std::basic_string` cannot really handle encodings such as `UTF-8` or `UTF-16` where the two differ. Yes you can store data in these encodings in it but you need to be extremely careful how you use it. What will `erase(it)` do if the iterator points in the middle of 4-byte UTF-8 sequence? 

Finally, and unrelatedly to the above, `std::string` lacks some simple things that are taken for granted these days by users of pretty much all other languages. There is case insensitive comparisons, no "trim" or "split" etc. It is possible to write those yourself of course but here the Unicode-unfriendliness raises its ugly head. To do any of these correctly you need to be able to handle a string as a sequence of Unicode characters and doing so with `std::string` is cumbersome.


## Non-goals

The following requirements which other string classes often have are specifically non-goals of this library. 

* Support C++ allocators. Since `sys_string_t` is meant to interoperate with system string class/types, it necessarily has to use the same allocation mechanisms as those. 

* Have an efficient `const char * c_str()` method on all platforms. The goal of the library is to provide an efficient conversion to the native string types rather than specifically `const char *`. While ability to obtain `const char *` *is* provided everywhere, it might involve additional memory allocations and other overhead. Note that on Linux `char *` is the system type so it can be obtained with 0 cost.

* Make `sys_string_t` an STL container. Conceptually a string is not a container. You can **view** contents of a string as a sequence of UTF-8 or UTF-16 or UTF-32 codepoints and the library provides such views which function as STL ranges. 

* Support non-Unicode "narrow" and "wide" character encodings. `sys_string_t` only understands Unicode. Conversions to/from non-Unicode encodings are a job for a different library. Most significantly it means that any `char *` passed to this library's methods is required to be in UTF-8.

* Provide locale-dependent functionality. Properly supporting locales with Unicode is an important area but it belongs to another library, not this one. This library is focused on locale-independent behavior that works the same everywhere. For example `to_lower` methods implements locale-independent part of Unicode specification. (Final uppercase Σ transforms to ς but I always transforms to i)

## Performance

In general `sys_string_t` aims to have the same performance of its operations as best hand-crafted code that uses corresponding native string types on every platforms. For example on macOS code using `sys_string` should be as fast as code manually using `NSString *`/`CFStringRef`. 

This needs to be kept in mind when evaluating whether `sys_string` is a better choice for your application than `std::string`. Continuing Apple's example, an `std::string` is generally faster for direct character access than `NSString *` and thus faster than `sys_string` too. If your code rarely transfers data from `NSString *` to `std::string` and spends most of the time iterating over `std::string` characters then using `std::string` might be the right choice.

Another way to look at it is that `sys_string_t` sometimes trades micro-benchmarking performance of individual string operations for reduced copying, allocations and memory pressure overall. Whether this is a right tradeoff for you depends on specifics of your codebase.

## Compatibility

This library has been tested with
* Xcode 13 - 14 on x86_64 and arm64
* MSVC 16.9 - 17.4 on x86_64
* Clang 12.0.5 under Android NDK, ANDROID_PLATFORM=19 on x86, x86_64, `armeabi-v7a` and `arm64-v8a` architectures
* GCC 9.3 - 11.3 on x86_64 Ubuntu 20.04 - 22.04
* Emscripten 3.1.21 

## Usage

* [Building/Integrating](doc/Building.md)
* [Usage](doc/Usage.md)

