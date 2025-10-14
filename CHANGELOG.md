# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),

## Unreleased

## Added
- This library now builds and works properly on Haiku OS

## [3.6] - 2025-09-26

### Added
- On Emscripten it is now possible to construct `sys_string` directly from `__externref_t` and convert `sys_string` back to it. 
  This is faster and usually more ergonomic that going through `Emval`. 
- On Emscripten it is now possible to make `sys_string` use [WASM JavaScript Builtins](https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/JavaScript_builtins) (if your WASM platform supports them). This can make construction of `sys_string` from `__externref_t` significantly faster. 
  Define `SYS_STRING_USE_WASM_JS_STRING` to 1 to enable this functionality.

## Fixed
- This library can now be used with `clang-cl` on Windows
- Bugs in BSTR storage support on 32-bit x86
- Warnings on clang 21

## [3.5] - 2025-07-16

### Fixed
- Incorrect CMake test for Python presence at build time

## [3.4] - 2025-06-27

### Fixed
- Inadvertent pessimization in `compare_no_case`
- Concepts rejecting `Char *` as eligible for addition to `sys_string`

## [3.3] - 2025-05-12

### Fixed

- CMake build now correctly handles when Python detected on the build machine is too old to be used
- Tests now run correctly with PyPy Python installation

## [3.2] - 2025-02-10

### Added

- `sys_string` can now be `normalize()`-ed to NFC and NFD.
- It is now possible to use ICU instead of internal data/code for case conversion, grapheme
  iteration and normalization. This makes those operations a tiny bit slower but saves 
  ~100kB in a final executable if you use all of them.

### Changed

- This library is now header only
- Speed improvements to case conversions, case insensitive comparisons and grapheme iteration.

### Fixed

- Addressed some false positive sanitizer warnings.

## [3.1] - 2025-01-10

### Added

- `grapheme_view` and `graphemes` adapter which provide ability to iterate over grapheme clusters in `sys_string` and any UTF range.

### Changed

- Unicode data has been optimized for better size/speed balance
- `sys_string_t::hash_type` has been changed from `unsigned` to `size_t` on some platforms. 

### Fixed

- Invalid character access in unicode mappings.
- Crash when sys_string_builder is re-used after `build()` on Apple and Python platforms.
- `utf_ref_view` and `utf_owning_view` now actually work with forward and input underlying ranges
- MSVC warnings when using `std::hash<sys_string>`

## [3.0] - 2024-12-02

This is a major release with some breaking changes

### Changed

- C++20 or higher is now required for compilation. In particular, the following C++20 features must be available:
  - Ranges support in standard library (`__cpp_lib_ranges >= 201911`)
  - Three-way comparison (spaceship operator)
  - `char8_t` type
  - `std::endian` support in standard library (`__cpp_lib_endian >= 201907`)
  - Minimal compilers known to work include: GCC 12, Clang 16, Apple Clang 15.4 and MSVC 17.6. 
- The library has been _range_-ified. 
  - All methods that used to accept iterator pairs now take iterator/sentinel pairs.
  - All these methods now also have overloads that accept ranges
  - Existing informal ranges (`sys_string::char_access`, `sys_string::utf_view`, etc.) are now
    formal ranges or views.
  - As part of the above `sys_string::utfX_view` classes has been renamed to `sys_string::utfX_access` (because they are
    not formally views as defined by standard library). The old names have been retained for compatibility but annotated
    as deprecated. Note that `sys_string_builder::utf_view` remains under the same name since it *is* a view.
  - Breaking change: as part of the above change the `sys_string::utf_access` and `sys_string_builder::utf_view` now 
    return distinct iterators and sentinels (that is they no longer satisfy `std::ranges::common_range` concept). 
    You will need to use ranges algorithms with their iterators (e.g. `std::ranges::for_each` rather than `std::for_each`).
  - The global `utf_view` template has been split into two: `utf_ref_view` that takes underlying range by reference (similar
    to `std::ref_view`) and `utf_owning_view` that owns a movable underlying range (similar to `std::owning_view`). These
    are automatically produced by `as_utf` range adapter closures (see below in Added section)
  - Breaking change: the non-standard `Cursor` classes has been removed.
- The library has been _concept_-ified.  
  - Most templated library calls now have concepts checks that validate their argument types. 
  - Primitive `std::enable_if` used before have been subsumed by these and removed.
