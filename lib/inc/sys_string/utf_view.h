//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UTF_VIEW_H_INCLUDED
#define HEADER_SYS_STRING_UTF_VIEW_H_INCLUDED

#include <sys_string/impl/unicode/utf_util.h>
#include <sys_string/impl/util/iter_util.h>

#include <array>
#include <iterator>
#include <ranges>

namespace sysstr
{
    namespace util
    {
        template<iter_direction Direction, std::input_iterator It, std::sentinel_for<It> EndIt>
        SYS_STRING_FORCE_INLINE
        char32_t current_utf32(It & it, EndIt last) 
        {
            constexpr auto encoding = utf_encoding_of<typename std::iterator_traits<It>::value_type>;
            if constexpr (Direction == iter_direction::forward)
                return utf32_input<encoding>::read(it, last);
            else
                return utf32_input<encoding>::read_reversed(it, last);
        }

        template<utf_encoding OutputEnc, std::input_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction>
        class utf_iterator
        {
            template<utf_encoding E, std::input_iterator X, std::sentinel_for<X> EndX, iter_direction D>
            friend class utf_iterator;
        public:
            using difference_type = typename std::iterator_traits<It>::difference_type;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = utf_char_of<OutputEnc>;
            using reference = const value_type;
            using pointer = void;
            
            using iterator_category = std::forward_iterator_tag;
            
            static constexpr bool is_forward = (Direction == iter_direction::forward);
            static constexpr bool is_reverse = !is_forward;
        public:
            constexpr utf_iterator() = default;
            
            SYS_STRING_FORCE_INLINE
            constexpr utf_iterator(It first, EndIt last) noexcept(std::is_nothrow_move_constructible_v<It> &&
                                                                std::is_nothrow_move_constructible_v<EndIt>):
                m_current(std::move(first)),
                m_next(m_current),
                m_last(std::move(last))
            {
                this->load_next();
            }

            template<std::input_iterator OtherIt, std::sentinel_for<OtherIt> OtherEndIt>
            requires(Direction == iter_direction::forward && ranges::reverse_iterator_for<OtherIt, It>)
            utf_iterator(const utf_iterator<OutputEnc, OtherIt, OtherEndIt, iter_direction::backward> & rev,
                         EndIt last):
                m_current(rev.storage_next().base()),
                m_next(rev.storage_current().base()),
                m_last(std::move(last)),
                m_encoder(rev.m_encoder),
                m_char_idx(rev.m_char_idx)
            {
                if (m_current != m_last)
                    ++*this;
            }

            template<std::input_iterator OtherIt, std::sentinel_for<OtherIt> OtherEndIt>
            requires(Direction == iter_direction::backward && ranges::reverse_iterator_for<It, OtherIt>)
            utf_iterator(const utf_iterator<OutputEnc, OtherIt, OtherEndIt, iter_direction::forward> & fwd,
                         EndIt last):
                m_current(fwd.storage_next()),
                m_next(fwd.storage_current()),
                m_last(std::move(last)),
                m_encoder(fwd.m_encoder),
                m_char_idx(fwd.m_char_idx)
            {
                if (m_current != m_last)
                    ++*this;
            }

            constexpr value_type operator*() const noexcept
                { return this->m_encoder.begin()[this->m_char_idx]; }

            explicit constexpr operator bool() const noexcept
                { return this->m_char_idx != this->m_encoder.size(); }

            SYS_STRING_FORCE_INLINE
            constexpr utf_iterator & operator++()
            {
                if constexpr (is_forward)
                {
                    if (++this->m_char_idx < this->m_encoder.size())
                        return *this;
                }
                else
                {
                    if (this->m_char_idx > 0)
                    {
                        --this->m_char_idx;
                        return *this;
                    }
                }

                this->m_current = this->m_next;
                this->load_next();
                return *this;
            }

            constexpr utf_iterator operator++(int)
            {
                utf_iterator ret = *this;
                ++(*this);
                return ret;
            }
            
            friend constexpr bool operator==(const utf_iterator & lhs, const utf_iterator & rhs)
                { return lhs.m_char_idx == rhs.m_char_idx && lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const utf_iterator & lhs, const utf_iterator & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const utf_iterator & lhs, std::default_sentinel_t)
                { return !lhs; }
            friend constexpr bool operator==(std::default_sentinel_t, const utf_iterator & rhs)
                { return !rhs; }
            friend constexpr bool operator!=(const utf_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const utf_iterator & rhs)
                { return !(lhs == rhs); }
            
            
            const It storage_current() const
                { return this->m_current; }
            const It storage_next() const
                { return this->m_next; }
            EndIt storage_last() const
                { return this->m_last; }
            
