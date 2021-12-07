## Apple platform conversions

On Apple platforms (macOS, iOS) the default `sys_string` storage is `CFStringRef` to interoperate with Core Foundation and Cocoa/UIKit APIs.

As a convenience Apple platforms also support "generic Unix" storage which stores `char *` and is meant to interoperate with plain Unix API.
It can be selected via `#define SYS_STRING_USE_GENERIC 1` and is described under [Linux](Linux.md).

With `CFStringRef` storage `sys_string` is trivially convertible from and to `CFStringRef` or `NSString *`. 

```objc
//Converting from/to CFStringRef
CFStringRef cfstr_in = CFSTR("abc");
sys_string str1(cfstr_in);
CFStringRef cfstr_out = str1.cf_str();
assert(cfstr_in == cfstr_out);

//Converting from/to NSString *
NSString * nsstr_in = @"abc";
sys_string str2(nsstr_in);
NSString * nsstr_out = str2.ns_str();
assert(nsstr_in == nsstr_out);

//nullptr
assert(sys_string((NSString *)nullptr) == sys_string());
assert(sys_string().cf_str() == nullptr);
assert(sys_string().ns_str() == nullptr);
```

Note the **null preservation** above. A default constructed `sys_string` or `sys_string` constructed from a `null` system string type produces `null` system string pointer back. This is by design to allow round-tripping of `null`s between C++ and ObjectiveC without information loss.

