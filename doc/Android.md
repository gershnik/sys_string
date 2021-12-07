## Windows platform conversions

Currently there is only one storage type available on Android.

`sys_string` internally stores a sequence of `char16_t` which can be converted to `jstring` with the least amount of JNI overhead. A conversion is not-trivial, however. It incurs allocation and copying. (A possible approach to store global references to `jstring` in `sys_string` is not feasible for many reasons, among them the fact that global reference table is of limited size).
As expected with JNI, all conversion require JNIEnv * argument.

```cpp
JNIEnv * env = ...;

//Conversions from/to jstring
jstring jstr_in = env->NewString((const jchar *)u"abc", std::size(u"abc") - 1);
sys_string str(env, jstr_in);
assert(str == S("abc"));
jstring jstr_out = str.make_jstring(env);
assert(jstr_in != jstr_out); //in and out are NOT the same!

//nullptr
assert(sys_string().make_jstring(env) == nullptr);
assert(sys_string(env, nullptr) == sys_string());
```

Note the **null preservation** above.