        private:
            SYS_STRING_FORCE_INLINE
            constexpr void load_next()
            {
                if (this->m_next != this->m_last)
                {
                    auto c = current_utf32<Direction>(this->m_next, this->m_last);
                    this->m_encoder.put(c);
                    if constexpr (is_forward)
                    {
                        this->m_char_idx = 0;
                    }
                    else
                    {
                        this->m_char_idx = uint8_t(this->m_encoder.size() - 1);
                    }
                }
                else
                {
                    this->m_encoder.clear();
                    this->m_char_idx = 0;
                }
            }
            
        private:
            It m_current{};
            It m_next{};
            [[no_unique_address]] EndIt m_last{};
            utf_codepoint_encoder<OutputEnc, false> m_encoder;
            uint8_t m_char_idx = 0;
        };

        template<std::input_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction>
        class utf_iterator<utf32, It, EndIt, Direction>
        {
        public:
            using difference_type = typename std::iterator_traits<It>::difference_type;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = char32_t;
            using reference = const char32_t;
            using pointer = void;
            
            using iterator_category = std::forward_iterator_tag;
            
            static constexpr bool is_forward = (Direction == iter_direction::forward);
            static constexpr bool is_reverse = !is_forward;
        public:
            constexpr utf_iterator() = default;

            SYS_STRING_FORCE_INLINE
            constexpr utf_iterator(It first, EndIt last) noexcept(std::is_nothrow_move_constructible_v<It> &&
                                                                  std::is_nothrow_move_constructible_v<EndIt>) :
                m_current(std::move(first)),
                m_next(m_current),
                m_last(std::move(last))
            {
                this->load_next();
            }

            template<std::input_iterator OtherIt, std::sentinel_for<OtherIt> OtherEndIt>
            requires(Direction == iter_direction::forward && ranges::reverse_iterator_for<OtherIt, It>)
            utf_iterator(const utf_iterator<utf32, OtherIt, OtherEndIt, iter_direction::backward> & rev,
                         EndIt last):
                utf_iterator(rev.storage_current().base(), last)
            {}

            template<std::input_iterator OtherIt, std::sentinel_for<OtherIt> OtherEndIt>
            requires(Direction == iter_direction::backward && ranges::reverse_iterator_for<It, OtherIt>)
            utf_iterator(const utf_iterator<utf32, OtherIt, OtherEndIt, iter_direction::forward> & fwd,
                         EndIt last):
                utf_iterator(It(fwd.storage_current()), last)
            {}
            
            constexpr value_type operator*() const noexcept
                { return this->m_value; }

            SYS_STRING_FORCE_INLINE
            constexpr utf_iterator & operator++()
            {
                this->m_current = this->m_next;
                this->load_next();
                return *this;
            }

            SYS_STRING_FORCE_INLINE
            constexpr utf_iterator operator++(int)
            {
                utf_iterator ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const utf_iterator & lhs, const utf_iterator & rhs)
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const utf_iterator & lhs, const utf_iterator & rhs)
                { return lhs.m_current != rhs.m_current; }
            
            friend constexpr bool operator==(const utf_iterator & lhs, std::default_sentinel_t)
                { return lhs.m_current == lhs.m_last; }
            friend constexpr bool operator==(std::default_sentinel_t, const utf_iterator & rhs)
                { return rhs.m_current == rhs.m_last; }
            friend constexpr bool operator!=(const utf_iterator & lhs, std::default_sentinel_t)
                { return lhs.m_current != lhs.m_last; }
            friend constexpr bool operator!=(std::default_sentinel_t, const utf_iterator & rhs)
                { return rhs.m_current != rhs.m_last; }
            
            It storage_current() const
                { return this->m_current; }
            It storage_next() const
                { return this->m_next; }
            EndIt storage_last() const
                { return this->m_last; }
            
