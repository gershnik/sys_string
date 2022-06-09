## Windows platform conversions

On Windows there isn't a single, universally used system string type. Win32 API uses plain `wchar_t *`, traditional OLE/COM relies on `BSTR`, while newer C++/WinRT uses `HSTRING`. To target your specific scenario `sys_string` can be configured at compile time to use either of these types of storage.
If no configuration flags are specified the default storage is plain `wchar_t *`. 

Additionally, some programs rely on Unix compatibility APIs exclusively and, for those, Windows platform also supports "generic Unix" storage which stores `char *` and is meant to interoperate with plain Unix API. 
It can be selected via `#define SYS_STRING_USE_GENERIC 1` and is described under [Linux](Linux.md).

Being the lowest-common denominator, zero-cost conversions to `const wchar_t *` are available in all configurations.

Conversions supported by each storage type are described below.

### Plain `wchar_t *` storage

With this, default, storage `sys_string` stores an array of `wchar_t` and can be trivially converted only to and from `const wchar_t *`. 

```cpp
const wchar_t * cstr_in = L"abc";
sys_string str(cstr_in);
const char * cstr_out = str.w_str();
assert(wcscmp(cstr_out, cstr_in) == 0);
assert(cstr_out != cstr_in); //in and out are NOT the same!

//nullptr
assert(sys_string().w_str() != nullptr); //NO null preservation
assert(wcscmp(sys_string().w_str(), "") == 0);
assert(sys_string((const wchar_t *)nullptr).w_str() != nullptr); //NO null preservation
assert(wcscmp(sys_string((const wchar_t *)nullptr).c_str(), "") == 0);

```

### `BSTR` storage

With this storage `sys_string` stores a `BSTR` and can be trivially converted to and from it as well as `const wchar_t *`. 

To make this storage default you need to `#define SYS_STRING_WIN_BSTR 1`.

Since `BSTR` is not a reference counted type a distinction is made between *copying* a `BSTR` in and out of `sys_string` and attaching/releasing one.

#### Attaching and releasing

```cpp
BSTR bstr = SysAllocString(L"abc");
sys_string attached(bstr, attach_pointer);

assert(attached == S("abc"));
assert(attached.b_str() == bstr);
assert(wcscmp(attached.w_str(), L"abc") == 0);

BSTR bstr1 = SysAllocString(L"xyz");
attached = sys_string(bstr1, attach_pointer);
BSTR released = attached.release();
assert(released == bstr1);
assert(attached == S(""));
SysFreeString(released);

BSTR null_bstr = nullptr;
attached = sys_string(null_bstr, attach_pointer);
assert(attached.b_str() == nullptr); //NULL is preserved
assert(attached.w_str() != nullptr); //But w_str() is an empty string, not null
assert(wcscmp(attached.w_str(), L"") == 0);

```

Note the **null preservation** of `BSTR` but always non-null `w_str()` above.


#### Copying

```cpp

BSTR bstr = SysAllocString(L"abc");
sys_string copied(bstr, copy_content);
assert(copied == S("abc"));
assert(copied.b_str() != bstr); //stored BSTR is NOT the same
assert(wcscmp(copied.w_str(), L"abc") == 0);
SysFreeString(bstr); //you own the original

```
  

### `HSTRING` storage

With this storage `sys_string` stores `HSTRING` and can be trivially converted to it. Conversions **from** `HSTRING` are usually trivial but can incur memory allocation and copy if the original `HSTRING` is a [fast-pass](https://devblogs.microsoft.com/oldnewthing/20160615-00/?p=93675) string. 

Conversions to `const wchar_t *` are also trivial. Conversion *from* `const wchar_t *` always incurs allocation and copy to create backing `HSTRING`.

To make this storage default you need to `#define SYS_STRING_WIN_HSTRING 1`.


```cpp
//Conversions from/to HSTRING
HSTRING hstr_in;
WindowsCreateString(L"abc", UINT32(std::size(L"abc") - 1), &hstr_in);
sys_string str1(hstr_in);
HSTRING hstr_out = str1.h_str();
assert(hstr_in == hstr_out);

//Conversions from/to wchar_t *
//this works on Windows only!
const wchar_t * wstr_in = L"abc";
sys_string str2(wstr_in);
const wchar_t * wstr_out = str2.w_str();
assert(wstr_in != wstr_out); //in and out are NOT the same!

//nullptr
assert(sys_string().h_str() == nullptr);
assert(wcscmp(sys_string().w_str(), L"") == 0);
assert(sys_string(HSTRING(nullptr)) == sys_string());
assert(sys_string(HSTRING(nullptr)) == S(""));
```

Note the **null preservation** of `HSTRING` but always non-null `w_str()` above.