- Unicode data used for case folding and whitespace detection has been updated to version 16.0.0

### Added
- `sys_string_t` can now be `+`-ed with any forward range of any type of character (including C strings and std::string). 
  This results in a the same optimized addition as when adding `sys_string_t` objects.
- `sys_string_t` objects can now be formatted via `std::format` (if available in your library). On platforms 
   where `wchar_t` is UTF-16 or UTF-32 you can also use wide character formatting.
- `sys_string_t::std_format` method. This formats a new `sys_string_t` (similar to the existing `sys_string_t::format`) 
  but uses `std::format` machinery and formatting string syntax.
- Range adapter closures: `as_utf8`, `as_utf18`, `as_utf32` and generic `as_utf<encoding>` . 
  - These can be used to create `utf_ref_view`/`utf_owning_view` from any range/view. For example `as_utf16(std::string("abc"))`
  - If you library supports custom adapter closures (usually `__cpp_lib_ranges >= 202202L`) they can be used in 
    view pipelines like `std::string("abc") | as_utf16 | std::views::take(2)` etc.

### Fixed
- Printing `sys_string_t` objects into `std::ostream` (and `std::wostream` if available) now functions correctly in presence 
  of stream formatting flags. Flags are currently ignored. This might change in a future version.
- Printing/formatting `sys_string_t` objects that use `char` storage type now does not perform sanitizing transcoding. The content
  of the string is printed as-is. This allows faithful round-tripping and support for invalid Unicode for those scenarios. Similar
  behavior applies to `wchar_t` on platform where it is UTF-16 or UTF-32.
- `operator<<` no longer pollutes global namespace

## [2.22] - 2025-10-14

## Added
- This library now builds and works properly on Haiku OS

## [2.21] - 2025-09-26

## Fixed
- This library can now be used with `clang-cl` on Windows
- Bugs in BSTR storage support on 32-bit x86
- Warnings on clang 21

## [2.20] - 2025-07-16

### Fixed
- Incorrect CMake test for Python presence at buildtime

## [2.19] - 2025-05-12

### Fixed

- Error on missing `std::char_traits<const char>` on newer compilers
- CMake build now correctly handles when Python detected on the build machine is too old to be used
- Tests now run correctly with PyPy Python installation


## [2.18] - 2025-02-27

### Fixed

- Unresolved symbols errors with GCC 10.2

## [2.17] - 2025-02-10

### Added

- `sys_string` can now be `normalize()`-ed to NFC and NFD (back-ported from version 3).

### Changed

- Supported Unicode version has been bumped to 16 (back-ported from version 3).
- Speed improvements to case conversions and case insensitive comparisons (back-ported from version 3).

### Fixed

- Addressed some false positive sanitizer warnings.

## [2.16] - 2025-01-10

### Fixed
- Crash when sys_string_builder is re-used after `build()` on Apple and Python platforms.
- MSVC warnings when using `std::hash<sys_string>`

### Changed
- `sys_string_t::hash_type` has been changed from `unsigned` to `size_t` on some platforms. 


## [2.15] - 2025-01-07

### Fixed

- Invalid character access in unicode mappings.

## [2.14] - 2024-05-02

### Fixed

- Missing standard header causing compilation error on some systems.

## [2.13] - 2023-11-18

### Fixed
- Compilation error with Python strings under Python 3.12
- Apple CLang compilation errors with XCode 15

## [2.12] - 2023-07-31

### Fixed
- Fixed CLang compilation errors
- Fixed CMake installation not producing proper version when run from extracted official archive
- Fixed various compiler warnings

## [2.11] - 2023-07-24

### Changed
- Updated CMake configuration to modernize it and allow local installation

## [2.10] - 2023-03-30

### Fixed
- Made Python strings work with GCC

## [2.9] - 2023-03-30

### Fixed
- Making Python strings work with PyPy implementation

## [2.8] - 2023-03-18

### Added
- Added support for Python strings

### Changed
- Updated documentation

## [2.7] - 2023-02-02

### Added
- Added support for Emscripten/JavaScript storage and interop. Strings are stored in UTF16 encoding and transformation methods to/from JavaScript are available similar to how Android/Java interop works. Docs to follow.