        private:
            SYS_STRING_FORCE_INLINE
            void load_next()
            {
                if (this->m_next != this->m_last)
                    this->m_value = current_utf32<Direction>(this->m_next, this->m_last);
            }
        private:
            It m_current{};
            It m_next{};
            [[no_unique_address]] EndIt m_last{};
            value_type m_value = 0;
        };
    }

    template<class T, class Container>
    concept utf_view_adapter = requires(const Container & cont)
    {
        { T::adapt(cont) } -> std::semiregular;
        { *T::adapt(cont) } -> std::ranges::input_range;
    };

    template<std::ranges::input_range Range>
    struct range_utf_view_adapter
    {
        static auto adapt(const std::add_const_t<std::remove_reference_t<Range>> & src) noexcept
            { return std::addressof(src); }
    };

    template<class T> struct utf_view_adapter_for 
        { using type = range_utf_view_adapter<T>; };

    template<utf_encoding Enc, class Container>
    class utf_view
    {
    private:
        using adapter = typename utf_view_adapter_for<Container>::type;
        static_assert(utf_view_adapter<adapter, Container>);
        
        using range_pointer = decltype(adapter::adapt(std::declval<const Container &>()));
        using range = decltype(*std::declval<range_pointer>());

        static constexpr bool is_reversible = ranges::reverse_traversable_range<std::remove_reference_t<range>>;
        static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<range>>;
        
        using access_iterator = decltype(std::begin(*std::declval<range_pointer>()));
        using access_sentinel = decltype(std::end(*std::declval<range_pointer>()));
        using access_reverse_iterator = std::conditional_t<is_reversible, 
                                                        decltype(std::rbegin(std::declval<range>())),
                                                        void>;
        using access_reverse_sentinel = std::conditional_t<is_reversible, 
                                                        decltype(std::rend(std::declval<range>())),
                                                        void>;

    public:
        using iterator = util::utf_iterator<Enc, access_iterator, access_sentinel, util::iter_direction::forward>;
        using const_iterator = iterator;
        using reverse_iterator = std::conditional_t<is_reversible, 
                                                    util::utf_iterator<Enc, access_reverse_iterator, access_reverse_sentinel, util::iter_direction::backward>,
                                                    void>;
        using const_reverse_iterator = reverse_iterator;
        
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using const_reference = reference;
        using pointer = typename iterator::pointer;
        using const_pointer = pointer;
        
    public:
        utf_view(const Container & src) noexcept(noexcept(range_pointer(adapter::adapt(src)))) :
            m_ptr(adapter::adapt(src))
        {}
        SYS_STRING_FORCE_INLINE iterator begin() const
            { return iterator(std::begin(*m_ptr), std::end(*m_ptr)); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t end() const
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_iterator cbegin() const
            { return begin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t cend() const
            { return end(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const requires(is_reversible)
            { return reverse_iterator(std::rbegin(*m_ptr), std::rend(*m_ptr)); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const requires(is_reversible)
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const requires(is_reversible)
            { return rbegin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const requires(is_reversible)
            { return rend(); }

        reverse_iterator reverse(iterator it) const requires(is_reversible)
            { return reverse_iterator(it, std::rend(*m_ptr)); }

        iterator reverse(reverse_iterator it) const requires(is_reversible)
            { return iterator(it, std::end(*m_ptr)); }
        
        template<class Func>
        decltype(auto) each(Func func) const
        {
            return utf_converter<source_encoding, Enc>::for_each_converted(*m_ptr, func);
        }
        
    private:
        range_pointer m_ptr;
    };

    template<utf_encoding Enc>
    struct as_utf_func 
    #if __cpp_lib_ranges >= 202202L
        : 
        #if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < 190000
            //libc++ lies about __cpp_lib_ranges
            public std::__range_adaptor_closure<as_utf_func<Enc>>
        #else
            public std::ranges::range_adaptor_closure<as_utf_func<Enc>>
        #endif
    #endif
    {
        template <class Range>
        [[nodiscard]] constexpr auto operator()(Range && range) const
            noexcept(noexcept(utf_view<Enc, std::remove_reference_t<Range>>(std::forward<Range>(range))))
                    -> decltype(utf_view<Enc, std::remove_reference_t<Range>>(std::forward<Range>(range))) 
                        { return utf_view<Enc, std::remove_reference_t<Range>>(std::forward<Range>(range)); }

    };

    template<utf_encoding Enc>
    inline constexpr auto as_utf = as_utf_func<Enc>{};
    inline constexpr auto as_utf8 = as_utf_func<utf8>{};
    inline constexpr auto as_utf16 = as_utf_func<utf16>{};
    inline constexpr auto as_utf32 = as_utf_func<utf32>{};

}

namespace std::ranges {
    template<sysstr::utf_encoding Enc, class Container>
    constexpr bool enable_view<sysstr::utf_view<Enc, Container>> = true;
    
    template<sysstr::utf_encoding Enc, class Container>
    constexpr bool enable_borrowed_range<sysstr::utf_view<Enc, Container>> = true;
}


#endif
