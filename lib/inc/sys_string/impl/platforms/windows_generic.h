//
// Copyright 2021 Eugene Gershnik
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

    struct generic_traits
    {
        using storage_type = char16_t;
        using size_type = size_t;
        using hash_type = unsigned;
        
        static constexpr size_type max_size = std::numeric_limits<size_t>::max() / sizeof(char16_t);
    };

    template<size_t N> 
    using generic_static_buffer     = generic::static_buffer<generic_traits::storage_type, generic_traits::size_type, N>;
    using generic_dynamic_buffer    = generic::dynamic_buffer<generic_traits::storage_type, generic_traits::size_type>;

    using generic_buffer            = generic::buffer<generic_traits::storage_type, generic_traits::size_type>;

    using generic_builder_impl      = generic::buffer_builder<generic_traits::storage_type, generic_traits::size_type>;

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

        generic_storage(const wchar_t * str, size_t len):
            generic_storage((char16_t*)str, len)
        {}

        using super::data;
        using super::copy_data;
        using super::size;
        using super::operator[];

        auto swap(storage & other) noexcept -> void
            { super::swap(other); }
    };
}

namespace sysstr::util
{
    template<>
    template<>
    inline 
    generic_char_access::char_access(const sys_string_t<generic_storage> & src) noexcept:
        char_access(src.m_storage.m_buffer)
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
    template<>
    inline 
    sys_string_t<generic_storage>::sys_string_t(const char_access::cursor & src, size_type length):
        sys_string_t(src.iterator(), length)
    {}

    template<>
    inline 
    sys_string_t<generic_storage>::sys_string_t(const char_access::reverse_cursor & src, size_type length):
        sys_string_t(src.iterator() - length, length)
    {}

    template<>
    inline 
    sys_string_t<generic_storage>::sys_string_t(const char_access::iterator & first, const char_access::iterator & last):
        sys_string_t(first, last - first)
    {}

    using sys_string_generic = sys_string_t<generic_storage>;
    using sys_string_generic_builder = sys_string_builder_t<generic_storage>;
}

#define SYS_STRING_STATIC_GENERIC(x) ([] () noexcept -> ::sysstr::sys_string_generic { \
        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
        static const ::sysstr::util::generic_static_buffer<size> sbuf{0, true, u##x}; \
        ::sysstr::util::generic_buffer buf((::sysstr::util::generic_dynamic_buffer *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string_generic *>(&buf); \
    }())





