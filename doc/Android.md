## Android platform conversions

On Android, there are two storage types available. The default one is optimized for interoperability with Java. It stores a sequence of `char16_t` that can be converted to `jstring` with the least amount of JNI overhead. 

Additionally, you can choose a "generic Unix" storage that stores `char *` and is meant to interoperate with the plain Unix API. It can be selected via `#define SYS_STRING_USE_GENERIC 1` and is described under [Linux](Linux.md).

With Java-optimized storage, a conversion to and from `jstring` is non-trivial. It incurs allocation on the Java or native heap and copying between them. 
(A possible approach to store global references to `jstring` in `sys_string` is not feasible for many reasons, among them the fact that the global reference table can be of limited size.)
As expected with JNI, all conversions require a `JNIEnv *` argument.

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

