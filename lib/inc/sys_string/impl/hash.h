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


#if (defined(__APPLE__) && defined(__MACH__))

inline auto std::hash<::sysstr::sys_string>::operator()(const ::sysstr::sys_string & val) const noexcept -> CFHashCode
{
    if (auto str = val.cf_str(); str)
        return CFHash(str);
    return 0;
}

#else

inline auto std::hash<::sysstr::sys_string>::operator()(const ::sysstr::sys_string & val) const noexcept -> ::sysstr::sys_string_traits::hash_type
{
    using namespace sysstr;

    sys_string::char_access access(val);
    std::hash<sys_string::char_access::value_type> hasher;
    sys_string_traits::hash_type result = 0;
    for(sys_string::char_access::size_type i = 0, count = access.size(); i < count; ++i)
        result = result * 31 + hasher(access[i]);
    return result;
}

#endif
