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

    template<>
    inline auto sys_string_cfstr::compare(const sys_string_t<cf_storage> & lhs, const sys_string_t<cf_storage> & rhs) noexcept -> compare_result
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

    #endif

    template<class Storage>
    inline auto sys_string_t<Storage>::compare(const sys_string_t<Storage> & lhs, const sys_string_t<Storage> & rhs) noexcept -> compare_result
    {
        char_access lhs_access(lhs);
        char_access rhs_access(rhs);

        return util::compare_3way(lhs_access.data(), lhs_access.size(), rhs_access.data(), rhs_access.size());
    }

    template<class Storage>
    auto sys_string_t<Storage>::compare_no_case(const sys_string_t<Storage> lhs, const sys_string_t<Storage> & rhs) noexcept -> compare_result
    {
        utf32_view lhs_view(lhs);
        utf32_view rhs_view(rhs);

        auto lhs_first = lhs_view.begin(), lhs_last = lhs_view.end();
        auto rhs_first = rhs_view.begin(), rhs_last = rhs_view.end();

        casefold<utf32> folder;

        constexpr auto buf_size = 32;
        static_assert(buf_size > folder.max_output_length, "buffer size is smaller than max output from case folder");

        char32_t lhs_buf[buf_size];
        char32_t rhs_buf[buf_size];

        char32_t * lhs_buf_end = lhs_buf;
        char32_t * rhs_buf_end = rhs_buf;

        for( ; ; )
        {
            while (lhs_first != lhs_last && size_t(std::end(lhs_buf) - lhs_buf_end) >= folder.max_output_length)
                lhs_buf_end = folder(*lhs_first++, lhs_buf_end);
            while (rhs_first != rhs_last && size_t(std::end(rhs_buf) - rhs_buf_end) >= folder.max_output_length)
                rhs_buf_end = folder(*rhs_first++, rhs_buf_end);

            auto shortest_length = std::min(lhs_buf_end - lhs_buf, rhs_buf_end - rhs_buf);
            if (shortest_length == 0)
                break;
            if (int res = std::char_traits<char32_t>::compare(lhs_buf, rhs_buf, shortest_length))
                return util::make_compare_result(res);

            std::copy(lhs_buf + shortest_length, lhs_buf_end, lhs_buf);
            lhs_buf_end = lhs_buf_end - shortest_length;
            std::copy(rhs_buf + shortest_length, rhs_buf_end, rhs_buf);
            rhs_buf_end = rhs_buf_end - shortest_length;
        }
        if (auto res = util::compare_3way(lhs_buf_end - lhs_buf, rhs_buf_end - rhs_buf); res != 0)
            return res;
#if SYS_STRING_USE_SPACESHIP_OPERATOR
        return (rhs_first == rhs_last) <=> (lhs_first == lhs_last);
#else
        return (rhs_first == rhs_last) - (lhs_first == lhs_last);
#endif
    }

}


