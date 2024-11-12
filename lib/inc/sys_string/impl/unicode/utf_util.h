//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_UTF_UTIL_INCLUDED
#define HEADER_SYS_STRING_UNICODE_UTF_UTIL_INCLUDED

#include <sys_string/impl/util/util.h>
#include <sys_string/impl/unicode/utf_encoding.h>

namespace sysstr
{

    template<utf_encoding From, utf_encoding To>
    class utf_converter
    {
    private:
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        static void reading(It first, EndIt last) noexcept(noexcept(*first++) && noexcept(first != last));
        template<std::ranges::forward_range Range>
        static void reading(Range && range) noexcept(noexcept(reading(std::begin(range), std::end(range))));
        template<std::output_iterator<utf_char_of<To>> OutIt>
        static void writing(OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<To>()));
        template<class Func>
        static void applying(Func func) noexcept(noexcept(func(utf_char_of<To>())));
    public:
        template<std::ranges::forward_range Range, class Func>
        static Func for_each_converted(Range && range, Func func) noexcept(noexcept(reading(std::forward<Range>(range))) && noexcept(applying(func)));

        template<std::ranges::forward_range Range, std::output_iterator<utf_char_of<To>> OutIt>
        SYS_STRING_FORCE_INLINE
        static OutIt convert(Range && range, OutIt dest) noexcept(noexcept(reading(std::forward<Range>(range))) && noexcept(writing(dest)))
        {
            for_each_converted(std::forward<Range>(range), [&](auto c) { *dest++ = c; });
            return dest;
        }

        template<std::ranges::forward_range Range>
        SYS_STRING_FORCE_INLINE
        static size_t converted_length(Range && range) noexcept(noexcept(reading(std::forward<Range>(range))))
        {
            size_t ret = 0;
            for_each_converted(std::forward<Range>(range), [&](auto) { ++ret; });
            return ret;
        }
        
