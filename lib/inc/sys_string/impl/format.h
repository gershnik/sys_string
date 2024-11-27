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
        if constexpr (std::is_same_v<typename sys_string_t<Storage>::storage_type, char>)
        {
            sys_string_builder_t<Storage> builder;
            if (util::vprintf_to(builder.chars(), format, vl) <= 0)
                return sys_string_t<Storage>();
            return builder.build();
        }
        else
        {
            std::vector<char> buf;
            if (util::vprintf_to(buf, format, vl) <= 0)
                return sys_string_t<Storage>();
            return sys_string_t<Storage>(buf);
        }
    }

    #if SYS_STRING_SUPPORTS_STD_FORMAT

        template<class Storage>
        template<class... Args>
        inline
        auto sys_string_t<Storage>::std_format(std::format_string<Args...> fmt, Args &&... args) -> sys_string_t
        {
            sys_string_builder_t<Storage> builder;
            if constexpr (std::is_same_v<typename sys_string_t<Storage>::storage_type, char>)
            {
                std::format_to(std::back_inserter(builder.chars()), fmt, std::forward<Args>(args)...);
            }
            else
            {
                auto decoder = make_utf_output_decoder<utf8>(std::back_inserter(builder));
                std::format_to(decoder.begin(), fmt, std::forward<Args>(args)...);
                decoder.flush();
            }
            return builder.build();
        }

        template<class Storage>
        inline
        auto sys_string_t<Storage>::std_vformat(std::string_view fmt, std::format_args args) -> sys_string_t
        {
            sys_string_builder_t<Storage> builder;
            if constexpr (std::is_same_v<typename sys_string_t<Storage>::storage_type, char>)
            {
                std::vformat_to(std::back_inserter(builder.chars()), fmt, args);
            }
            else
            {
                auto decoder = make_utf_output_decoder<utf8>(std::back_inserter(builder));
                std::vformat_to(decoder.begin(), fmt, args);
                decoder.flush();
            }
            return builder.build();
        }
    #endif
}
