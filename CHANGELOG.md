# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),

## Unreleased

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
