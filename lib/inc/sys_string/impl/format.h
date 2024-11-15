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

namespace sysstr
{
    template<class Storage>
    inline
    auto sys_string_t<Storage>::format(const char * format, ...) -> sys_string_t<Storage>
    {
        va_list vl;
        va_start(vl, format);
        return formatv(format, vl);
    }

    template<class Storage>
    auto sys_string_t<Storage>::formatv(const char * format, va_list vl) -> sys_string_t<Storage>
    {
        std::vector<char> buf;
        va_list try_vl;
        va_copy(try_vl, vl);
        const int size = vsnprintf(0, 0, format, try_vl);
        va_end(try_vl);
        if (size == -1)
            return sys_string_t();
        buf.resize(size + 1);
        const int ret = vsnprintf(&buf[0], buf.size(), format, vl);
        va_end(vl);
        if (ret <= 0)
            return sys_string_t();
        buf.resize(ret);
        return sys_string_t<Storage>(&buf[0], buf.size());
    }
}
