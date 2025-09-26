## Emscripten platform conversions

When compiling under Emscripten there are two storage types available. The default one is optimized for interoperability with JavaScript. It stores a sequence of `char16_t` which can be converted to `String` with the least amount of overhead. 

Additionally you can chose a "generic Unix" storage which stores `char *` and is meant to interoperate with plain Unix API. It can be selected via `#define SYS_STRING_USE_GENERIC 1` and is described under [Linux](Linux.md).

With JavaScript-optimized storage a conversion to and from `String` is non-trivial. It incurs allocation on JavaScript or native heap and copying between them.

There are two ways you can convert strings from/to JavaScript

### Using external references

Newer versions of Emscripten (actually clang that it is using) support [WASM 2.0 external references](https://github.com/WebAssembly/reference-types) via
`__externref_t` type. If this feature is available (the library detects it automatically) you can convert a JavaScript `String`
object to `sys_string` and create a `String` object from `sys_string` as follows:

```cpp
//Conversions from/to JavaScript
__externref_t jstr_in = ...;
sys_string str(jstr);

...

__externref_t jstr_out = str.make_js_string_ref();
```

Note that `make_js_string_ref` creates a new JavaScript string every time (the `__externref_t` handles have severe limitations on their use in C++ code
and cannot be easily cached for example). Also unlike other platforms you **cannot pass `null` (or `undefined`)** as strings to C++ side. Neither will `make_js_string_ref()` ever return a handle that will convert to `null` or `undefined` on JavaScript side.

Using external references is the fastest conversion method available. It can be made even faster if your WASM platform supports [WASM JavaScript Builtins](https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/JavaScript_builtins). In such case you can define `SYS_STRING_USE_WASM_JS_STRING` macro to 1 before including any `sys_string` headers. This will cause conversions to use built-ins which can result in significant perf improvements for JavaScript -> C++ string conversion. A few things to keep in mind:
1. This WASM feature is very new and might not be supported by your version of Node or browser.
2. The opposite direction, C++ -> JavaScript is currently unaffected. (The way built-ins are specified makes them useless for that scenario)
3. You will need to pass `builtins: ["js-string"]` as compile options when loading the resultant WASM code as described by the link above
4. You overall WASM code now will have an external dependency. You will likely need to pass flags such as:
    ```
    -sMAIN_MODULE=2 -sERROR_ON_UNDEFINED_SYMBOLS=0 -sWARN_ON_UNDEFINED_SYMBOLS=0
    ```
   to your build resulting in other trade-offs.

All in all, the WASM built-ins should be considered highly experimental at this point.

### Using `embind` facilities

In this, older, but reliably supported, method conversions rely on on `embind` [library](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html) so you will need to link with that (e.g. `-lembind`).


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



