//
// Copyright 2025 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_COMMON_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_COMMON_H_INCLUDED

#include <algorithm>
#include <iterator>

#include <sys_string/impl/unicode/utf_encoding.h>

namespace sysstr::util {

    template<utf_encoding Enc, class OutIt>
    static auto write_unsafe(char32_t c, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
    {
        if constexpr (Enc == utf32)
        {
            *dest++ = c;
            return dest;
        }
        else
        {
            utf_codepoint_encoder<Enc, false> encoder;
            encoder.put(c);
            return std::copy(encoder.begin(), encoder.end(), dest);
        }
    }

    template<utf_encoding Enc, class OutIt>
    static auto write_unsafe(const char16_t * begin, const char16_t * end, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
    {
        if constexpr (Enc == utf16)
        {
            return std::copy(begin ,end, dest);
        }
        else
        {
            utf_codepoint_decoder<utf16> decoder;
            while(begin != end)
            {
                decoder.put(*begin++);
                if (!decoder.done())
                    decoder.put(*begin++); //no need to bounds check, we know end is good
                dest = write_unsafe<Enc>(decoder.value(), dest);
            }
            return dest;
        }
    }
}

#endif
