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
#include <sys_string/impl/util/cursor.h>
#include <sys_string/impl/unicode/utf_encoding.h>

namespace sysstr
{

    template<utf_encoding From, utf_encoding To>
    class utf_converter
    {
    private:
        template<class InCursor>
        static void reading(InCursor & cursor) noexcept(noexcept(*cursor) && noexcept(++cursor) && noexcept(bool(cursor)));
        template<class InIt, class EndIt>
        static void reading(InIt first, EndIt last) noexcept(noexcept(*first++) && noexcept(first != last));
        template<class OutIt>
        static void writing(OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<To>()));
        template<class Func>
        static void applying(Func func) noexcept(noexcept(func(utf_char_of<To>())));
    public:
        template<class InCursor, class Func>
        static Func for_each_converted(InCursor & cursor, Func func) noexcept(noexcept(reading(cursor)) && noexcept(applying(func)));
        
        template<class InCursor, class OutIt>
        SYS_STRING_FORCE_INLINE
        static OutIt convert(InCursor & cursor, OutIt dest) noexcept(noexcept(reading(cursor)) && noexcept(writing(dest)))
        {
            for_each_converted(cursor, [&](auto c) { *dest++ = c; });
            return dest;
        }
        
        template<class InCursor>
        SYS_STRING_FORCE_INLINE
        static size_t converted_length(InCursor & cursor) noexcept(noexcept(reading(cursor)))
        {
            size_t ret = 0;
            for_each_converted(cursor, [&](auto) { ++ret; });
            return ret;
        }
        
        template<class InIt, class EndIt, class Func>
        SYS_STRING_FORCE_INLINE
        static Func for_each_converted(InIt first, EndIt last, Func func) noexcept(noexcept(reading(first, last)) && noexcept(applying(func)))
        {
            util::iter_cursor<InIt, InIt, util::cursor_direction::forward> cursor(first, last, first);
            return for_each_converted(cursor, func);
        }
        
        
        template<class InIt, class EndIt, class OutIt>
        SYS_STRING_FORCE_INLINE
        static OutIt convert(InIt first, EndIt last, OutIt dest) noexcept(noexcept(reading(first, last)) && noexcept(writing(dest)))
        {
            for_each_converted(first, last, [&](auto c) { *dest++ = c; });
            return dest;
        }
        
        template<class InIt, class EndIt>
        SYS_STRING_FORCE_INLINE
        static size_t converted_length(InIt first, EndIt last) noexcept(noexcept(reading(first, last)))
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
        template<class InCursor>
        static void reading(InCursor & cursor) noexcept(noexcept(*cursor) && noexcept(++cursor) && noexcept(bool(cursor)));
        template<class InIt, class EndIt>
        static void reading(InIt first, EndIt last) noexcept(noexcept(*first++) && noexcept(first != last));

    public:
        template<class InCursor>
        static char32_t read(InCursor & cursor) noexcept(noexcept(reading(cursor)));

        template<class InCursor>
        static char32_t read_reversed(InCursor & cursor) noexcept(noexcept(reading(cursor)));

        template<class InCursor, class Sink>
        SYS_STRING_FORCE_INLINE
        static Sink read(InCursor & cursor, Sink sink) noexcept(noexcept(reading(cursor)) && noexcept(sink(char32_t())))
        {
            while(cursor)
                sink(read(cursor));
            return sink;
        }
        