### Fixed
- Fixed creation of longer Apple strings
- Fixed warnings in newer Xcode versions.

## [2.5] - 2022-11-07

### Added
- Made generic Unix support work for any Unix, not just Linux/BSD

### Fixed
- Fixed broken `swap()`` method for some storage types

## [2.4] - 2022-07-02

### Added
- Fixed compilation with GCC >= 10 and <= 11.2 which were broken due to compiler bug. GCC 9 and >= 11.3 are unaffected.

### Changed
- Updated Catch library to fix tests not building on some Linux versions

## [2.3] - 2022-06-09

### Added
- FreeBSD is now supported (identical to Linux)
- Generic `char *`` storage is now supported on Windows

### Fixed
- Removed spurious output during CMake build


## [2.2] - 2022-05-23

### Added
- String partitioning methods: 
  * Returning one side 
    * `suffix_after_first(str_or_char)`
    * `prefix_before_first(str_or_char)`
    * `suffix_after_last(str_or_char)`
    * `prefix_before_last(str_or_char)`
  * Returning both sides (as `std::pair`) 
    * `partition_at_first(str_or_char)`
    * `partition_at_last(str_or_char)`

## [2.1] - 2022-05-19

### Added
- Support for GCC 12

### Fixed
- Workarounds for Xcode 13 libc++ issues
- Minor internal tweaks and improvements

### Changed
- Better naming in parser configuration

### Changed
- Updated documentation

## [2.0] - 2021-12-07

### Changed
- `sys_string` is now a template on storage policy which leads to
   - ability to select different storage types for each platform
   - ability to use multiple storage types in the same project, if desired

### Added
- New storage types on Windows: `BSTR` and plain `wchar_t *`
   - Breaking change: default storage type switched to plain `wchar_t *`

### Fixed 
- Performance improvements

### Fixed
- Various minor bug fixes


## [1.1] - 2021-11-05

### Fixed

- Bug fixes and performance improvements

## [1.0] - 2021-05-03

### Added
- Initial version

[1.0]: https://github.com/gershnik/sys_string/releases/v1.0
[1.1]: https://github.com/gershnik/sys_string/releases/v1.1
[2.0]: https://github.com/gershnik/sys_string/releases/v2.0
[2.1]: https://github.com/gershnik/sys_string/releases/v2.1
[2.2]: https://github.com/gershnik/sys_string/releases/v2.2
[2.3]: https://github.com/gershnik/sys_string/releases/v2.3
[2.4]: https://github.com/gershnik/sys_string/releases/v2.4
[2.5]: https://github.com/gershnik/sys_string/releases/v2.5
[2.7]: https://github.com/gershnik/sys_string/releases/v2.7
[2.8]: https://github.com/gershnik/sys_string/releases/v2.8
[2.9]: https://github.com/gershnik/sys_string/releases/v2.9
[2.10]: https://github.com/gershnik/sys_string/releases/v2.10
[2.11]: https://github.com/gershnik/sys_string/releases/v2.11
[2.12]: https://github.com/gershnik/sys_string/releases/v2.12
[2.13]: https://github.com/gershnik/sys_string/releases/v2.13
[2.14]: https://github.com/gershnik/sys_string/releases/v2.14
[2.15]: https://github.com/gershnik/sys_string/releases/v2.15
[2.16]: https://github.com/gershnik/sys_string/releases/v2.16
[2.17]: https://github.com/gershnik/sys_string/releases/v2.17
[2.18]: https://github.com/gershnik/sys_string/releases/v2.18
[2.19]: https://github.com/gershnik/sys_string/releases/v2.19
[2.20]: https://github.com/gershnik/sys_string/releases/v2.20
[2.21]: https://github.com/gershnik/sys_string/releases/v2.21
[2.22]: https://github.com/gershnik/sys_string/releases/v2.22
[3.0]: https://github.com/gershnik/sys_string/releases/v3.0
[3.1]: https://github.com/gershnik/sys_string/releases/v3.1
[3.2]: https://github.com/gershnik/sys_string/releases/v3.2
[3.3]: https://github.com/gershnik/sys_string/releases/v3.3
[3.4]: https://github.com/gershnik/sys_string/releases/v3.4
[3.5]: https://github.com/gershnik/sys_string/releases/v3.5
[3.6]: https://github.com/gershnik/sys_string/releases/v3.6
