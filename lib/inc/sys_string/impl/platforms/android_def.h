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

#include <jni.h>

namespace sysstr
{

    struct sys_string_traits
    {
        using storage_type = char16_t;
        using size_type = jsize;
        using hash_type = unsigned;
        using native_handle_type = jstring;
        
        static constexpr size_type max_size = std::numeric_limits<jsize>::max() / sizeof(char16_t);
    };
}

namespace sysstr::util
{

    template<size_t N>
    using static_buffer     = generic::static_buffer<sys_string_traits::storage_type, sys_string_traits::size_type, N>;
    using dynamic_buffer    = generic::dynamic_buffer<sys_string_traits::storage_type, sys_string_traits::size_type>;

    using buffer            = generic::buffer<sys_string_traits::storage_type, sys_string_traits::size_type>;

    using builder_impl      = generic::buffer_builder<sys_string_traits::storage_type, sys_string_traits::size_type>;

    sys_string build(builder_impl &) noexcept;
    
    using char_access       = generic::char_access<sys_string_traits::storage_type, sys_string_traits::size_type>;
    
    class storage : private generic::storage<sys_string_traits::storage_type, sys_string_traits::size_type>
    {
    friend char_access;
    private:
        using super = generic::storage<sys_string_traits::storage_type, sys_string_traits::size_type>;
    public:
        using super::size_type;
        using super::storage_type;
        using native_handle_type = sys_string_traits::native_handle_type;

    public:
        using super::super;

        storage(JNIEnv * env, native_handle_type str) : 
            super(create_buffer(env, str))
        {}

        using super::data;
        using super::copy_data;
        using super::size;
        using super::operator[];
        

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
        
        auto swap(storage & other) noexcept -> void
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





