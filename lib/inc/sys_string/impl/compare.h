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

    #if (defined(__APPLE__) && defined(__MACH__))

    inline auto sys_string::compare(const sys_string & lhs, const sys_string & rhs) noexcept -> compare_result
    {
        auto lhs_ptr = lhs.cf_str();
        auto rhs_ptr = rhs.cf_str();

        if (lhs_ptr == rhs_ptr)
            return ordering_equal;
        if (!lhs_ptr)
            return CFStringGetLength(rhs_ptr) == 0 ? ordering_equal : ordering_less;
        if (!rhs_ptr)
            return CFStringGetLength(lhs_ptr) == 0 ? ordering_equal : ordering_greater;

        CFComparisonResult res = CFStringCompare(lhs_ptr, rhs_ptr, 0);
        return util::make_compare_result(res);
    }

    #else

    inline auto sys_string::compare(const sys_string & lhs, const sys_string & rhs) noexcept -> compare_result
    {
        sys_string::char_access lhs_access(lhs);
        sys_string::char_access rhs_access(rhs);

        return util::compare_3way(lhs_access.data(), lhs_access.size(), rhs_access.data(), rhs_access.size());
    }

    #endif

}


