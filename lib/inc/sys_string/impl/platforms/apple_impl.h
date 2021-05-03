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

    inline util::char_access::char_access(const sys_string & src) noexcept:
        m_string(src.cf_str())
    {
        if (m_string)
        {
            m_size = CFStringGetLength(m_string);
            if ( (m_direct_unichar_buffer = CFStringGetCharactersPtr(m_string)) )
                return;
            m_direct_cstring_buffer = CFStringGetCStringPtr(m_string, kCFStringEncodingASCII);
        }
    }

    inline sys_string::sys_string(const char_access::cursor & src, size_type length):
        sys_string(src.container() ? src.container()->get_string() : nullptr, src.position(), src.position() + length)
    {}

    inline sys_string::sys_string(const char_access::reverse_cursor & src, size_type length):
        sys_string(src.container() ? src.container()->get_string() : nullptr, src.position() - length, src.position())
    {}

    inline sys_string::sys_string(const char_access::iterator & first, const char_access::iterator & last):
        sys_string(first, last.position() - first.position())
    {}

    inline sys_string util::build(builder_impl & builder) noexcept
    {
        auto str = convert_to_string(builder);
        return sys_string(str, handle_retain::no);
    }
}


#define SYS_STRING_STATIC(x) ::sysstr::sys_string(CFSTR(x), ::sysstr::handle_retain::no)
 

