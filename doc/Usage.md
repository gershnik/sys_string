
<!-- TOC -->

- [Header](#header)
- [Namespace](#namespace)
- [Error handling](#error-handling)
- [Basics](#basics)
- [Construction](#construction)
    - [Conversions from invalid Unicode](#conversions-from-invalid-unicode)
- [Platform-specific conversions](#platform-specific-conversions)
- [Adding Strings](#adding-strings)
- [Multiplying Strings](#multiplying-strings)
- [Comparing Strings](#comparing-strings)
    - [Case insensitive comparison](#case-insensitive-comparison)
- [Iterating over string content](#iterating-over-string-content)
    - [Storage iteration](#storage-iteration)
    - [UTF iteration](#utf-iteration)
    - [UTF iteration over any C++ character range](#utf-iteration-over-any-c-character-range)
    - [Grapheme iteration](#grapheme-iteration)
- [Substrings](#substrings)
- [Accessing C strings](#accessing-c-strings)
- [Accessing storage as a C array](#accessing-storage-as-a-c-array)
- [Building and modifications](#building-and-modifications)
- [Utility methods](#utility-methods)
    - [Check if the string is empty](#check-if-the-string-is-empty)
    - [Transformations](#transformations)
    - [Printf-style construction of a string](#printf-style-construction-of-a-string)
    - [std::format-style construction of a string](#stdformat-style-construction-of-a-string)
    - [Printing into std::ostream](#printing-into-stdostream)
    - [Formatting with std::format](#formatting-with-stdformat)
    - [Trimming](#trimming)
    - [Digression: sys_string_or_char concept](#digression-sys_string_or_char-concept)
    - [Splitting](#splitting)
    - [Joining](#joining)
    - [Prefix, suffix and infix handling](#prefix-suffix-and-infix-handling)
    - [Replacing content](#replacing-content)

<!-- /TOC -->

## Header

Everything needed to use `sys_string` and all related functionality is declared in the `sys_string/sys_string.h` header. 

An `#include <sys_string/sys_string.h>` directive is assumed in all the examples.

## Namespace

Everything in this library is under namespace `sysstr`. 

A `using namespace sysstr` directive is assumed in all the examples.

## Error handling

Most methods in this library are `noexcept`. Methods that aren't, usually involve memory allocation or creation of system string types that can fail (usually also due to memory allocation failure). In such cases, errors are reported via exceptions. All thrown exceptions derive from `std::exception`. When it is certain that the failure is due to memory allocation, `std::bad_alloc` is raised. Otherwise, the library throws a more generic `std::runtime_error`.


## Basics

The two main classes provided by this library are `sys_string` and `sys_string_builder`. These are actually aliases (typedefs) of instantiations
of `template<class Storage> class sys_string_t` and `template<class Storage> class sys_string_builder_t` with a `Storage` chosen based on the platform
and compilation options. A `Storage` is a policy class that defines what kind of system string type is stored inside a `sys_string`. 
On every supported platform, there are one or more storage types available. You can always use other storage types in addition to 
the default one. The following table describes the available `sys_string_t` instantiations, their availability and when they are made to be 
the default `sys_string`.

| Type                     | Interoperates with         |  Availability   | Default `sys_string` when
|--------------------------|----------------------------|-----------------|---------------
| `sys_string_generic`     | Basic `char *`             | Always          | On non-Apple Unix or anywhere if the `SYS_STRING_USE_GENERIC` macro is set to 1
| `sys_string_cfstr`       | `CFStringRef`/`NSString *` | Apple platforms | No other macros set
| `sys_string_win_generic` | Basic `wchar_t *`          | Windows         | No other macros set
| `sys_string_bstr`        | `BSTR`                     | Windows         | If the `SYS_STRING_WIN_BSTR` macro is set to 1
| `sys_string_hstring`     | `HSTRING`                  | Windows         | If the `SYS_STRING_WIN_HSTRING` macro is set to 1
| `sys_string_pystr`       | Python `PyObject *`        | If `SYS_STRING_ENABLE_PYTHON` or `SYS_STRING_USE_PYTHON` is set to 1 | If `SYS_STRING_USE_PYTHON` is set to 1
| `sys_string_android`     | `jstring`                  | Android         | No other macros set
| `sys_string_emscripten`  | Emscripten JavaScript strings (via `EM_VAL`) | Emscripten | No other macros set

Apart from storage-specific conversions, all of these types have the same interface and are simply described below as being a `sys_string`.

## Construction

In the simplest case, a `sys_string` literal can be created statically, that is, having its content embedded into the executable image.

```cpp
const sys_string & str = S("Hello!");
```
`S` unfortunately has to be a macro. If such a short macro name offends you or collides with another macro in your program, you can disable it via `#define SYS_STRING_NO_S_MACRO 1`. In this case, you can use the verbose version:
```cpp
const sys_string & str = SYS_STRING_STATIC("Hello!");
``` 

You can also create a string by converting it from a single character, C string, C string + length, `std::basic_string`, `std::basic_string_view`, `std::vector`, `std::span` or any other `std::ranges::contiguous_range` of characters. 
The type of character can be one of:
* `char`, `signed char`, `unsigned char` (these are always assumed to be UTF-8)
* `char8_t`
* `char16_t`
* `char32_t`.

```cpp
sys_string str1('a');
assert(str1 == S("a"));

sys_string str2(u"Hello!");
assert(str2 == S("Hello!"));

sys_string str3(U"Hello!", 2);
assert(str3 == S("He"));

std::string cppstr = "World";
sys_string str4(cppstr);
assert(str4 == S("World"));

std::vector<char16_t> vec = {u'a', u'b', u'c'};
sys_string str5(vec);
assert(str5 == S("abc"));
```

For the constructor taking a character pointer, `nullptr` is a valid value and results in an empty `sys_string`. 

### Conversions from invalid Unicode

A `sys_string_t` internally stores data in one of: UTF-8 (`char`), UTF-16 (`char16_t`) or UTF-32 (`char32_t`) encodings, depending on the configuration. The specific type used is represented by the `sys_string_t::storage_type` typedef. What happens when you create a `sys_string_t` from external data that contains invalid Unicode? It depends on whether the external data is in the same encoding as the storage type.
* If the type is the same, then the data is copied into `sys_string_t` verbatim. This allows you to store and manipulate invalid Unicode in a platform-specific way. For example, Windows filenames, while ostensibly in UTF-16, can contain unpaired surrogates. `sys_string_t` allows you to handle these, but you need to make sure you pass them in as the OS gives them - in `wchar_t *` or `char16_t *`.
* If the type is not the same, then the result is **unspecified but valid** Unicode. In practice, invalid content will be replaced by one or more 
['REPLACEMENT CHARACTER' (U+FFFD)](https://www.fileformat.info/info/unicode/char/fffd/index.htm) - similar to how a text editor or a browser would deal with it. The exact number of characters and the replacement algorithm in general are deliberately left unspecified. Thus, portable code (that is, code that is agnostic to the type of `sys_string_t` storage) cannot rely on invalid Unicode having a specific representation - this is by design.

## Platform-specific conversions 

In addition to the portable conversions described above, `sys_string` supports platform-specific conversions to and from various system string types.
Those are described on the following pages.

* [Apple](Apple.md)
* [Windows](Windows.md)
* [Android](Android.md)
* [Linux](Linux.md)
* [Emscripten](Emscripten.md)
* [Python](Python.md)

## Adding Strings

You can combine strings using the `+` operator. 
```cpp
sys_string str1 = S("a");
sys_string str2 = S("b");
sys_string str3 = S("c");
sys_string str = str1 + str2 + str3;
assert(str == S("abc"));
```

Unlike `std::string`, the addition of `sys_string_t` uses [expression templates](https://en.wikipedia.org/wiki/Expression_templates) to avoid the creation of temporaries. This means that addition is cheap and you have no reason to avoid it. 

You can add `sys_string_t`, single `char32_t` characters and any range of any characters this way.

For example:

```cpp
using namespace std::literals;

sys_string result = S("a") + U'b' + "cd" + "ef"s + u"gh"sv + U"ij" + std::vector{'k', 'l'};
assert(result == S("abcdefghijkl"));
```

> [!WARNING]
> You must not use `auto` to declare the addition result. The result of an addition is a special temporary that only performs actual concatenation when converted to `sys_string_t`. Using `auto` declares a variable of that temporary type, which will result in dangling pointers.

```cpp
auto res = sys_string("abc") + sys_string("xyz"); //Bad!
//using res here is undefined behavior - it refers to already destroyed temporaries
if (res == S("abcxyz")) 
   ...
```

## Multiplying Strings

Similar to what you can do in Python, you can multiply strings and integral values. 
The result is the original string repeated the specified number of times.
```cpp
sys_string str = S("abc");
sys_string res = str * 3;
assert(res == S("abcabcabc"));
// you can multiply from either side
res = 3 * str;
assert(res == S("abcabcabc"));
// 0 or negative multipliers result in an empty string
assert(str * 0  == S(""));
assert(str * -1 == S(""));
``` 

## Comparing Strings

`sys_string_t` supports the normal inventory of comparison operators: `==`, `!=` and `<=>` (from which `<`, `<=` etc. are auto-generated). Comparison of strings is always performed as a binary comparison of internal representations, **not** Unicode codepoints and **not** normalizing Unicode. That is:

```cpp
//these strings are canonically equivalent but not the same
assert(S("Cafe\u0301") != S("Café"));
```

The comparisons are defined this way for performance. If you do need canonical equivalence, see the [`normalize()`](#transformations) method below.

```cpp
assert(S("Cafe\u0301").normalize(normalization::nfc) == S("Café").normalize(normalization::nfc));
```

This implies that while identical strings will compare equal and all strings are strongly ordered, the specific order might differ from the one a human would expect. If you do require human-oriented ordering, the correct thing to do is to use a full-fledged collator from another library such as ICU or Windows NLS.

There is also a free function `compare` that behaves identically to the `<=>` operator.

### Case insensitive comparison

Unlike `std::string`, `sys_string_t` also supports case-insensitive comparisons. 
This is done by binary comparing the result of Unicode case folding. 

```cpp
assert(compare_no_case(S("maße"), S("MASSE")) == std::strong_ordering::equal);
```

Both `compare` and `compare_no_case` return `std::strong_ordering`.

## Iterating over string content

You can iterate over `sys_string` in 5 possible ways:
* Over a sequence of UTF-32, `char32_t` codepoints. This iteration is unidirectional, either forward or reverse and 'sanitizing'.
* Over a sequence of UTF-16, `char16_t` units. This iteration is unidirectional, either forward or reverse and 'sanitizing'.
* Over a sequence of UTF-8, `char` units. This iteration is unidirectional, either forward or reverse and 'sanitizing'.
* Over a sequence of "storage units". This iteration is random-access and NOT sanitizing.
* Over a sequence of "grapheme clusters" (or graphemes) for short. This iteration is different from the others and is described fully
  [below](#grapheme-iteration).

A "storage unit" is a platform-dependent type (either `char`, `char16_t` or `char32_t`) that is used to actually store the data in `sys_string`. 
It is represented by the type `sys_string::storage_type`. Iterating over storage units is the fastest way to iterate and the only way to get access 
to "invalid Unicode" that might be stored in `sys_string`. 

All other types of iteration are "sanitizing". It is possible for a string to contain invalid Unicode - see [Conversions from invalid Unicode](#conversions-from-invalid-unicode). When this happens, a 'sanitizing' iteration will replace invalid units with one or more replacement characters `U+FFFD` in an unspecified manner. Note that it is not even guaranteed that forward and reverse iteration will produce the same replacements. Thus, when dealing with invalid Unicode, all you can expect is that sanitizing iteration will always produce valid Unicode, but you cannot specifically guarantee what it would be. If you do need to predictably handle invalid Unicode content, you need to iterate over storage units.


### Storage iteration

To iterate over string storage units you first need to create a `sys_string::char_access` object. This object is cheap to create and acts as a "reference" to the actual `sys_string`. It is a "reference object" and is **only valid while the original string exists**. It is meant to be used on the stack, during computation and then discarded rather than stashed away for subsequent use.

`sys_string::char_access` is a [random_access_range](https://en.cppreference.com/w/cpp/ranges/random_access_range) but not a [view](https://en.cppreference.com/w/cpp/ranges/view) and not even a [viewable_range](https://en.cppreference.com/w/cpp/ranges/viewable_range). 

Thus, in order to use this range in view pipelines, you need to have it as an lvalue and access it via `std::ranges::views::all` or `std::ranges::ref_view`. For example:

```cpp
using namespace std::ranges;
using namespace std::views;

sys_string str = S("abc");
sys_string::char_access access(str);
assert(equal(all(access) | take(1), std::array{sys_string::storage_type('a')}));
```

`sys_string::char_access` is a read-only range and is the fastest way to go over the `sys_string` content.
The size of the range is given by the `size()` member (it is a [sized_range](https://en.cppreference.com/w/cpp/ranges/random_access_range)). 

There is also an `operator[]` that allows direct indexing access to the sequence.

```cpp
sys_string str("Hello");
for(sys_string::storage_type c: sys_string::char_access(str)) {
    ...
}
sys_string::char_access access(str);
std::cout << "storage size is " << access.size() << '\n';
std::cout << "unit at [0] is " << access[0] << '\n';
```

On some platforms `sys_string::char_access` has a `data()` member that returns a pointer to a contiguous array of storage units. The combination of `data()` and `size()` allows fast interop with platform-specific code.

### UTF iteration

To perform UTF iteration you need to construct one of: `utf32_access`, `utf16_access` or `utf8_access` from `sys_string`. 
These are read-only [forward_ranges](https://en.cppreference.com/w/cpp/ranges/forward_range) but not [views](https://en.cppreference.com/w/cpp/ranges/view) and not even [viewable_ranges](https://en.cppreference.com/w/cpp/ranges/viewable_range). 

```cpp
sys_string str("Hello");
for(char32_t c: sys_string::utf32_access(str)) {
    ...
}
for(char16_t c: sys_string::utf16_access(str)) {
    ...
}
for(char c: sys_string::utf8_access(str)) {
    ...
}
```

Note that all these ranges have sentinel types (results of `end()`, `cend()` etc.) _different_ from their iterator types. Thus, they are not
[common_ranges](https://en.cppreference.com/w/cpp/ranges/common_range) and generally require 
[new range algorithms](https://en.cppreference.com/w/cpp/algorithm/ranges) from the `std::ranges` namespace rather than traditional ones.
For example:

```cpp

sys_string str = S("abc");
sys_string::utf32_access view(str);
std::vector<char32_t> dest;

//This won't compile
//std::copy(view.begin(), view.end(), std::back_inserter(dest));

//But this will
std::ranges::copy(view.begin(), view.end(), std::back_inserter(dest));

//Or simply
std::ranges::copy(view, std::back_inserter(dest));

```

Similar to `sys_string::char_access` (which they actually store inside) or `std::string_view`, these views are reference types. Their instances are **only valid as long as the original string exists**.


Despite being `forward_range` as far as the standard library is concerned, these ranges also allow reverse iteration. That is, they 
have reverse_iterator type and `rbegin`/`rend` methods. There is currently no standard concept that describes such a range, so this library provides
`sysstr::reverse_traversable_range`. 

The fact that these ranges are reversible rather than bidirectional is due to two issues.

The first is efficiency. Allowing a decoding iterator to move in the opposite direction will increase its size and won't be as efficient as a forward move.

The second is the 'sanitizing' iteration. Code using bidirectional iterators usually assumes that:
  
```cpp
iterator it1 = ...;
iterator it2 = it1;
it2 = ++(--it2);
assert(it2 == it1);
```
  
However, with sanitizing iteration this might not necessarily be the case. If a substitution is made for illegal characters, the amount of movement and returned values may differ for forward and reverse iteration. Thus, to avoid subtle issues, UTF range iterators disallow `--`. If you want to iterate in reverse, simply use `rbegin()`/`rend()`. You can also convert forward iterators to reverse and vice versa using the `sysstr::ranges::make_reverse_iterator(view, it)` call. 

```cpp
utf32_access access(...);
utf32_access::iterator it = ...;

utf32_access::reverse_iterator rit = access.reverse(it);
//or
//utf32_access::reverse_iterator rit = ranges::make_reverse_iterator(access, it);
utf32_access::iterator it1 = access.reverse(rit);
//or
//utf32_access::iterator it1 = ranges::make_reverse_iterator(access, rit);
assert(it1 == it);

//you can also do reverse on sentinels
utf32_access::reverse_iterator rfirst = access.reverse(access.end());
utf32_access::iterator first = access.reverse(access.rend());

```

### UTF iteration over any C++ character range

Since the internal facility to perform UTF iteration is quite generic, this library exposes it to allow you to perform UTF iteration over any C++ input range of compatible characters (`char`, `char8_t`, `char16_t`, `char32_t`, and possibly `wchar_t` on platforms where it is encoded in UTF-16 or UTF-32). At the time of this writing, there is work on including something similar to the C++ standard library, but, even if eventually approved, it will be a long time before it becomes available. 

You access UTF sequences by using view adapters. For example:

```cpp
std::string str = "😀😜";
for (char16_t c: as_utf16(str)) {
    ...
}
for (char32_t c: as_utf32(str)) {
    ...
}

for (char c: as_utf8(u"😀😜")) {
    ...
}
```

The `as_utf8`, `as_utf16` and `as_utf32` adapters produce view objects (e.g. fulfilling the [std::ranges::view](https://en.cppreference.com/w/cpp/ranges/view) concept) that can be used like any standard view. You can iterate over them in a for-each loop, pass their iterators or themselves to range algorithms, etc.

If your standard library supports user-supplied [range adapter closures](https://en.cppreference.com/w/cpp/ranges/range_adaptor_closure), these views can also be used in view pipelines:

```cpp
as_utf8(u"😀😜") | std::views::take(1) | ...
```

### Grapheme iteration

Sometimes even UTF-32 iteration is not what you need. Many user-perceived "characters" are actually composed of multiple
UTF-32 codepoints. The Unicode standard defines a [grapheme cluster](http://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries) as 
what corresponds to a user's notion of a character. A single grapheme cluster, or grapheme for short, can contain one or more Unicode
codepoints. 

This library allows you to easily iterate over grapheme clusters in `sys_string_t` content as well as in any C++ 
[forward_range](https://en.cppreference.com/w/cpp/ranges/forward_range) of compatible character type.

To iterate over graphemes, you need to construct an instance of `grapheme_view` directly or use the `graphemes` view adapter. In either case,
you need to supply a **view** of characters to iterate over. The view can be a reference to `sys_string_t::char_access`, `sys_string_t::utfX_access`
or any other compatible forward view.

The "values" returned from `grapheme_view` are `std::ranges::subrange`s of the underlying view containing graphemes.

To put it all in context, here is how you can iterate over all graphemes in a `sys_string`.

```cpp
sys_string str = S("क्त्य");  //6 Unicode codepoints but one grapheme!
sys_string::char_access access(str);
for (auto grapheme_range: graphemes(access)) {
    //grapheme_range is a subrange of sys_string::char_access::iterator
    sys_string grapheme(grapheme_range);
}
```

A `grapheme_view` is reversible, that is, it can be iterated in both directions. Here is how to accomplish a common task -
safely remove the last "character" from a string (see [Substrings](#substrings) below for details on how to obtain parts of a string):

```cpp
sys_string str = S("abक्त्य");
sys_string::char_access access(str);
auto gr_view = graphemes(access);
if (auto rit = gr_view.rbegin(); rit != gr_view.rend()) {
    auto grapheme = *rit;
    str = sys_string(access.begin(), grapheme.begin());
}
assert(str == S("ab"));
```

You can easily extend this to removing any number of trailing characters.


## Substrings

You can obtain a substring of a `sys_string` in two ways:

* From a pair of `sys_string::utf32_access::iterator`s (or a range that provides them). This will "cut" the string at a Unicode codepoint boundary, ensuring that the output is well-formed. This is the preferred way to slice strings.

```cpp
const sys_string & str = S("🤢abc");
sys_string::utf32_access view(str);
auto first = view.begin();
auto last = std::next(first, 1);
sys_string substring(first, last);
assert(substring == S("🤢"));
```

* From a pair of `sys_string::char_access::iterator`s (or a range that provides them). Since `char_access` iterates over the actual storage units, this allows you to "cut" the string anywhere. This way of slicing is **dangerous**, since you can end up slicing the string in the middle of a Unicode character. Unless this is exactly what you want to do for some reason, this would likely result in bugs in your code. Remember that sanitizing UTF iteration over such a string will produce substitution characters.

```cpp
const sys_string & str1 = S("abc");
sys_string::char_access access1(str1);
auto first = access1.begin();
auto last = first;
++last;
sys_string substring1(first, last);
//This works because 'a' is a single character in any UTF encoding
assert(substring1 == S("a")); 

const sys_string & str2 = S("🤢bc");
sys_string::char_access access2(str2);
first = access2.begin();
last = first;
++last;
sys_string substring2(first, last);
//Unless the internal storage is UTF-32 (which is currently not the case with any storage), the
//substring will NOT be equal to the first character
assert(substring2 != S("🤢")); 
```

What if you are iterating over the string using `utf16_access` or `utf8_access` and want to slice it using those views' iterators? This is also possible since all of the `utf_access` iterators provide the `storage_start()` method that returns a `char_access::iterator` that **points to the start of the Unicode codepoint** in which the current UTF-8/UTF-16 iterator is. 

There is also `storage_next()` which **points to the end of the Unicode codepoint** in which the current UTF-8/UTF-16 iterator is. Finally, there is `storage_last()`, which returns the sentinel at the end of the storage sequence. These can be used to slice the string too (see also the second method below).

```cpp
const sys_string & str = S("🤢abc");
sys_string::utf8_access view(str);
auto first = view.begin();
//The 🤢 character takes 4 UTF-8 bytes
assert(sys_string(first.storage_start(), std::next(first, 1).storage_start()) == S(""));
assert(sys_string(first.storage_start(), std::next(first, 2).storage_start()) == S(""));
assert(sys_string(first.storage_start(), std::next(first, 3).storage_start()) == S(""));
assert(sys_string(first.storage_start(), std::next(first, 4).storage_start()) == S("🤢"));
```

> [!TIP]
> Note that for all the slicing methods you can also use reverse iterators in a similar fashion, if needed.

## Accessing C strings

Ok, all this is great, but what if you *do* need to get a simple C `const char *` out of `sys_string` in a portable fashion? Perhaps you need to call a C API or one that expects `std::string_view` on all platforms. To support this scenario, `sys_string::char_access` exposes the `c_str()` method:

```cpp
const sys_string & str = S("abc");
sys_string::char_access access(str);
const char * cstr = access.c_str();
assert(strcmp(cstr, "abc") == 0);
```

The returned pointer is valid as long as the `char_access` object is alive. The returned string is **always** in UTF-8, **not** in your locale's "narrow" encoding. All calls to `c_str()` on the same `char_access` object return the same pointer - that is, the result is memoized.

Note that if the underlying string storage is not made of `char`s, the first call to `c_str()` will result in memory allocation and conversion of `sys_string` data to UTF-8. If you end up doing the above a lot and it becomes a performance issue, this is a sign that `sys_string` is not the right class for your problem domain and `std::string` might be a better choice.

## Accessing storage as a C array

Sometimes you might want to access the underlying storage units of `sys_string` as a C array. To do so, there are 3 methods that usually need to be called together in portable code. The first one is `data()`. It returns a direct pointer to the underlying storage **if the underlying storage is, in fact, a contiguous array**. Otherwise it returns `nullptr`. In such a case you will need to allocate contiguous storage yourself - the size is given by `storage_size()` - and copy data into it using `copy_data()`. For example:

```cpp

const sys_string & str = S("abc");
const sys_string::size_type size = str.storage_size();
if (const sys_string::storage_type * data = str.data())
{
    use_data(data, size);
}
else
{
    std::vector<sys_string::storage_type> buffer(str.storage_size());
    str.copy_data(0, buffer.data(), str.storage_size());
    use_data(buffer.data(), size);
}
```
Note that the content returned by `data()` may not be null-terminated.

The `copy_data()` method also allows you to copy starting from any index (first argument) and any valid size (third argument).

## Building and modifications

Instances of `sys_string` are immutable. When you do need to construct a `sys_string` from parts, you have 3 ways of doing so:

* Use the `+` operator. This works well for concatenation of existing `sys_string`s, but is not optimal if the items you need to concatenate aren't `sys_string` or `char32_t` and need conversion to `sys_string`. For example, consider
```cpp
std::string str1 = ..., str2 = ...;
sys_string result = sys_string(str1) + sys_string(str2);
```
This might perform 2 allocations for addends to store the conversion results and then add them and discard the temporaries.

* Use the `format()` or `std_format` methods. The first method works like `printf` except it returns the formatted result in a `sys_string`. 
The second does the same using the `std::format` facility (if available) and uses its syntax. This works well for formatting, but it is very slow and inefficient.

* Use the `sys_string_builder` class. This is the most general, performant and flexible way.

To use `sys_string_builder`, you create an instance of it, perform various methods that populate its content and then call the `build()` method to produce a `sys_string`. 

Generally, `sys_string_builder` can be used in two ways. First, it exposes the conventional "builder" idiom where you can do things like:

```cpp
std::string cppstr = ...;
sys_string str = sys_string_builder()
                   .append(U'🟣')
                   .append("abc")
                   .append(u"xyz")
                   .append(cppstr)
                   .build();
```

This is more efficient than concatenating `sys_string`s because conversions happen only once and reallocations follow the amortized cost algorithm pattern used by `std::vector` or `std::string`.

Second, `sys_string_builder` exposes an interface similar to that of `std::vector<char32_t>`. You can do things like `push_back(char32_t)`, `pop_back()`, `insert(iterator, char32_t)`, `erase(iterator)`, `erase(iterator, iterator)` or `clear()`. 

The iterators used in these methods are returned by the regular `begin()`, `end()` etc. methods.

For example, you can do things like:

```cpp
sys_string_builder builder;
builder.insert(builder.end(), U'a');
builder.erase(builder.begin());
std::ranges::copy(U"bc"sv, std::back_inserter(builder));

assert(builder.build() == S("bc"));
```

However, note that `sys_string_builder` isn't really a container of `char32_t`s. Its internal storage unit is the same as that of `sys_string` and is platform-dependent. The interface deliberately forces you to use `char32_t`s to prevent constructing invalid Unicode. 

The iterators do not return references to the actual underlying storage, and you cannot make modifications via iterators' `operator*`. That is, the following won't work
```cpp
sys_string_builder builder;
builder.append("abc");
*builder.begin() = 'q'; //This won't work!
```

Another difference from `std::vector` is that there are no `size()`, `resize()` etc. methods - the internal storage size is not the same as the size in `char32_t`s. Instead, there are `storage_size()`, `resize_storage()` and `reserve_storage()` methods. 

In addition, `sys_string_builder` supports `as_utf8`, `as_utf16` and `as_utf32` adapters, allowing you to iterate over its contents in any of these ways. The iterators exposed by `as_utf32` are exactly the same as `sys_string_builder` iterators.


## Utility methods

### Check if the string is empty
```cpp
auto empty() const noexcept -> bool
```

### Transformations

`sys_string` supports common transformations:

* `to_lower()` which returns the original string converted to lowercase
* `to_upper()` which returns the original string converted to uppercase
* `normalize(normalization)` which normalizes the string to standard NFC or NFD normalization forms.

`to_lower()`/`to_upper()` implement *default Unicode case conversion* (see sections 3.13 and 5.18 of the Unicode standard). This
conversion is non-locale-specific. (E.g. final Greek `Σ` will convert to `ς` but the uppercase of `i` is always `I` regardless of locale).

```cpp
assert(S("βους").to_upper() == S("ΒΟΥΣ"));
assert(S("ΒΟΥΣ").to_lower() == S("βους"));
assert(S("maße").to_upper() == S("MASSE"));
```

For normalization, only NFC and NFD forms are supported, being of practical importance to most applications. If you require other normalization
forms, you are likely to be doing some non-trivial text processing and likely already have a library such as ICU that can help with those.

```cpp
assert(S("\u00C5").normalize(normalization::nfd) == S("\u0041\u030A"));
assert(S("\u0041\u030A").normalize(normalization::nfc) == S("\u00C5"));
```

### Printf-style construction of a string

```cpp
static auto format(const char * spec, ...) -> sys_string;
static auto formatv(const char * spec, va_list vl) -> sys_string;
```

The allowed format syntax is exactly the same as for `printf` on your platform.

### std::format-style construction of a string

```cpp
template<class... Args>
static auto std_format(std::format_string<Args...> fmt, Args &&... args) -> sys_string_t;
static auto std_vformat(std::string_view fmt, std::format_args args) -> sys_string_t;
```

This uses `std::format` format string syntax and provides a modern and type-safe alternative to `format`.
However, these methods are only available if your standard library supports the `std::format` facility.

### Printing into `std::ostream`

```cpp
friend auto operator<<(std::ostream & str, const sys_string & val) -> std::ostream &;
```

Prints the string content **as UTF-8** into `std::ostream`. If the storage type of `sys_string` is `char`, its content is printed verbatim.
Otherwise, it is first converted to UTF-8.

> [!WARNING] 
> If the output is converted to UTF-8 and goes somewhere that doesn't use UTF-8 encoding (Windows console or Unix terminal with non-UTF-8 locale, for example), the output will be garbled.

On Windows or on any platform that defines the `__STDC_ISO_10646__` macro there is also

```cpp
friend auto operator<<(std::wostream & str, const sys_string & val) -> std::wostream &;
```

that behaves similarly for `wchar_t`. 

> [!NOTE]
> The `operator<<` currently completely ignores the width, precision, and fill settings of the stream - the entire string
> is printed out as is. This might change in future versions.

### Formatting with `std::format`

You can use `sys_string` instances in `std::format`. Currently, there are no allowed format flags. As with `operator<<` the output is produced in UTF-8. 

```cpp
std::string str = std::format("Hello {}", S("abc"));
assert(str == "Hello abc");
```


### Trimming
The following methods trim the string on both ends, left or right. By default, space characters are removed. You can pass a predicate functor to change that. 

The default `sysstr::isspace` predicate detects as space characters those having the Unicode property [White_Space](https://en.wikipedia.org/wiki/Whitespace_character#Unicode). 

```cpp
template<std::predicate<char32_t> Pred = sysstr::isspace>
auto trim(Pred pred = Pred()) const -> sys_string;

template<std::predicate<char32_t> Pred = sysstr::isspace>
auto ltrim(Pred pred = Pred()) const -> sys_string;

template<std::predicate<char32_t> Pred = sysstr::isspace>
auto rtrim(Pred pred = Pred()) const -> sys_string;
```

### Digression: sys_string_or_char concept

Many `sys_string_t` methods described below can take either a `sys_string_t` as an argument or a single `char32_t`.
The concept that describes such an argument is named `sys_string_or_char` in this text.

### Splitting
The following methods split the string into a sequence of strings on a given separator:

```cpp
template<sys_string_or_char StringOrChar, std::output_iterator<sys_string> OutIt>
auto split(OutIt dest, const StringOrChar & sep, size_t max_split = std::numeric_limits<size_t>::max()) const -> OutIt;

template<std::invocable<typename sys_string_t::utf32_access::iterator, std::default_sentinel_t> Search, 
         std::output_iterator<sys_string> OutIt>
auto split(OutIt dest, Search pred) const -> OutIt;
```

The `dest` argument is an output iterator to the destination. Each split part will be written into it as `sys_string`.

The second argument can be 
* A `char32_t` or a `sys_string`, in which case it is the separator to split on. The optional 3rd argument specifies the maximum number of splits to make.
* A searcher functor that is given a pair `(begin, end)` of `utf32_access` iterators. It is responsible for locating the first occurrence of a separator in this sequence and returning a pair of iterators (`std::pair`, `std::tuple` or anything that can be destructured) to the beginning and end of the separator. To finish splitting, it must return an empty range. 

The rules for splitting on `char32_t` or a `sys_string` are as follows:
* If the string is empty, the output is a single empty string.
* If the separator is not found, the output is a single string equal to the source.
  * An empty separator is never found.
* At most `max_split` splits are made, so the output contains `max_split + 1` elements. If `max_split` is 0, the output is a single string equal to the source.
* If the source starts with the separator, the output will start with an empty string (e.g. empty "prefix"). Similarly, if the source ends with the separator, the output will end with an empty string (e.g. an empty "suffix"). Thus, if the source is equal to the separator, the output will be 2 empty strings.

### Joining
This is the reverse of splitting. The following method joins a sequence of `sys_strings` using the source.
```cpp
template<std::input_iterator It, std::sentinel_for<It> EndIt>
auto join(It first, EndIt last) const -> sys_string;

template<std::ranges::input_range Range>
auto join(const Range & range) const -> sys_string;
```

The passed iterators/range should produce something that can be appended (via one of the `.append()` methods) to a `sys_string_builder`.

You use it like this
```cpp
std::vector<sys_string> parts = {S("a"), S("b")};

sys_string joined = S(",").join(parts);
assert(joined == S("a,b"));
```

The rules for joining are as follows:
* If the sequence is empty (`first == last`), the output is an empty string.
* If the sequence contains one string, the output is that string.

### Prefix, suffix and infix handling

You can check whether a string starts with/ends with/contains another string or a single `char32_t` using: 

```cpp
template<sys_string_or_char StringOrChar>
auto starts_with(const StringOrChar & prefix) const -> bool;
template<sys_string_or_char StringOrChar>
auto ends_with(const StringOrChar & suffix) const -> bool;
template<sys_string_or_char StringOrChar>
auto contains(const StringOrChar & inner) const -> bool;
```

If you have multiple prefixes/suffixes/infixes to test, you can use the following, which is more efficient than calling the above in a loop:
```cpp
template<std::input_iterator It, std::sentinel_for<It> EndIt>
auto find_prefix(It first, EndIt last) const -> It;
template<std::ranges::input_range Range>
auto find_prefix(Range && range) const -> std::ranges::iterator_t<Range>;

template<std::input_iterator It, std::sentinel_for<It> EndIt>
auto find_suffix(It first, EndIt last) const -> It;
template<std::ranges::input_range Range>
auto find_suffix(Range && range) const -> std::ranges::iterator_t<Range>;

template<std::input_iterator It, std::sentinel_for<It> EndIt>
auto find_contained(It first, EndIt last) const -> It;
template<std::ranges::input_range Range>
auto find_contained(Range && range) const -> std::ranges::iterator_t<Range>;
```
The iterators should return the sequence of `sys_string` or `char32_t` to attempt to match. The return value is the iterator pointing to the first found item or the end of the sequence.

Sometimes, it is convenient to find whether a given substring is present and get the parts before, after or both in one step. The following methods enable this functionality:

```cpp
template<sys_string_or_char StringOrChar>
auto suffix_after_first(const StringOrChar & divider) const -> std::optional<sys_string_t>;
template<sys_string_or_char StringOrChar>
auto prefix_before_first(const StringOrChar & divider) const -> std::optional<sys_string_t>;

template<sys_string_or_char StringOrChar>
auto suffix_after_last(const StringOrChar & divider) const -> std::optional<sys_string_t>;
template<sys_string_or_char StringOrChar>
auto prefix_before_last(const StringOrChar & divider) const -> std::optional<sys_string_t>;

template<sys_string_or_char StringOrChar>
auto partition_at_first(const StringOrChar & divider) const -> std::optional<std::pair<sys_string_t, sys_string_t>>;
template<sys_string_or_char StringOrChar>
auto partition_at_last(const StringOrChar & divider) const -> std::optional<std::pair<sys_string_t, sys_string_t>>;
```

All these methods return an `std::optional` result, which is not set if the divider isn't present. If it is present, the
result contains the looked-for part: suffix, prefix or an `std::pair` of both.

You can find and remove a prefix or suffix in one call using: 
```cpp
template<sys_string_or_char StringOrChar>
auto remove_prefix(const StringOrChar & prefix) const -> sys_string;
template<sys_string_or_char StringOrChar>
auto remove_suffix(const StringOrChar & suffix) const -> sys_string;
```

For all of these methods an empty string argument is always "found".

### Replacing content

You can replace content of the string using:

```cpp
template<sys_string_or_char StringOrChar1, sys_string_or_char StringOrChar2>
auto replace(const StringOrChar1 & old, const StringOrChar2 & new_,
             size_t max_count = std::numeric_limits<size_t>::max()) const -> sys_string;
```

This replaces at most `max_count` occurrences of `old` with the `new_` and returns the resultant string.
The rules for finding `old` are as follows:

* An empty string is never found.
* Search is never performed on replacements. That is, after finding an instance of `old` and replacing it with `new_`, the search continues *after* the replacement, never taking it into account.

Note that, unlike similar methods in other languages, `replace` doesn't currently support regular expressions. 
