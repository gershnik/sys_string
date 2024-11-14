//
// Copyright 2022 Eugene Gershnik
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
    struct emscripten_traits
    {
        using storage_type = char16_t;
        using size_type = size_t;
        using hash_type = unsigned;
        
        static constexpr size_type max_size = std::numeric_limits<ptrdiff_t>::max() / sizeof(char16_t);
    };

    template<size_t N>
    using emscripten_static_string     = generic::static_string<emscripten_traits::storage_type, emscripten_traits::size_type, N>;
    using emscripten_dynamic_string    = generic::dynamic_string<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_any_string        = generic::any_string<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_builder_impl      = generic::any_string_builder<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_char_access       = generic::char_access<emscripten_traits::storage_type, emscripten_traits::size_type>;

}

namespace sysstr
{
    class emscripten_storage: private util::generic::storage<util::emscripten_traits::storage_type, util::emscripten_traits::size_type>
    {
    friend util::emscripten_char_access;
    private:
        using super = util::generic::storage<util::emscripten_traits::storage_type, util::emscripten_traits::size_type>;
    public:
        using super::size_type;
        using super::storage_type;
        
        using hash_type = util::emscripten_traits::hash_type;
        using char_access = util::emscripten_char_access;
        using builder_impl = util::emscripten_builder_impl;

        static constexpr auto max_size = util::emscripten_traits::max_size;

    public:
        using super::super;

        emscripten_storage(emscripten::EM_VAL js_str):
            super(create_buffer(js_str))
        {}

#ifdef _MSC_VER
        template<has_utf_encoding Char>
        emscripten_storage(const Char * str, size_t len):
            super(str, len)
        {}
#endif

    protected:
        ~emscripten_storage() noexcept = default;
        emscripten_storage(const emscripten_storage & src) noexcept = default;
        emscripten_storage(emscripten_storage && src) noexcept = default;
        emscripten_storage & operator=(const emscripten_storage & rhs) noexcept = default;
        emscripten_storage & operator=(emscripten_storage && rhs) noexcept = default;

    public:

        using super::data;
        using super::copy_data;

        auto make_js_string() const noexcept -> emscripten::EM_VAL 
        {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
            
            auto ret = (emscripten::EM_VAL)EM_ASM_PTR({
                let ptr = $0;
                let length = $1;
                if (length > 16 && UTF16Decoder)
                    return Emval.toHandle(UTF16Decoder.decode(HEAPU8.subarray(ptr, ptr + length  * 2)));
                
                let ret = "";
  
                for (let i = 0; i < length; ++i) {
                    const codeUnit = HEAP16[((ptr)>>1)];
                    ret += String.fromCharCode(codeUnit);
                    ptr += 2;
                }
                
                return Emval.toHandle(ret);
            }, data(), size());

            return ret;

            #pragma clang diagnostic pop
            
        }

    protected:
        using super::size;
        using super::operator[];

        auto swap(emscripten_storage & other) noexcept -> void
            { super::swap(other); }

    private:
        static auto create_buffer(emscripten::EM_VAL js_str) -> any_string
        {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

            size_t length = EM_ASM_INT({
                let str = Emval.toValue($0);
                return str.length;
            }, js_str);
            
            if (length == 0)
                return any_string();

            any_string ret(length);
            EM_ASM({
                let str = Emval.toValue($0);
                let ptr = $1;
                let length = str.length;
                for (let i = 0; i < length; ++i) {
                    HEAP16[((ptr)>>1)] = str.charCodeAt(i);
                    ptr += 2;
                }
            }, js_str, ret.data());
            return ret;

            #pragma clang diagnostic pop
        }
    };
}

namespace sysstr::util
{
    template<>
    template<>
    inline emscripten_char_access::char_access(const sys_string_t<emscripten_storage> & src) noexcept:
        char_access(src.get_buffer())
    {}

    template<>
    inline sys_string_t<emscripten_storage> build(emscripten_builder_impl & builder) noexcept
    {
        return sys_string_t<emscripten_storage>(convert_to_string(builder));
    }
}

namespace sysstr
{
    using sys_string_emscripten = sys_string_t<emscripten_storage>;
    using sys_string_emscripten_builder = sys_string_builder_t<emscripten_storage>;
}

#define SYS_STRING_STATIC_EMSCRIPTEN(x) ([] () noexcept -> ::sysstr::sys_string_emscripten { \
        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
        static const ::sysstr::util::emscripten_static_string<size> sbuf{0, true, u##x}; \
        ::sysstr::util::emscripten_any_string buf((::sysstr::util::emscripten_dynamic_string *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string_emscripten *>(&buf); \
    }())




