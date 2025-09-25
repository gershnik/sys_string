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


#include <sys_string/impl/util/generic_impl.h>

//Like EM_JS but with inline and extern "C" only on data
#define SYSSTR_EM_JS(ret, c_name, js_name, params, ...)                        \
  ret c_name params EM_IMPORT(js_name);                                        \
  extern "C"                                                                   \
  __attribute__((visibility("hidden")))                                        \
  inline void * __em_js_ref_##c_name = (void*)&c_name;                         \
  extern "C"                                                                   \
  EMSCRIPTEN_KEEPALIVE                                                         \
  __attribute__((section("em_js"), aligned(1)))                                \
  inline char __em_js__##js_name[] =                                           \
    #params "<::>" #__VA_ARGS__;


namespace sysstr::util
{
    struct emscripten_traits
    {
        using storage_type = char16_t;
        using size_type = size_t;
        using hash_type = size_t;
        
        static constexpr size_type max_size = std::numeric_limits<ptrdiff_t>::max() / sizeof(char16_t);
    };

    template<size_t N>
    using emscripten_static_string     = generic::static_string<emscripten_traits::storage_type, emscripten_traits::size_type, N>;
    using emscripten_dynamic_string    = generic::dynamic_string<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_any_string        = generic::any_string<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_builder_impl      = generic::builder_impl<emscripten_traits::storage_type, emscripten_traits::size_type>;

    using emscripten_char_access       = generic::char_access<emscripten_traits::storage_type, emscripten_traits::size_type>;

#ifdef __wasm_reference_types__

    #if SYS_STRING_USE_WASM_JS_STRING
        uint32_t js_length(__externref_t str) __attribute__((import_module("wasm:js-string"), import_name("length")));
        uint32_t js_charCodeAt(__externref_t str, uint32_t idx) __attribute__((import_module("wasm:js-string"), import_name("charCodeAt")));
    #else

        SYSSTR_EM_JS(uint32_t, js_length, sysstr_length, (__externref_t str), {
            return str.length;
        });

        SYSSTR_EM_JS(uint32_t, js_charCodeAt, sysstr_charCodeAt, (__externref_t str, uint32_t idx), {
            return str.charCodeAt(idx);
        });
    #endif

    SYSSTR_EM_JS(__externref_t, js_UTF16ToString, sysstr_UTF16ToString, (const void * ptr, size_t length), {
        let start = (ptr>>1);
        if (length > 16 && UTF16Decoder)
            return UTF16Decoder.decode(HEAPU16.subarray(start, start + length));

        let ret = "";

        for (let i = 0; i < length; ++i) {
            const codeUnit = HEAPU16[start];
            ret += String.fromCharCode(codeUnit);
            ++start;
        }
        
        return ret;
    });
    
#endif

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

#ifdef __wasm_reference_types__
        emscripten_storage(__externref_t js_str):
            super(create_buffer(js_str))
        {}
#endif

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

        auto make_js_string() const noexcept -> emscripten::EM_VAL {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"
            
            auto ret = (emscripten::EM_VAL)EM_ASM_PTR({
                return Emval.toHandle(sysstr_UTF16ToString($0, $1));
            }, data(), size());

            return ret;

            #pragma clang diagnostic pop
            
        }

#ifdef __wasm_reference_types__
        auto make_js_string_ref() const noexcept -> __externref_t {
            return util::js_UTF16ToString(data(), size());
        }
#endif

    protected:
        using super::size;
        using super::operator[];

        auto swap(emscripten_storage & other) noexcept -> void
            { super::swap(other); }

    private:
        static auto create_buffer(emscripten::EM_VAL js_str) -> buffer
        {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

            size_t length = EM_ASM_INT({
                let str = Emval.toValue($0);
                return str.length;
            }, js_str);
            
            if (length == 0)
                return buffer();

            buffer ret(length);
            EM_ASM({
                let str = Emval.toValue($0);
                let ptr = $1;
                let length = str.length;
                for (let i = 0; i < length; ++i) {
                    HEAPU16[((ptr)>>1)] = str.charCodeAt(i);
                    ptr += 2;
                }
            }, js_str, ret.data());
            return ret;

            #pragma clang diagnostic pop
        }

#ifdef __wasm_reference_types__
        static auto create_buffer(__externref_t js_str) -> buffer {
            uint32_t len = util::js_length(js_str);

            if (len == 0)
                return buffer();

            buffer ret(len);
            auto ptr = ret.data();
            for(uint32_t i = 0; i < len; ++i)
                ptr[i] = util::js_charCodeAt(js_str, i);
            return ret;
        }
#endif
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

namespace sysstr::util 
{
    template<util::ct_string Str>
    inline auto make_static_sys_string_emscripten() noexcept -> sys_string_emscripten
    {
        constexpr ::size_t size = Str.size();
        static const emscripten_static_string<size> sbuf{0, true, Str};
        emscripten_any_string buf((emscripten_dynamic_string *)&sbuf, size - 1, 0);
        return *reinterpret_cast<sys_string_emscripten *>(&buf);
    }
}

#define SYS_STRING_STATIC_EMSCRIPTEN(x) ::sysstr::util::make_static_sys_string_emscripten<u##x>()




