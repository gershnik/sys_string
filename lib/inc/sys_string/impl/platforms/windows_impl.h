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

namespace sysstr
{

    inline util::char_access::char_access(const sys_string & src) noexcept
    {
        m_buffer = (pointer)WindowsGetStringRawBuffer(src.h_str(), &m_size);
    }

    inline sys_string::sys_string(const char_access::cursor & src, size_type length):
        sys_string(src.iterator(), length)
    {}

    inline sys_string::sys_string(const char_access::reverse_cursor & src, size_type length):
        sys_string(src.iterator() - length, length)
    {}

    inline sys_string::sys_string(const char_access::iterator & first, const char_access::iterator & last):
        sys_string(first, last - first)
    {}

    inline sys_string util::build(builder_impl & builder) noexcept
    {
        return sys_string(convert_to_string(builder), handle_retain::no);
    }
}

//#define SYS_STRING_STATIC(x) ([] () noexcept -> ::systr::sys_string { \
//        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
//        using header_type = ::sysstr::util::hstring_header; \
//        static const header_type header{1, size - 1, 0, 0, u##x}; \
//        return ::sysstr::sys_string(HSTRING(uintptr_t(&header) | ::sysstr::util::hstring_static_allocation_bit) , handle_retain::no); \
//    }())

#define SYS_STRING_STATIC(x) ([] () noexcept -> ::sysstr::sys_string { \
        constexpr ::size_t size = sizeof(u##x) / sizeof(char16_t); \
        using header_type = ::sysstr::util::hstring_header; \
        static struct bug_workaround { const header_type header{1, size - 1, 0, 0, u##x}; } b; \
        return ::sysstr::sys_string(HSTRING(uintptr_t(&b.header) | ::sysstr::util::hstring_static_allocation_bit) , handle_retain::no); \
    }())

