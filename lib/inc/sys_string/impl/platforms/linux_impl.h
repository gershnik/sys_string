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

    template<>
    inline util::char_access::char_access(const sys_string & src) noexcept:
        char_access(src.m_storage.m_buffer)
    {}


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
        return sys_string(convert_to_string(builder));
    }
}

#define SYS_STRING_STATIC(x) ([] () noexcept -> ::sysstr::sys_string { \
        constexpr ::size_t size = sizeof(x) / sizeof(char); \
        static const ::sysstr::util::static_buffer<size> sbuf{0, true, x}; \
        ::sysstr::util::buffer buf((::sysstr::util::dynamic_buffer *)&sbuf, size - 1, 0); \
        return *reinterpret_cast<::sysstr::sys_string *>(&buf); \
    }())


