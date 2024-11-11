//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INSIDE
#error This header must not be included directly. Please include sys_string.h
#endif


#include <sys_string/impl/util/generic_buffer.h>

namespace sysstr::util
{
    struct android_traits
    {
        using storage_type = char16_t;
        using size_type = jsize;
        using hash_type = unsigned;
        using native_handle_type = jstring;
        
        static constexpr size_type max_size = std::numeric_limits<jsize>::max() / sizeof(char16_t);
    };

    template<size_t N>
    using static_buffer     = generic::static_buffer<android_traits::storage_type, android_traits::size_type, N>;
    using dynamic_buffer    = generic::dynamic_buffer<android_traits::storage_type, android_traits::size_type>;

    using buffer            = generic::buffer<android_traits::storage_type, android_traits::size_type>;

    using builder_impl      = generic::buffer_builder<android_traits::storage_type, android_traits::size_type>;

    using char_access       = generic::char_access<android_traits::storage_type, android_traits::size_type>;

}

namespace sysstr
{
    class android_storage : private util::generic::storage<util::android_traits::storage_type, util::android_traits::size_type>
    {
    friend util::char_access;
    private:
        using super = util::generic::storage<util::android_traits::storage_type, util::android_traits::size_type>;
    public:
        using super::size_type;
        using super::storage_type;
        using native_handle_type = util::android_traits::native_handle_type;

        using hash_type = util::android_traits::hash_type;
        using char_access = util::char_access;
        using builder_impl = util::builder_impl;

        static constexpr auto max_size = util::android_traits::max_size;

    public:
        using super::super;

        android_storage(JNIEnv * env, native_handle_type str) : 
            super(create_buffer(env, str))
        {}
        
    protected:
        ~android_storage() noexcept = default;
        android_storage(const android_storage & src) noexcept = default;
        android_storage(android_storage && src) noexcept = default;
        android_storage & operator=(const android_storage & rhs) noexcept = default;
        android_storage & operator=(android_storage && rhs) noexcept = default;

    public:

        using super::data;
        using super::copy_data;
        

        auto make_jstring(JNIEnv * env) const -> native_handle_type
        {
            auto data = get_buffer().data();
            if (!data)
                return nullptr;
            jstring ret = env->NewString(reinterpret_cast<const jchar *>(data), get_buffer().size());
            if (!ret)
                raise_exception_on_jni_error(env, "NewString failed");
            return ret;
        }
        
    protected:

        using super::size;
        using super::operator[];

        auto swap(android_storage & other) noexcept -> void
            { super::swap(other); }

    private:
        [[noreturn]] static void raise_exception_on_jni_error(JNIEnv * env, const char * desc) 
        {
            if (jthrowable ex = env->ExceptionOccurred())
                env->ExceptionClear();
            throw std::runtime_error(desc);
        }
        
        static buffer create_buffer(JNIEnv * env, jstring str)
        {
            if (!str)
                return buffer(nullptr);
            
            jsize length = str ? env->GetStringLength(str) : 0;
            if (length < 0)
                raise_exception_on_jni_error(env, "GetStringLength failed");
            
            if (length == 0)
                return buffer();

            buffer ret(length);
            env->GetStringRegion(str, 0, length, reinterpret_cast<jchar *>(ret.data()));
            return ret;
        }
    };
}

namespace sysstr::util
{
    template<>
    template<>
    inline char_access::char_access(const sys_string_t<android_storage> & src) noexcept:
        char_access(src.get_buffer())
    {}

    template<>
    inline sys_string_t<android_storage> build(builder_impl & builder) noexcept
    {
        return sys_string_t<android_storage>(convert_to_string(builder));
    }
}

namespace sysstr
{
    using sys_string_android = sys_string_t<android_storage>;
    using sys_string_android_builder = sys_string_builder_t<android_storage>;
}

#define SYS_STRING_STATIC_ANDROID(x) ([] () noexcept -> ::sysstr::sys_string_android { \
        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
        static const ::sysstr::util::static_buffer<size> sbuf{0, true, u##x}; \
        ::sysstr::util::buffer buf((::sysstr::util::dynamic_buffer *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string_android *>(&buf); \
    }())