        template<class InIt, class EndIt, class Sink>
        SYS_STRING_FORCE_INLINE
        static Sink read(InIt first, EndIt last, Sink sink) noexcept(noexcept(reading(first,last)) && noexcept(sink(char32_t())))
        {
            util::iter_cursor<InIt, EndIt, util::cursor_direction::forward> cursor(first, last, first);
            return read(cursor, sink);
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
                utf_codepoint_encoder<To, false> encoder;
                encoder.put(value);
                for(auto first = encoder.begin(), last = encoder.end(); first != last; ++first)
                    sink(*first);
            }
        };
        //Moronic GCC refuses to accept deduction guide not on namespace level
        //https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79501
        //template<class Sink> writer(Sink sink) -> writer<Sink>;
        
        template<class Sink> static auto make_writer(Sink sink) -> writer<Sink>
            { return writer<Sink>(sink); }
    };

    template<>
    struct utf32_output<utf32>
    {
        template<class Sink>
        struct writer
        {
            Sink sink;

            writer(Sink s) : sink(s) {}
            
            constexpr void operator()(char32_t value) noexcept(noexcept(sink(value)))
            {
                sink(value);
            }
        };
        //Moronic GCC refuses to accept deduction guide not on namespace level
        //https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79501
        //template<class Sink> writer(Sink sink) -> writer<Sink>;
        template<class Sink> static auto make_writer(Sink sink) -> writer<Sink>
            { return writer<Sink>(sink); }
    };

    template<utf_encoding From, utf_encoding To>
    template<class InCursor, class Func>
    SYS_STRING_FORCE_INLINE
    Func utf_converter<From, To>::for_each_converted(InCursor & cursor, Func func) noexcept(noexcept(reading(cursor)) && noexcept(applying(func)))
    {
        return utf32_input<From>::read(cursor, utf32_output<To>::make_writer(func)).sink;
    }


    //MARK:- UTF-16

    #ifndef __INTELLISENSE__
    template<>
    template<class InCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf16>::read(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        utf_codepoint_decoder<utf16> decoder;

        decoder.put(uint16_t(*cursor));
        ++cursor;
        if (decoder.done())
            return char32_t{decoder.value()};

        if (decoder.error() || !cursor)
            return char32_t{U'\uFFFD'};

        decoder.put(uint16_t(*cursor));
        if (!decoder.done())
            return char32_t{U'\uFFFD'};
        ++cursor;

        return char32_t{decoder.value()};
    }

    template<>
    template<class InCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf16>::read_reversed(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        utf_reverse_codepoint_decoder<utf16> decoder;

        decoder.put(uint16_t(*cursor));
        ++cursor;
        if (decoder.done())
            return char32_t{decoder.value()};

        if (decoder.error() || !cursor)
            return char32_t{U'\uFFFD'};

        decoder.put(uint16_t(*cursor));
        if (!decoder.done())
            return char32_t{U'\uFFFD'};
        ++cursor;

        return char32_t{decoder.value()};
    }
    #endif

    //MARK:- UTF-8

    #ifndef __INTELLISENSE__
    template<>
    template<class InCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf8>::read(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        uint8_t byte = uint8_t(*cursor);
        ++cursor;
        if (byte <= 0x7f)
            return char32_t{byte};

        utf_codepoint_decoder<utf8> decoder;
        decoder.put(byte);
        if (decoder.error() || !cursor)
            return char32_t{U'\uFFFD'};

        for ( ; ; )
        {
            byte = uint8_t(*cursor);
            decoder.put(byte);
            if (decoder.error())
                return char32_t{U'\uFFFD'};
            ++cursor;
            if (decoder.done())
                return char32_t{decoder.value()};
            if (!cursor)
                return char32_t{U'\uFFFD'};
        }
    }

    template<>
    template<class InCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf8>::read_reversed(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        uint8_t byte = uint8_t(*cursor);
        ++cursor;
        if (byte <= 0x7f)
            return char32_t{byte};
        
        utf_reverse_codepoint_decoder<utf8> decoder;
        decoder.put(byte);
        if (decoder.error() || !cursor)
            return char32_t{U'\uFFFD'};

        for (uint8_t rewind_count = 1; ; ++rewind_count)
        {
            byte = uint8_t(*cursor);
            ++cursor;
            decoder.put(byte);

            if (decoder.done())
                return char32_t{decoder.value()};
            
            if (decoder.error())
            { 
                cursor -= rewind_count;
                return char32_t{U'\uFFFD'};
            }
            
            if (!cursor)
                return char32_t{u'\uFFFD'};
        }
    }
    #endif

    //MARK:- UTF-32

    
    #ifndef __INTELLISENSE__
    template<>
    template<class InCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf32>::read(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        utf_codepoint_decoder<utf32> decoder;
        bool res = decoder.put(uint32_t(*cursor));
        ++cursor;
        return res ? char32_t{decoder.value()} : U'\uFFFD';
    }

    template<>
    template<class InCursor>
    char32_t utf32_input<utf32>::read_reversed(InCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        return utf32_input::read(cursor);
    }
    #endif
}

#endif 


