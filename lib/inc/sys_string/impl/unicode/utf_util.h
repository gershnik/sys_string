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
        
        template<class InIt>
        static void reading(InIt first, InIt last) noexcept(noexcept(*first++) && noexcept(first != last));
        template<class OutIt>
        static void writing(OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<To>()));
        template<class Func>
        static void applying(Func func) noexcept(noexcept(func(utf_char_of<To>())));
    public:
        template<class InIt, class Func>
        static Func for_each_converted(InIt first, InIt last, Func func) noexcept(noexcept(reading(first, last)) && noexcept(applying(func)));
        
        
        template<class InIt, class OutIt>
        SYS_STRING_FORCE_INLINE
        static OutIt convert(InIt first, InIt last, OutIt dest) noexcept(noexcept(reading(first, last)) && noexcept(writing(dest)))
        {
            for_each_converted(first, last, [&](auto c) { *dest++ = c; });
            return dest;
        }
        
        template<class InIt>
        SYS_STRING_FORCE_INLINE
        static size_t converted_length(InIt first, InIt last) noexcept(noexcept(reading(first, last)))
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
        template<class FwdCursor>
        static void reading(FwdCursor & cursor) noexcept(noexcept(*cursor) && noexcept(++cursor) && noexcept(bool(cursor)));
        template<class InIt>
        static void reading(InIt first, InIt last) noexcept(noexcept(*first++) && noexcept(first != last));

    public:
        template<class FwdCursor>
        static char32_t read(FwdCursor & cursor) noexcept(noexcept(reading(cursor)));

        template<class FwdCursor>
        static char32_t read_reversed(FwdCursor & cursor) noexcept(noexcept(reading(cursor)));

        template<class InIt, class Sink>
        static Sink read(InIt first, InIt last, Sink sink) noexcept(noexcept(reading(first,last)) && noexcept(sink(char32_t())));
    };

    template<utf_encoding To>
    struct utf32_output
    {
        template<class Sink>
        struct write
        {
            write(Sink s) : sink(s) {}
            
            SYS_STRING_FORCE_INLINE
            constexpr void operator()(char32_t value) noexcept(noexcept(sink(utf_char_of<To>())))
            {
                utf_codepoint_encoder<To, false> encoder;
                encoder.put(value);
                for(auto first = encoder.begin(), last = encoder.end(); first != last; ++first)
                    sink(*first);
            }
                
            Sink sink;
        };
        template<class Sink> write(Sink sink) -> write<Sink>;
    };

    template<>
    struct utf32_output<utf32>
    {
        template<class Sink>
        struct write
        {
            write(Sink s) : sink(s) {}
            
            constexpr void operator()(char32_t value) noexcept(noexcept(sink(value)))
            {
                sink(value);
            }
                
            Sink sink;
        };
        template<class Sink> write(Sink sink) -> write<Sink>;
    };

    template<utf_encoding From, utf_encoding To>
    template<class InIt, class Func>
    SYS_STRING_FORCE_INLINE
    Func utf_converter<From, To>::for_each_converted(InIt first, InIt last, Func func) noexcept(noexcept(reading(first, last)) && noexcept(applying(func)))
    {
        return utf32_input<From>::read(first, last, typename utf32_output<To>::write(func)).sink;
    }


    //MARK:- UTF-16

    template<>
    template<class InIt, class Sink>
    SYS_STRING_FORCE_INLINE
    Sink utf32_input<utf16>::read(InIt first, InIt last, Sink sink) noexcept(noexcept(reading(first, last)) && noexcept(sink(char32_t())))
    {
        utf_codepoint_decoder<utf16> decoder;
        while(first != last)
        {
            decoder.put(uint16_t(*first));
            ++first;
            if (decoder.done())
            {
                sink(char32_t{decoder.value()});
                continue;
            }
            
            if (decoder.error())
            {
                sink(char32_t{U'\uFFFD'});
                continue;
            }
            
            if (first == last)
            {
                sink(char32_t{U'\uFFFD'});
                return sink;
            }
            
            decoder.put(uint16_t(*first));
            if (!decoder.done())
            {
                sink(char32_t{U'\uFFFD'});
                continue;
            }
            ++first;

            sink(char32_t{decoder.value()});
        }
        return sink;
    }

    #ifndef __INTELLISENSE__
    template<>
    template<class FwdCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf16>::read(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
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
    template<class FwdCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf16>::read_reversed(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
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

    template<>
    template<class InIt, class Sink>
    SYS_STRING_FORCE_INLINE
    Sink utf32_input<utf8>::read(InIt first, InIt last, Sink sink) noexcept(noexcept(reading(first, last)) && noexcept(sink(char32_t())))
    {
        utf_codepoint_decoder<utf8> decoder;
        
        while(first != last)
        {
            uint8_t byte = uint8_t(*first);
            ++first;
            if (byte <= 0x7f)
            {
                sink(char32_t{byte});
                continue;
            }
            
            decoder.put(byte);
            if (decoder.error())
            {
                sink(char32_t{U'\uFFFD'});
                continue;
            }
            
            if (first == last)
            {
                sink(char32_t{U'\uFFFD'});
                return sink;
            }
            
            for ( ; ; )
            {
                byte = uint8_t(*first);
                decoder.put(byte);
                if (decoder.error())
                {
                    sink(char32_t{U'\uFFFD'});
                    break;
                }
                ++first;
                if (decoder.done())
                {
                    sink(char32_t{decoder.value()});
                    break;
                }
                if (first == last)
                {
                    sink(char32_t{U'\uFFFD'});
                    return sink;
                }
            }
        }
        
        return sink;
    }

    #ifndef __INTELLISENSE__
    template<>
    template<class FwdCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf8>::read(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
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
    template<class FwdCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf8>::read_reversed(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
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

    template<>
    template<class InIt, class Sink>
    SYS_STRING_FORCE_INLINE
    Sink utf32_input<utf32>::read(InIt first, InIt last, Sink sink) noexcept(noexcept(reading(first, last)) && noexcept(sink(char32_t())))
    {
        utf_codepoint_decoder<utf32> decoder;
        for( ; first != last; ++first)
        {
            if (decoder.put(uint32_t(*first)))
                sink(decoder.value());
            else
                sink(U'\uFFFD');
        }
        return sink;
    }

    #ifndef __INTELLISENSE__
    template<>
    template<class FwdCursor>
    SYS_STRING_FORCE_INLINE
    char32_t utf32_input<utf32>::read(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        utf_codepoint_decoder<utf32> decoder;
        bool res = decoder.put(uint32_t(*cursor));
        ++cursor;
        return res ? char32_t{decoder.value()} : U'\uFFFD';
    }

    template<>
    template<class FwdCursor>
    char32_t utf32_input<utf32>::read_reversed(FwdCursor & cursor) noexcept(noexcept(reading(cursor)))
    {
        return utf32_input::read(cursor);
    }
    #endif
}

#endif 


