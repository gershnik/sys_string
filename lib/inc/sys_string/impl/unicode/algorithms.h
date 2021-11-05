//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_ALGORITHMS_H_INCLUDED

#include <sys_string/impl/unicode/mappings.h>

namespace sysstr
{
    struct isspace
    {
        auto operator()(char32_t c) const noexcept -> bool
        {
            return util::unicode::isspace(c);
        }
    };

    template<utf_encoding OutEnc>
    struct casefold
    {
        static constexpr auto max_output_length = util::unicode::mapper::max_mapped_length;
        
        template<class OutIt>
        auto operator()(char32_t c, OutIt dest) const noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            return util::unicode::mapper::case_fold.map_char<OutEnc>(c, dest);
        }
    };

    namespace util
    {
        class sigma_tolower
        {
        public:
            template<class Cursor>
            auto operator()(Cursor cursor) const noexcept -> char32_t
            {
                if (any_non_cased_then_cased(cursor.reverse()))
                {
                    ++cursor;
                    if (!any_non_cased_then_cased(cursor))
                    {
                        return U'\u03C2'; //replace with ς
                    }
                }
                return U'\u03C3'; // replace with σ
            }
        private:
            template<class Cursor>
            auto any_non_cased_then_cased(Cursor cursor) const noexcept -> bool
            {
                for( ; cursor; ++cursor)
                {
                    auto c = *cursor;
                    auto props = unicode::get_char_prop(c);
                    if (uint8_t(props & unicode::char_prop::case_ignorable))
                        continue;
                    return uint8_t(props & unicode::char_prop::cased) != 0;
                }
                return false;
            }
        };
    }

    template<utf_encoding OutEnc>
    struct tolower
    {
        template<class Cursor, class OutIt>
        inline auto operator()(Cursor cursor, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;
            
            for( ; cursor; ++cursor)
            {
                auto c = *cursor;
                if (c != U'\u03A3') // not Σ
                {
                    dest = mapper::to_lower_case.map_char<OutEnc>(c, dest);
                }
                else
                {
                    char32_t lc = util::sigma_tolower()(cursor);
                    auto writer = utf32_output<OutEnc>::make_writer([&](auto value) {
                        *dest++ = value;
                    });
                    writer(lc);
                }
            }
            return dest;
        }
    };

    template<utf_encoding OutEnc>
    struct toupper
    {
        template<class Cursor, class OutIt>
        inline auto operator()(Cursor cursor, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<OutEnc>())) -> OutIt
        {
            using namespace util::unicode;
            
            for( ; cursor; ++cursor)
            {
                auto c = *cursor;
                dest = mapper::to_upper_case.map_char<OutEnc>(c, dest);
            }

            return dest;
        }
    };

}

#endif
