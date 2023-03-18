## Python conversions

When compiled with `SYS_STRING_USE_PYTHON=1` the default storage type for `sys_string` becomes Python string (e.g. `PyObject *`). You can also compile with just 
`SYS_STRING_ENABLE_PYTHON=1` in which case Python strings become available via `sys_string_pystr` and `sys_string_builder_pystr` but not default.

With `PyObject *` storage `sys_string` is trivially convertible from and to a Python API string.

```cpp
//Converting to/from PyObject *
auto raw = PyUnicode_FromString("abc");
sys_string str(raw); //this increments reference. You still owe raw
assert(raw == S("abc"));

auto raw1 = str.py_str(); //returns borrowed reference!
assert(raw1 == raw);
assert(PyUnicode_AsUTF8(raw1), "abc"));

Py_DECREF(raw); //you owe raw but not raw1

```

Not that unlike other platforms `PyObject *` passed to `sys_string` cannot be `null` and `py_str()` will never return `null`. 
This is in keeping with normal Python API semantics where `null` pointers are never valid and signify errors.
