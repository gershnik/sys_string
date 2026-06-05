
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

namespace sysstr::util
{
    template<class Storage>
    class size_accumulator {
    public:

        void add(size_t s) noexcept
        {
            m_overflow = m_overflow || (size_t(Storage::max_size) - m_size < s);
            m_size += s;
        }

        bool overflow() const noexcept
            { return m_overflow; }

        auto value() const noexcept
            { return typename sys_string_t<Storage>::size_type(m_size); }
    private:
        size_t m_size = 0;
        bool m_overflow = false;
    };

    template<class Storage, class T>
    class addend;

    template<class Storage>
    class addend<Storage, sys_string_t<Storage>>
    {
    public:
        addend(const sys_string_t<Storage> & str) noexcept:
            m_val(str)
        {}
        
        void add_size(size_accumulator<Storage> & acc) const noexcept 
            { acc.add(size_t(this->m_val.storage_size())); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(this->m_val); }
    private:
        const sys_string_t<Storage> & m_val;
    };

    template<class Storage>
    class addend<Storage, char32_t>
    {
    public:
        addend(char32_t c) noexcept
        {
            if (!m_encoded.put(c))
                m_encoded.put(U'\uFFFD');
        }
        
        void add_size(size_accumulator<Storage> & acc) const noexcept 
            { acc.add(size_t(this->m_encoded.end() - this->m_encoded.begin())); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
        { 
            builder.chars().append(this->m_encoded.begin(), 
                                   typename sys_string_t<Storage>::size_type(this->m_encoded.end() - this->m_encoded.begin())); 
        }
    private:
        utf_codepoint_encoder<utf_encoding_of<typename sys_string_t<Storage>::storage_type>, true> m_encoded;
    };

    template<class Storage, class T>
    requires(
        std::is_pointer_v<std::decay_t<T>> &&
        has_utf_encoding<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>
    )
    class addend<Storage, T>
    {
    private: 
        using Char = std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>;
    public:
        addend(const Char * ptr) noexcept:
            m_view(ptr)
        {}
        
        void add_size(size_accumulator<Storage> & acc) const noexcept
            { acc.add(m_view.size()); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(m_view); }
    private:
        std::basic_string_view<Char> m_view;
    };

    template<class Storage, std::ranges::forward_range Range>
    requires(
        !std::is_pointer_v<std::decay_t<Range>> &&
        has_utf_encoding<std::ranges::range_value_t<Range>>)
    class addend<Storage, Range>
    {
    public:
        addend(const Range & range) noexcept:
            m_range(range)
        {}
        
        void add_size(size_accumulator<Storage> & acc) const
        { 
            if constexpr (std::is_same_v<std::ranges::range_value_t<Range>, typename sys_string_t<Storage>::storage_type>)
            {
                if constexpr (std::ranges::sized_range<Range>)
                {
                    acc.add(std::ranges::size(this->m_range));
                }
                else
                {
                    size_t count = 0;
                    for(auto & x: this->m_range)
                        ++count;
                    acc.add(count);
                }
            }
            else
            {
                using converter = utf_converter<utf_encoding_of<std::ranges::range_value_t<Range>>, 
                                            utf_encoding_of<typename sys_string_t<Storage>::storage_type>>;
                acc.add(converter::converted_length(this->m_range));
            }
        }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { builder.append(this->m_range); }
    private:
        const Range & m_range;
    };

    template<class Storage, class First, class Second>
    class addend<Storage, addition<Storage, First, Second>>
    {
    public:
        addend(addition<Storage, First, Second> && val) noexcept:
            m_val(std::move(val))
        {}
        
        void add_size(size_accumulator<Storage> & acc) const noexcept
            { this->m_val.add_size(acc); }
        
        auto append_to(sys_string_builder_t<Storage> & builder) const -> void
            { this->m_val.append_to(builder); }
    private:
        addition<Storage, First, Second> && m_val;
    };

    template<class Storage, class First, class Second>
    class addition
    {
    public:
        addition(const First & left, const Second & right) noexcept:
            m_first{left},
            m_second{right}
        {}
        addition(First && left, const Second & right) noexcept:
            m_first{std::move(left)},
            m_second{right}
        {}
        addition(const First & left, Second && right) noexcept:
            m_first{left},
            m_second{std::move(right)}
        {}
        addition(First && left, Second && right) noexcept:
            m_first{std::move(left)},
            m_second{std::move(right)}
        {}
        
        template <addable<Storage> T>
        friend auto operator+(addition && lhs, const T & rhs) noexcept
            { return addition<Storage, addition, T>(std::move(lhs), rhs); }
        
        template <addable<Storage> T>
        friend auto operator+(const T & lhs, addition && rhs) noexcept
            { return addition<Storage, T, addition>(lhs, std::move(rhs)); }
        
        template<class RHSFirst, class RHSSecond>
        friend auto operator+(addition && lhs, addition<Storage, RHSFirst, RHSSecond> && rhs) noexcept
            { return addition<Storage, addition, addition<Storage, RHSFirst, RHSSecond>>(std::move(lhs), std::move(rhs)); }
        
        operator sys_string_t<Storage>() const &&
        {
            size_accumulator<Storage> acc;
            this->add_size(acc);
            if (acc.overflow())
                throw std::bad_alloc();
            sys_string_builder_t<Storage> builder;
            builder.reserve_storage(acc.value());
            this->append_to(builder);
            return builder.build();
        }

        operator sys_string_t<Storage>() const & = SYS_STRING_DELETE_REASON(
            "you must convert the result of sys_string_t addition to sys_string_t. Do not assign it to auto and carry around");
        
        void add_size(size_accumulator<Storage> & acc) const noexcept
        { 
            this->m_first.add_size(acc);
            this->m_second.add_size(acc);
        }
        
        void append_to(sys_string_builder_t<Storage> & builder) const
        {
            this->m_first.append_to(builder);
            this->m_second.append_to(builder);
        }
    private:
        addend<Storage, First> m_first;
        addend<Storage, Second> m_second;
    };
}
