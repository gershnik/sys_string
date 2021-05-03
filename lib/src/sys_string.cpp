//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#include <sys_string/sys_string.h>

using namespace sysstr;
using namespace sysstr::util;


auto sys_string::formatv(const char * format, va_list vl) -> sys_string
{
    std::vector<char> buf;
    va_list try_vl;
    va_copy(try_vl, vl);
    const int size = vsnprintf(0, 0, format, try_vl);
    va_end(try_vl);
    if (size == -1)
        return sys_string();
    buf.resize(size + 1);
    const int ret = vsprintf(&buf[0], format, vl);
    va_end(vl);
    if (ret <= 0)
        return sys_string();
    buf.resize(ret);
    return sys_string(&buf[0], buf.size());
}

auto sys_string::compare_no_case(const sys_string lhs, const sys_string & rhs) noexcept -> compare_result
{
    sys_string::utf32_view lhs_view(lhs);
    sys_string::utf32_view rhs_view(rhs);

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
            return make_compare_result(res);
        
        std::copy(lhs_buf + shortest_length, lhs_buf_end, lhs_buf);
        lhs_buf_end = lhs_buf_end - shortest_length;
        std::copy(rhs_buf + shortest_length, rhs_buf_end, rhs_buf);
        rhs_buf_end = rhs_buf_end - shortest_length;
    }
    if (auto res = compare_3way(lhs_buf_end - lhs_buf, rhs_buf_end - rhs_buf); res != 0)
        return res;
#if SYS_STRING_USE_SPACESHIP_OPERATOR
    return (rhs_first == rhs_last) <=> (lhs_first == lhs_last);
#else
    return (rhs_first == rhs_last) - (lhs_first == lhs_last);
#endif
}





            





