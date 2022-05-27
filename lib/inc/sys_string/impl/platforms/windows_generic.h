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

    struct win_generic_traits
    {
        using storage_type = char16_t;
        using size_type = size_t;
        using hash_type = unsigned;
        
        static constexpr size_type max_size = std::numeric_limits<size_t>::max() / sizeof(char16_t);
    };

    template<size_t N> 
    using win_generic_static_buffer     = generic::static_buffer<generic_traits::storage_type, generic_traits::size_type, N>;
    using win_generic_dynamic_buffer    = generic::dynamic_buffer<generic_traits::storage_type, generic_traits::size_type>;

    using win_generic_buffer            = generic::buffer<generic_traits::storage_type, generic_traits::size_type>;

    using win_generic_builder_impl      = generic::buffer_builder<generic_traits::storage_type, generic_traits::size_type>;

    using win_generic_char_access       = generic::char_access<generic_traits::storage_type, generic_traits::size_type>;
    
}

namespace sysstr
{
    class win_generic_storage: private util::generic::storage<util::generic_traits::storage_type, util::generic_traits::size_type>
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

        template<class Char>
        generic_storage(const Char * str, size_t len, std::enable_if_t<has_utf_encoding<Char>> * = nullptr):
            super(str, len)
        {}

    protected:
        ~generic_storage() noexcept = default;
        generic_storage(const generic_storage & src) noexcept = default;
        generic_storage(generic_storage && src) noexcept = default;
        generic_storage & operator=(const generic_storage & rhs) noexcept = default;
        generic_storage & operator=(generic_storage && rhs) noexcept = default;

    public:
        using super::data;
        using super::copy_data;

        auto w_str() const noexcept -> const wchar_t *
        { 
            auto ret = (const wchar_t *)data();
            return ret ? ret : L""; 
        }

    protected:
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
    win_generic_char_access::char_access(const sys_string_t<generic_storage> & src) noexcept:
        char_access(src.m_buffer)
    {}

    template<>
    inline 
    sys_string_t<generic_storage> build(win_generic_builder_impl & builder) noexcept
    {
        return sys_string_t<generic_storage>(convert_to_string(builder));
    }
}

namespace sysstr
{
    template<>
    inline 
    sys_string_t<win_generic_storage>::sys_string_t(const char_access::cursor & src, size_type length):
        sys_string_t(src.iterator(), length)
    {}

    template<>
    inline 
    sys_string_t<win_generic_storage>::sys_string_t(const char_access::reverse_cursor & src, size_type length):
        sys_string_t(src.iterator() - length, length)
    {}

    template<>
    inline 
    sys_string_t<win_generic_storage>::sys_string_t(const char_access::iterator & first, const char_access::iterator & last):
        sys_string_t(first, last - first)
    {}

    using sys_string_win_generic = sys_string_t<win_generic_storage>;
    using sys_string_win_generic_builder = sys_string_builder_t<win_generic_storage>;
}

#define SYS_STRING_STATIC_WIN_GENERIC(x) ([] () noexcept -> ::sysstr::sys_string_generic { \
        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
        static const ::sysstr::util::generic_static_buffer<size> sbuf{0, true, u##x}; \
        ::sysstr::util::generic_buffer buf((::sysstr::util::generic_dynamic_buffer *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string_generic *>(&buf); \
    }())





