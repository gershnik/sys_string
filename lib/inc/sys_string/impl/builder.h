
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
    template<class Storage>
    void sys_string_builder_t<Storage>::append_one(impl_type & impl, char32_t c)
    {
        if constexpr (std::is_same_v<storage_type, char32_t>)
        {
            impl.push_back(c);
        }
        else
        {
            using converter = utf_converter<utf32, utf_encoding_of<storage_type>>;
            converter::convert(&c, &c + 1, std::back_inserter(impl));
        }
    }

    template<class Storage>
    auto sys_string_builder_t<Storage>::insert_one(impl_type & impl, typename impl_type::iterator where, char32_t c) ->
        typename impl_type::iterator
    {
        if constexpr (std::is_same_v<storage_type, char32_t>)
        {
            return impl.insert(where, c);
        }
        else
        {
            using converter = utf_converter<utf32, utf_encoding_of<storage_type>>;
            auto pos = where - std::ranges::begin(impl);
            converter::convert(&c, &c + 1, std::inserter(impl, where));
            return std::ranges::begin(impl) + pos;
        }
    }

    template<class Storage>
    template<std::ranges::input_range Range>
    requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
    inline void sys_string_builder_t<Storage>::append_many(impl_type & impl, const Range & range)
    {
        if constexpr(std::is_same_v<std::ranges::range_value_t<Range>, storage_type>)
        {
            if constexpr (std::ranges::contiguous_range<Range>)
            {
                impl.append(std::ranges::data(range), limit_size(std::ranges::size(range)));
            }
            else
            {
                for(storage_type c: range)
                    impl.push_back(c);
            }
        }
        else
        {
            using converter = utf_converter<utf_encoding_of<std::ranges::range_value_t<Range>>, utf_encoding_of<storage_type>>;
            converter::convert(range, std::back_inserter(impl));
        }
    }


    template<class Storage>
    template<std::ranges::input_range Range>
    requires(has_utf_encoding<std::ranges::range_value_t<Range>>)
    auto sys_string_builder_t<Storage>::insert_many(impl_type & impl, typename impl_type::iterator where, const Range & range) ->
    typename impl_type::iterator
    {
        if constexpr (std::is_same_v<storage_type, std::ranges::range_value_t<Range>>)
        {
            if constexpr (std::ranges::contiguous_range<Range>)
            {
                return impl.insert(where, std::ranges::data(range), limit_size(std::ranges::size(range)));
            }
            else
            {
                auto pos = where - std::ranges::begin(impl);
                for(storage_type c: range)
                    impl.push_back(c);
                return std::ranges::begin(impl) + pos;
            }
        }
        else
        {
            using converter = utf_converter<utf_encoding_of<std::ranges::range_value_t<Range>>, utf_encoding_of<storage_type>>;
            auto pos = where - std::ranges::begin(impl);
            converter::convert(range, std::inserter(impl, where));
            return std::ranges::begin(impl) + pos;
        }
    }

    

}
