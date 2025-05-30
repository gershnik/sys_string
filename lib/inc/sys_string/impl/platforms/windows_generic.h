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


#include <sys_string/impl/util/generic_impl.h>

namespace sysstr::util
{

    struct win_generic_traits
    {
        using storage_type = char16_t;
        using size_type = size_t;
        using hash_type = size_t;
        
        static constexpr size_type max_size = std::numeric_limits<ptrdiff_t>::max() / sizeof(char16_t);
    };

    template<size_t N> 
    using win_generic_static_string     = generic::static_string<win_generic_traits::storage_type, win_generic_traits::size_type, N>;
    using win_generic_dynamic_string    = generic::dynamic_string<win_generic_traits::storage_type, win_generic_traits::size_type>;

    using win_generic_any_string        = generic::any_string<win_generic_traits::storage_type, win_generic_traits::size_type>;

    using win_generic_builder_impl      = generic::builder_impl<win_generic_traits::storage_type, win_generic_traits::size_type>;

    using win_generic_char_access       = generic::char_access<win_generic_traits::storage_type, win_generic_traits::size_type>;
    
}

namespace sysstr
{
    class win_generic_storage: private util::generic::storage<util::win_generic_traits::storage_type, util::win_generic_traits::size_type>
    {
    friend util::win_generic_char_access;
    private:
        using super = util::generic::storage<util::win_generic_traits::storage_type, util::win_generic_traits::size_type>;
    public:
        using super::size_type;
        using super::storage_type;
        
        using hash_type = util::win_generic_traits::hash_type;
        using char_access = util::win_generic_char_access;
        using builder_impl = util::win_generic_builder_impl;

        static constexpr auto max_size = util::win_generic_traits::max_size;

    public:
        using super::super;

        template<has_utf_encoding Char>
        win_generic_storage(const Char * str, size_t len):
            super(str, len)
        {}

    protected:
        ~win_generic_storage() noexcept = default;
        win_generic_storage(const win_generic_storage & src) noexcept = default;
        win_generic_storage(win_generic_storage && src) noexcept = default;
        win_generic_storage & operator=(const win_generic_storage & rhs) noexcept = default;
        win_generic_storage & operator=(win_generic_storage && rhs) noexcept = default;

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

        auto swap(win_generic_storage & other) noexcept -> void
            { super::swap(other); }
    };
}

namespace sysstr::util
{
    template<>
    template<>
    inline 
    win_generic_char_access::char_access(const sys_string_t<win_generic_storage> & src) noexcept:
        char_access(src.m_buffer)
    {}

    template<>
    inline 
    sys_string_t<win_generic_storage> build(win_generic_builder_impl & builder) noexcept
    {
        return sys_string_t<win_generic_storage>(convert_to_string(builder));
    }
}

namespace sysstr
{
    using sys_string_win_generic = sys_string_t<win_generic_storage>;
    using sys_string_win_generic_builder = sys_string_builder_t<win_generic_storage>;
}

namespace sysstr::util 
{
    template<util::ct_string Str>
    inline auto make_static_sys_string_win_generic() noexcept -> sys_string_win_generic
    {
        constexpr ::size_t size = Str.size();
        static const win_generic_static_string<size> sbuf{0, true, Str};
        win_generic_any_string buf((win_generic_dynamic_string *)&sbuf, size - 1, 0);
        return *reinterpret_cast<sys_string_win_generic *>(&buf);
    }
}

#define SYS_STRING_STATIC_WIN_GENERIC(x) ::sysstr::util::make_static_sys_string_win_generic<u##x>()




