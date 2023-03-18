## Emscripten platform conversions

When compiling under Emscripten there are two storage types available. The default one is optimized for interoperability with JavaScript. It stores a sequence of `char16_t` which can be converted to `String` with the least amount of overhead. 

Additionally you can chose a "generic Unix" storage which stores `char *` and is meant to interoperate with plain Unix API. It can be selected via `#define SYS_STRING_USE_GENERIC 1` and is described under [Linux](Linux.md).

With JavaScript-optimized storage a conversion to and from `String` is non-trivial. It incurs allocation on JavaScript or native heap and copying between them. 

Conversions rely on on `embind` [library](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html) so you will need to link with that (e.g. `-lembind`).


```cpp
//Conversions from/to JavaScript
EM_VAL handle_in = ... //passed from JavaScript, see below
sys_string str(handle_in);
assert(str == S("abc"));

...

EM_VAL handle_out = str.make_js_string();
assert(handle_in != handle_out); //in and out are NOT the same!
//Return handle_out to JavaScript, see below
```

Passing strings from JavaScript can be accomplished as follows

```javascript
let handle_in = Emval.toHandle("abc");
try {
    nativeFunctionWithStringArg(handle_in);
} finally {
    __emval_decref(handle_in);
}
```

And receiving strings from native code as follows

```javascript
let handle_out = nativeFunctionReturningString();
try {
    let str = Emval.toValue(handle_out);
} finally {
    __emval_decref(handle_out);
}
```

Note that unlike other platforms you **cannot pass `null` (or `undefined`)** as strings to C++ side. Neither will `make_js_string()` ever return a handle that will convert to `null` or `undefined` on JavaScript side.



