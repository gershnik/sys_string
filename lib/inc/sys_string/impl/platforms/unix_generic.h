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

#include <limits>

namespace sysstr::util
{
    struct generic_traits
    {
        using storage_type = char;
        using size_type = size_t;
        using hash_type = unsigned;
        
        static constexpr size_type max_size = std::numeric_limits<ptrdiff_t>::max() / sizeof(char);
    };

    template<size_t N> 
    using generic_static_string     = generic::static_string<generic_traits::storage_type, generic_traits::size_type, N>;
    using generic_dynamic_string    = generic::dynamic_string<generic_traits::storage_type, generic_traits::size_type>;

    using generic_any_string        = generic::any_string<generic_traits::storage_type, generic_traits::size_type>;

    using generic_builder_impl      = generic::any_string_builder<generic_traits::storage_type, generic_traits::size_type>;

    using generic_char_access       = generic::char_access<generic_traits::storage_type, generic_traits::size_type>;
    
}

namespace sysstr
{
    class generic_storage: private util::generic::storage<util::generic_traits::storage_type, util::generic_traits::size_type>
    {
    friend util::generic_char_access;
    private:
        using super = util::generic::storage<util::generic_traits::storage_type, util::generic_traits::size_type>;
    public:
        using super::size_type;
        using super::storage_type;
        
        using hash_type = util::generic_traits::hash_type;
        using char_access = util::generic_char_access;
        using builder_impl = util::generic_builder_impl;

        static constexpr auto max_size = util::generic_traits::max_size;

    public:
        using super::super;

#ifdef _MSC_VER
        template<has_utf_encoding Char>
        generic_storage(const Char * str, size_t len):
            super(str, len)
        {}
#endif

    protected:
        ~generic_storage() noexcept = default;
        generic_storage(const generic_storage & src) noexcept = default;
        generic_storage(generic_storage && src) noexcept = default;
        generic_storage & operator=(const generic_storage & rhs) noexcept = default;
        generic_storage & operator=(generic_storage && rhs) noexcept = default;

    public:

        using super::data;
        using super::copy_data;
        
        auto c_str() const noexcept -> const char *
        {
            auto ret = data();
            return ret ? ret : "";
        }
        
    protected:
        using super::size;
        using super::operator[];

        auto swap(generic_storage & other) noexcept -> void
            { super::swap(other); }
    };
}

namespace sysstr::util
{
    template<>
    template<>
    inline 
    generic_char_access::char_access(const sys_string_t<generic_storage> & src) noexcept:
        char_access(src.m_buffer)
    {}

    template<>
    inline 
    sys_string_t<generic_storage> build(generic_builder_impl & builder) noexcept
    {
        return sys_string_t<generic_storage>(convert_to_string(builder));
    }
}

namespace sysstr
{
    using sys_string_generic = sys_string_t<generic_storage>;
    using sys_string_generic_builder = sys_string_builder_t<generic_storage>;
}

#define SYS_STRING_STATIC_GENERIC(x) ([] () noexcept -> ::sysstr::sys_string_generic { \
        constexpr ::size_t size = sizeof(x) / sizeof(char); \
        static const ::sysstr::util::generic_static_string<size> sbuf{0, true, x}; \
        ::sysstr::util::generic_any_string buf((::sysstr::util::generic_dynamic_string *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string_generic *>(&buf); \
    }())