        template<std::forward_iterator It, std::sentinel_for<It> EndIt, class Func>
        SYS_STRING_FORCE_INLINE
        static Func for_each_converted(It first, EndIt last, Func func) noexcept(noexcept(reading(first, last)) && noexcept(applying(func)))
        {
            using range_t = std::ranges::subrange<It, EndIt>;
            return for_each_converted(range_t(first, last), func);
        }
        
        
        template<std::forward_iterator It, std::sentinel_for<It> EndIt, std::output_iterator<utf_char_of<To>> OutIt>
        SYS_STRING_FORCE_INLINE
        static OutIt convert(It first, EndIt last, OutIt dest) noexcept(noexcept(reading(first, last)) && noexcept(writing(dest)))
        {
            for_each_converted(first, last, [&](auto c) { *dest++ = c; });
            return dest;
        }
        
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        SYS_STRING_FORCE_INLINE
        static size_t converted_length(It first, EndIt last) noexcept(noexcept(reading(first, last)))
        {
            size_t ret = 0;
            for_each_converted(first, last, [&](auto) { ++ret; });
            return ret;
        }

    };

    template<utf_encoding From>
    class utf32_input
    {
    private:
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        static void reading(It first, EndIt last) noexcept(noexcept(*first++) && noexcept(first != last));
        template<std::ranges::forward_range Range>
        static void reading(Range && range) noexcept(noexcept(reading(std::begin(range), std::end(range))));

    public:
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        static char32_t read(It & first, EndIt last) noexcept(noexcept(reading(first,last)));

        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        static char32_t read_reversed(It & first, EndIt last) noexcept(noexcept(reading(first,last)));

        template<std::ranges::forward_range Range, class Sink>
        SYS_STRING_FORCE_INLINE
        static Sink read(Range && range, Sink sink) noexcept(noexcept(reading(std::forward<Range>(range))) && noexcept(sink(char32_t())))
        {
            return read(std::begin(std::forward<Range>(range)), std::end(std::forward<Range>(range)), sink);
        }
        
        template<std::forward_iterator It, std::sentinel_for<It> EndIt, class Sink>
        SYS_STRING_FORCE_INLINE
        static Sink read(It first, EndIt last, Sink sink) noexcept(noexcept(reading(first,last)) && noexcept(sink(char32_t())))
        {
            while(first != last)
                sink(read(first, last));
            return sink;
        }
    };

    template<utf_encoding To>
    struct utf32_output
    {
        template<class Sink>
        struct writer
        {
            Sink sink;

            writer(Sink s) : sink(s) {}
            
            SYS_STRING_FORCE_INLINE
            constexpr void operator()(char32_t value) noexcept(noexcept(sink(utf_char_of<To>())))
            {
                if constexpr (To != utf32)
                {
                    utf_codepoint_encoder<To, false> encoder;
                    encoder.put(value);
                    for(auto first = encoder.begin(), last = encoder.end(); first != last; ++first)
                        sink(*first);
                }
                else 
                {
                    sink(value);
                }
            }
        };
        //Moronic GCC refuses to accept deduction guide not on namespace level
        //https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79501
        //template<class Sink> writer(Sink sink) -> writer<Sink>;
        
        template<class Sink> static auto make_writer(Sink sink) -> writer<Sink>
            { return writer<Sink>(sink); }
    };

    template<utf_encoding From, utf_encoding To>
    template<std::ranges::forward_range Range, class Func>
    SYS_STRING_FORCE_INLINE
    Func utf_converter<From, To>::for_each_converted(Range && range, Func func) 
        noexcept(noexcept(reading(std::forward<Range>(range))) && noexcept(applying(func)))
    {
        return utf32_input<From>::read(std::forward<Range>(range), utf32_output<To>::make_writer(func)).sink;
    }


    template<utf_encoding From>
    template<std::forward_iterator It, std::sentinel_for<It> EndIt>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<From>::read(It & first, EndIt last) noexcept(noexcept(reading(first,last)))
    {
        if constexpr (From == utf8) 
        {
            uint8_t byte = uint8_t(*first);
            ++first;
            if (byte <= 0x7f)
                return char32_t{byte};

            utf_codepoint_decoder<utf8> decoder;
            decoder.put(byte);
            if (decoder.error() || first == last)
                return char32_t{U'\uFFFD'};

            for ( ; ; )
            {
                byte = uint8_t(*first);
                decoder.put(byte);
                if (decoder.error())
                    return char32_t{U'\uFFFD'};
                ++first;
                if (decoder.done())
                    return char32_t{decoder.value()};
                if (first == last)
                    return char32_t{U'\uFFFD'};
            }
        }
        else if constexpr (From == utf16) 
        {
            utf_codepoint_decoder<utf16> decoder;

            decoder.put(uint16_t(*first));
            ++first;
            if (decoder.done())
                return char32_t{decoder.value()};

            if (decoder.error() || first == last)
                return char32_t{U'\uFFFD'};

            decoder.put(uint16_t(*first));
            if (!decoder.done())
                return char32_t{U'\uFFFD'};
            ++first;

            return char32_t{decoder.value()};
        } 
        else if constexpr (From == utf32)
        {
            utf_codepoint_decoder<utf32> decoder;
            bool res = decoder.put(uint32_t(*first));
            ++first;
            return res ? char32_t{decoder.value()} : U'\uFFFD';
        }


    }

    template<utf_encoding From>
    template<std::forward_iterator It, std::sentinel_for<It> EndIt>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<From>::read_reversed(It & first, EndIt last) noexcept(noexcept(reading(first, last)))
    {
        if constexpr (From == utf8)
        {
            uint8_t byte = uint8_t(*first);
            ++first;
            if (byte <= 0x7f)
                return char32_t{byte};
            
            utf_reverse_codepoint_decoder<utf8> decoder;
            decoder.put(byte);
            if (decoder.error() || first == last)
                return char32_t{U'\uFFFD'};

            It rewind_point = first;
            for ( ; ; )
            {
                byte = uint8_t(*first);
                ++first;
                decoder.put(byte);

                if (decoder.done())
                    return char32_t{decoder.value()};
                
                if (decoder.error())
                { 
                    first = std::move(rewind_point);
                    return char32_t{U'\uFFFD'};
                }
                
                if (first == last)
                    return char32_t{u'\uFFFD'};
            }
        }
        else if constexpr (From == utf16)
        {
            utf_reverse_codepoint_decoder<utf16> decoder;

            decoder.put(uint16_t(*first));
            ++first;
            if (decoder.done())
                return char32_t{decoder.value()};

            if (decoder.error() || first == last)
                return char32_t{U'\uFFFD'};

            decoder.put(uint16_t(*first));
            if (!decoder.done())
                return char32_t{U'\uFFFD'};
            ++first;

            return char32_t{decoder.value()};
        }
        else if constexpr (From == utf32)
        {
            return utf32_input::read(first, last);
        }
    }
}

#endif 


