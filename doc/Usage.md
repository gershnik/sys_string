
<!-- TOC -->

- [Header](#header)
- [Namespace](#namespace)
- [Error handling](#error-handling)
- [Basics](#basics)
- [Construction](#construction)
    - [Conversions from invalid Unicode](#conversions-from-invalid-unicode)
- [Platform-specific conversions](#platform-specific-conversions)
- [Adding Strings](#adding-strings)
- [Comparing Strings](#comparing-strings)
    - [Case insensitive comparison](#case-insensitive-comparison)
- [Iterating over string content](#iterating-over-string-content)
    - [Storage iteration](#storage-iteration)
    - [UTF iteration](#utf-iteration)
- [Substrings](#substrings)
- [Accessing C strings](#accessing-c-strings)
- [Accessing storage as C array](#accessing-storage-as-c-array)
- [Building and modifications](#building-and-modifications)
- [Utility methods](#utility-methods)
    - [Check if the string is empty](#check-if-the-string-is-empty)
    - [Printf-style construction of a string](#printf-style-construction-of-a-string)
    - [Printing into std::ostream](#printing-into-stdostream)
    - [Trimming](#trimming)
    - [Splitting](#splitting)
    - [Joining](#joining)
    - [Prefix, suffix and infix handling](#prefix-suffix-and-infix-handling)
    - [Replacing content](#replacing-content)

<!-- /TOC -->

## Header

Everything needed to use `sys_string` and all related functionality is declared in `sys_string/sys_string.h` header. 

An `#include <sys_string/sys_string.h>` directive is assumed in all the examples.

## Namespace

Everything in this library is under namespace `sysstr`. 

A `using namespace sysstr` directive is assumed in all the examples.

## Error handling

Most methods in this library are `noexcept`. Methods that aren't `noeexcept` involve memory allocation or creation of system string types that can fail (usually also due to memory allocation failure). In such cases errors are reported via exceptions. All thrown exceptions derive from `std::exception`. When it is certain that the failure is due to memory allocation `std::bad_alloc` is raised. Otherwise, the library throws a more generic `std::runtime_error`


## Basics

The two main classes provided by this library are `sys_string` and `sys_string_builder`. These are actually aliases (typedefs) to instantiations
of `template<class Storage> class sys_string_t` and `template<class Storage> class sys_string_builder_t` with a `Storage` chosen based on platform
and compilation options. A `Storage` is a policy class that defines what kind of system string type is stored inside a `sys_string`. 
On every supported platform there is 1 or more storage types available. 


## Construction

In the simplest case a `sys_string` literal can be created statically, that is, having its content embedded into executable image.

```cpp
const sys_string & str = S("Hello!");
```
`S` unfortunately has to be a macro. If such a short macro name offends you or collides with another macro in your program you can disable it via `#define SYS_STRING_NO_S_MACRO 1`. In this case you can use verbose:
```cpp
const sys_string & str = SYS_STRING_STATIC("Hello!");
``` 

You can also create a string by converting it from a single character, C string, C string + length, `std::basic_tring`, `std::basic_string_view`, `std::vector` or `std::range` of characters. 
The type of character can be one of:
* `char`, `signed char`, `unsigned char` (always assumed to be UTF-8)
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

A `sys_string` internally stores data in one of: UTF-8 (`char`), UTF-16 (`char16_t`) or UTF-32 (`char32_t`) encodings, depending on configuration. The specific type used is represented by `sys_string::storage_type` typedef. What happens when you create a `sys_string` from external data that contains invalid Unicode? It depends on whether the external data is in the same encoding as the storage type.
* If the type is the same then the data is copied into `sys_string` verbatim. This allows you to store and manipulate invalid Unicode in platform specific way. For example Windows filenames, while ostensibly in UTF-16, can contain unpaired surrogates. `sys_string` allows you to handle these but you need to make sure you pass them in as the OS gives them - in `wchar_t *` or `char16_t *`
* If the type is not the same then the result is **unspecified but valid** Unicode. In practice invalid content will be replaced by one or more "Unicode replacement character" - `U+FFFD` - similar to how text editor or a browser would deal with it. The exact number of characters and the replacement algorithm in general are deliberately left unspecified. Thus portable code (that is code that is agnostic to the type of `sys_string` storage) cannot rely on invalid Unicode having specific representation - this is by design.

## Platform-specific conversions 

In addition to portable conversions described above, `sys_string` supports platform-specific conversions to and from various system string types.
Those are described on the following pages.

* [Apple](Apple.md)
* [Windows](Windows.md)
* [Android](Android.md)
* [Linux](Linux.md)

## Adding Strings

You can combine strings using `+` operator. 
```cpp
sys_string str1 = S("a");
sys_string str2 = S("b");
sys_string str3 = S("c");
sys_string str = str1 + str2 + str3;
assert(str == S("abc"));
```

Unlike with `std::string` this uses [expression templates](https://en.wikipedia.org/wiki/Expression_templates) to avoid creation of temporaries. This means that addition is cheap and you have no reason to avoid it. 

However, you need to avoid using `auto` for the result. The result of addition is a special temporary that only performs actual concatenation when converted to `sys_string`. Using auto makes a variable of that temporary type which at beast won't work and at worst result in dangling pointers.

```cpp
auto res = S("a") + S("b");
std::cout << res; //error, res is not something that can be output 
auto res1 = sys_string("abc") + sys_string("xyz");
//using res1 here is undefined behavior - it refers to already destroyed temporaries
```

## Comparing Strings

`sys_string` supports the normal inventory of comparison operators: `==`, `!=`, `<=>` (under C++20), `<`, `<=` etc. Comparison strings is always performed as binary comparison of internal representations, **not** Unicode codepoints and **not** normalizing Unicode. That is:

```cpp
//these strings are canonically equivalent but not the same
assert(S("Cafe\u0301") != S("Café"));
```

The comparisons are defined this way for performance. If you do need canonical equivalence you will need to normalize strings yourself.
What this implies is that while identical strings will compare equal and all strings are strongly ordered, the specific order might differ from the one a human would expect. If you do require human-oriented ordering, the correct thing to do is to use a full-fledged collator from another library such as ICU or Windows NLS.

There is also a free function `compare` that behaves identically to `<=>` operator but is available on both C++17 and C++20.

### Case insensitive comparison

Unlike `std::string`, `sys_string` also supports case insensitive comparisons. 
This is done by binary comparing result of Unicode case folding. 

```cpp
assert(compare_no_case(S("maße"), S("MASSE")) == sys_string::ordering_equal);
```

Both `compare` and `compare_no_case` return `sys_string::ordering_equal`, `sys_string::ordering_less` and `sys_string::ordering_greater`.
These are defined in terms of either `std::strong_ordering::equal` etc. on C++20 or `0`, `-1`, `1` on C++17.

## Iterating over string content

You can iterate over `sys_string` in 4 possible ways:
* Over a sequence of UTF-32, `char32_t` codepoints. This iteration is unidirectional, forward or reverse and 'sanitizing'.
* Over a sequence of UTF-16, `char16_t` units. This iteration is unidirectional, forward or reverse and 'sanitizing'.
* Over a sequence of UTF-8, `char` units. This iteration is unidirectional, forward or reverse and 'sanitizing'.
* Over a sequence of "storage units". This iteration is random-access and NOT sanitizing.

A "storage unit" is a platform dependent type (one of `char`, `char16_t` or `char32_t`) that is used to actually store the data in `sys_string`. It is represented by type `sys_string::storage_type`. Iterating over storage units is the fastest way to iterate and the only way to get access to "invalid Unicode" that might be stored in `sys_string`. 

All other types of iteration are "sanitizing". It is possible for a string to contain invalid Unicode - see (#Conversions_from_invalid_Unicode). When this happens a 'sanitizing' iteration will replace invalid units with one or more replacement characters `U+FFFD` in an unspecified manner. Note that it is not even guaranteed that forward and reverse iteration will produce the same replacements. Thus, when dealing with invalid Unicode, all you can expect is that sanitizing iteration will always produce valid Unicode but you cannot specifically guarantee what it would be. If you do need to predictably handle invalid Unicode content you need to iterate over storage units.


### Storage iteration

To iterate over string storage units you first need to create a `sys_string::char_access` object. This object is cheap to create and acts as a "reference" to the actual `sys_string`. It is **only valid while the original string exists**. It is meant to be used on stack, during computation and then discarded rather than stashed away for subsequent use.

It is a read-only STL container with the usual methods: `begin()`, `end()`, `rbegin()` etc. Those produce random access iterators which are the fastest way to go over the `sys_string` content.
The size of the sequence is given by `size()` member. There is also `operator[]` that allows direct indexing access to the sequence.

```cpp
sys_string str("Hello");
for(sys_string::storage_type c: sys_string::char_access(str)) {
    ...
}
sys_string::char_access access(str);
std::cout << "storage size is " << access.size() << '\n';
std::cout << "unit at [0] is " << access[0] << '\n';
```

On some platforms `sys_string::char_access` has a `data()` member that returns a pointer to a contiguous array of storage units. Combination of `data()` and `size()` allow fast interop with platform specific code.

### UTF iteration

To perform UTF iteration you need to construct one of: `utf32_view`, `utf16_view` or `utf8_view` from `sys_string`. These are read-only STL sequences that expose forward (not random or bidirectional!) iterators. Note that despite the iterators being forward, reverse iterators that move backwards in the sequence are available via `rbegin`/`rend`.

Similar to `sys_string::char_access` (which they actually store inside) or `std::string_view` these views are reference types. Their instances are **only valid as long as the original string exists**.

The reason the iterators are forward-only even though you can iterate backwards via reverse iterators is due ti the 'sanitizing' iteration. Code using bidirectional iterators usually assumes that

```cpp
iterator it1 = ...;
iterator it2 = it1;
it2 = ++(--it2);
assert(it2 == it1);
```

However, with sanitizing iteration this might not necessarily be the case. If a substitution is made for illegal characters the amount of movement and returned values may differ for forward and reverse iteration. Thus to avoid subtle issues UTF iterators disallow `--`. If you want to iterate in reverse simply use `reverse_iterator`s returned by `rbegin()`/`rend()`.

```cpp
sys_string str("Hello");
for(char32_t c: utf32_view(str)) {
    ...
}
for(char16_t c: utf16_view(str)) {
    ...
}
for(char c: utf8_view(str)) {
    ...
}
```
The three UTF views are not limited to operating on `sys_string`. You can also use them to iterate on any random-access ranges of characters including `std::vector`, `std::string` or arrays. If you only require this functionality and not full `sys_string` you can include `sys_string/utf_view.h` header.

```cpp
std::u16_string u16str = u"Hello";
for(char c: utf8_view(u16str)) {
    //UTF-8 iteration over UTF-16 encoded string
}
std::string str = "Hello";
for(char c: utf32_view(str)) {
    //UTF-32 iteration over UTF-8 encoded string
}
//etc. all 9 combinations are possible
```

## Substrings

You can obtain a substring of a `sys_string` in two ways:

* From a pair of `sys_string::utf32_view::iterator`s. This will "cut" the string at Unicode codepoint boundary ensuring that the output is well formed. This is the preferred way to slice strings.

```cpp
const sys_string & str = S("🤢abc");
sys_string::utf32_view view(str);
auto first = view.begin();
auto last = std::advance(first, 1);
sys_string substring(first, last);
assert(substring == S("🤢"));
```

What if you are iterating over the string using `utf16_view` or `utf8_view` and want to slice it using those views' iterators? This is also possible since all of the `utf_view` iterators provide `char_start()` method that returns a `utf32_view::iterator` that **points to the start of the Unicode codepoint** in which the current iterator is in. 

```cpp
const sys_string & str = S("🤢abc");
sys_string::utf8_view view(str);
auto first = view.begin();
//The 🤢 character takes 4 UTF-8 bytes
assert(sys_string(first.char_start(), std::advance(first, 1).char_start()) == S(""));
assert(sys_string(first.char_start(), std::advance(first, 2).char_start()) == S(""));
assert(sys_string(first.char_start(), std::advance(first, 3).char_start()) == S(""));
assert(sys_string(first.char_start(), std::advance(first, 4).char_start()) == S("🤢"));
```


* From a pair of `sys_string::char_access::iterator`s. Since `char_access` iterates over the actual storage units this allows you to "cut" the string anywhere. This way of slicing is **dangerous**, since you can end up slicing the string in a middle of a Unicode character. Unless this is exactly what you want to do for some reason, this would likely result in bugs in your code. Remember that sanitizing UTF iteration over such string will produce substitution characters.

```cpp
const sys_string & str1 = S("abc");
sys_string::char_access access1(str1);
sys_string substring1(access1.begin(), access1.begin() + 1);
//This works because 'a' is a single character in any UTF encoding
assert(substring1 == S("a")); 

const sys_string & str2 = S("🤢bc");
sys_string::char_access access2(str2);
sys_string substring2(access2.begin(), access2.begin() + 1);
//Unless the interal storage is UTF-32 (which is currently not the case on any platform) the
//substring will NOT be equal to the first character
assert(substring2 != S("🤢")); 
```

## Accessing C strings

Ok, all this is great but what if you *do* need to get simple C `const char *` out of `sys_string` in a portable fashion? Perhaps you need to call a C API or one that expects `std::string_view` on all platforms. To support this scenario `sys_string::char_access` exposes `c_str()` method:

```cpp
const sys_string & str = S("abc");
sys_string::char_access access(str);
const char * cstr = access.c_str();
assert(strcmp(cstr, "abc") == 0);
```

The returned pointer is valid as long as `char_access` object is alive. The returned string is **always** in UTF-8, **not** in your locale's "narrow" encoding. All calls to `c_str()` on the same `char_access` object return the same pointer - that is the result is memoized.

Note that (except on Linux) the first call to `c_str()` will result in memory allocation and conversion of `sys_string` data to UTF-8. If you end up doing the above a lot and it becomes a performance issue this is a sign that `sys_string` is not a right class for your problem domain and `std::string` might be a better choice.

## Accessing storage as C array

Sometimes you might want to access underlying storage units of `sys_string` as a C array. To do so there are 3 methods that are usually need to be called together in portable code. The first one is `data()`. It returns a direct pointer to underlying storage **if the underlying storage is, in fact, a contiguous array**. Otherwise it returns `nullptr`. In such case you will need to allocate contiguous storage yourself - the size is given by `storage_size()` - and copy data into it using `copy_data()`. For example:

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
Note that the content returned by `data()` is generally not null-terminated.
The `copy_data()` method is more general allowing you to copy starting from any index (first argument) and any valid size (third argument).

## Building and modifications

Instances of `sys_string` are immutable. When you do need to construct a `sys_string` from parts you have 3 ways of doing so

* Use `+` operator. This work well for concatenation of existing `sys_string`s but is not optimal if the items you need to concatenate aren't `sys_string` or `char32_t` and need conversion to `sys_string`. For example consider
```cpp
std::string str1 = ..., str2 = ...;
sys_string result = sys_string(str1) + sys_string(str2)
```
This might perform 2 allocation for addends to store the conversion results and then add them and discard the temporaries.

* Use `format()` method. This method works like `printf` except it returns the formatted result in a `sys_string`. This works well for formatting but it is very slow and inefficient.

* Use `sys_string_builder` class. This is the most general, performant and flexible way.

To use `sys_string_builder` you create an instance of it, perform various methods that populate its content and then call the `build()` method to produce a `sys_string`. 
The `build()` method is `noexcept` and is very low or 0 cost - there are no allocations happening in it. Everything that needed to be allocated is already done by the time you call it.

Generally, `sys_string_builder` can be used in 2 ways. First it exposes the conventional "builder" idiom where you can do things like:

```cpp
std::string cppstr = ...;
sys_string str = sys_string_builder()
                   .append(U'🟣')
                   .append("abc")
                   .append(u"xyz")
                   .append(cppstr)
                   .build();
```

This is more efficient that concatenating `sys_string`s because conversions happen only once and re-allocations follow the same amortized pattern used by `std::vector` or `std::string`.

Second, `sys_string_builder` exposes interface similar to the one of `std::vector<char32_t>`. You can do things like `push_back(char32_t)`, `pop_back()`, `insert(iterator, char32_t)`, `erase(iterator)`, `erase(iterator, iterator)` or `clear()`. 

The iterators used in these methods are returned by the regular `begin()`, `end()` etc. methods. 

However, note that `sys_string_builder` isn't really a container of `char32_t`s. Its internal storage unit is the same as `sys_string` and is platform dependent. The interface deliberately forces you to use `char32_t`s to prevent constructing invalid Unicode. 

The iterators are bi-directional rather than random-access ones and they return non-modifiable values rather than references.

Thus, because the iterators do not return references to the actual underlying storage, you cannot make modifications via iterators' `operator*`. That is the following won't work
```cpp
sys_string_builder builder;
builder.append("abc");
*builder.begin() = 'q'; //This won't work!
```

Another difference from `std::vector` is that there is no `size()`, `resize()` etc. methods - the internal storage size is not the same as size in `char32_t`s. Instead there are `storage_size()`, `resize_storage()` and `reserve_storage()` methods. 

Finally, just like `sys_string` class, `sys_string_builder` exposes `utf8_view`, `utf16_view` and `utf32_view` types allowing you to iterate over its contents in any of these ways. The iterators exposed by `utf32_view` are exactly the same as `sys_string_builder` iterators.


## Utility methods

### Check if the string is empty
```cpp
auto empty() const noexcept -> bool
```

### Printf-style construction of a string
```cpp
static auto format(const char * spec, ...) -> sys_string;
static auto formatv(const char * spec, va_list vl) -> sys_string;
```

### Printing into `std::ostream`

```cpp
friend auto operator<<(std::ostream & str, const sys_string & val) -> std::ostream &;
```

Prints the string content **as UTF-8** into `std::ostream`. Note that if your output goes somewhere that doesn't use UTF-8 encoding (Windows console or Unix terminal with non UTF-8 locale, for example) the output will not be garbled.

On Windows and any platform that defines `__STDC_ISO_10646__` macro there is also

```cpp
friend auto operator<<(std::wostream & str, const sys_string & val) -> std::wostream &;
```

That prints out UTF-16 or UTF-32 views of the string.


### Trimming
The following methods trim the string on both ends, left or right. By default space characters are removed. You can pass a predicate functor to change that. The space characters are those having Unicode property [White_Space](https://en.wikipedia.org/wiki/Whitespace_character#Unicode). 
```cpp
template<class Pred = sysstr::isspace>
auto trim(Pred pred = Pred()) const -> sys_string;
template<class Pred = sysstr::isspace>
auto ltrim(Pred pred = Pred()) const -> sys_string;
template<class Pred = sysstr::isspace>
auto rtrim(Pred pred = Pred()) const -> sys_string;
```

### Splitting
The following methods split the string into a sequence of strings on a given separator
```cpp
template<class StringOrChar, class OutIt>
auto split(OutIt dest, const StringOrChar & sep, size_t max_split = std::numeric_limits<size_t>::max()) const -> OutIt;
template<class Search, class OutIt>
auto split(OutIt dest, Search pred) const -> OutIt;
```
The `dest` argument is an output iterator to the destination. Each split part will be written into it as `sys_string`.
The second argument can be 
* A `char32_t` or a `sys_string` in which case it is the separator to split on. In this case the optional 3rd argument specifies maximum number of splits to make.
* A searcher functor that is given a pair `(begin ,end)` of `utf32_view` iterators. It is responsible to locate the first occurrence of a separator in this sequence and return a pair of iterators (`std::pair`, `std::tuple` or anything that can be destructured) to beginning and end of the separator. To finish splitting it must return an empty range. 

The rules for splitting are as follows:
* If the string is empty the output is a single empty string
* If the separator is not found the output is a single string equal to source
  * An empty separator is never found.
* At most `max_split` splits are made so the output contains `max_split + 1` elements. If `max_split` is 0 the output is a single string equal to source.
* If the source starts with separator the output will start with an empty string (e.g. empty "prefix"). Similarly, if the source ends with separator the output will end with an empty string (e.g. empty "suffix"). Thus, if the source is equal to separator the output will be 2 empty strings.

### Joining
This is the reverse of splitting. The following method joins a sequence of `sys_strings` using the source
```cpp
template<class FwdIt>
auto join(FwdIt first, FwdIt last) const -> sys_string;
```  

You use it like this
```cpp
std::vector<sys_string> parts = {S("a"), S("b")};

sys_string joined = S(',').join(parts.begin(), parts.end());
assert(joined == S("a,b"));
```

The rules for joining are as follows
* If the sequence is empty (`first == last`), the output is an empty string.
* If the sequence contains one string, the output is that string.

### Prefix, suffix and infix handling

You can check whether a string starts with/ends with/contains another string or a single `char32_t` using 

```cpp
template<class StringOrChar>
auto starts_with(const StringOrChar & prefix) const -> bool;
template<class StringOrChar>
auto ends_with(const StringOrChar & suffix) const -> bool;
template<class StringOrChar>
auto contains(const StringOrChar & inner) const -> bool;
```

If you have multiple prefixes/suffixes/infixes to test you can use the following which is more efficient than calling the above in a loop
```cpp
template<class InIt>
auto find_prefix(InIt first, InIt last) const -> InIt;
template<class InIt>
auto find_suffix(InIt first, InIt last) const -> InIt;
template<class InIt>
auto find_contained(InIt first, InIt last) const -> InIt;
```
The iterators should return the sequence of `sys_string` or `char32_t` to attempt to match. The return value is the iterator to the first found item or `last`.

You can find and remove prefix or suffix in one call using 
```cpp
template<class StringOrChar>
auto remove_prefix(const StringOrChar & prefix) const -> sys_string;
template<class StringOrChar>
auto remove_suffix(const StringOrChar & prefix) const -> sys_string;
```

For all of these method an empty string is always "found".

### Replacing content

You can replace content of the string using

```cpp
template<class StringOrChar1, class StringOrChar2>
auto replace(const StringOrChar1 & old, const StringOrChar2 & new_,
             size_t max_count = std::numeric_limits<size_t>::max()) const -> sys_string>;
```

This replaces at most `max_count` of occurrences of `old` with the `new_` and returns the resultant string.
The rules for finding `old` are as follows

* An empty string is never found.
* Search is never done in replacement. That is after finding an instance of `old` and replacing i with `new_` the search continues *after* the replacement never taking it into account.

Note that unlike similar method in other languages `replace` doesn't currently support regular expressions. 












