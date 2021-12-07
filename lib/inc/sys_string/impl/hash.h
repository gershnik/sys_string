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

namespace std
{
    template<class Storage>
    struct hash<::sysstr::sys_string_t<Storage>>
    {
        auto operator()(const ::sysstr::sys_string_t<Storage> & val) const noexcept -> typename ::sysstr::sys_string_t<Storage>::hash_type;
    };

    #if (defined(__APPLE__) && defined(__MACH__))

        template<>
        inline auto hash<::sysstr::sys_string_cfstr>::operator()(const ::sysstr::sys_string_cfstr & val) const noexcept -> CFHashCode
        {
            if (auto str = val.cf_str(); str)
                return CFHash(str);
            return 0;
        }

    #endif

    template<class Storage>
    inline auto hash<::sysstr::sys_string_t<Storage>>::operator()(const ::sysstr::sys_string_t<Storage> & val) const noexcept
        -> typename ::sysstr::sys_string_t<Storage>::hash_type
    {
        using namespace sysstr;

        typename sys_string_t<Storage>::char_access access(val);
        std::hash<typename sys_string_t<Storage>::char_access::value_type> hasher;
        typename sys_string_t<Storage>::hash_type result = 0;
        for(typename sys_string_t<Storage>::char_access::size_type i = 0, count = access.size(); i < count; ++i)
            result = result * 31 + hasher(access[i]);
        return result;
    }

}
