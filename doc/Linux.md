## Linux platform conversions

The only storage type available on Linux is "generic Unix". It is meant to interoperate with Posix-style APIs that deal with `char *`.
`sys_string` logically stores its content as a sequence of `char`s in UTF-8 encoding. Conversions **from** `const char *` always incur copying and sometimes memory allocation (`sys_string` does small string optimization similar to `std::string`).
Conversion **to** `const char *` are 0-cost.

```cpp
const char * cstr_in = "abc";
sys_string str(cstr_in);
const char * cstr_out = str.c_str();
assert(strcmp(cstr_out, cstr_in) == 0);
assert(cstr_out != cstr_in); //in and out are NOT the same!

//nullptr
assert(sys_string().c_str() != nullptr); //NO null preservation
assert(strcmp(sys_string().c_str(), "") == 0);
assert(sys_string((const char *)nullptr).c_str() != nullptr); //NO null preservation
assert(strcmp(sys_string((const char *)nullptr).c_str(), "") == 0);
```

Note that unlike other storage types there is no null preservation here. `c_str()` returns an empty C string for default constructed `sys_string` or one constructed from `nullptr`. This is deliberate to align with `std::string` behavior that never produces `nullptr` from its `c_str()`.

