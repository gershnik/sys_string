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
    template<class Container>
    struct utf_access_traits;
    
    template<class T, class Container>
    concept utf_access_traits_t = requires(const Container & cont)
    {
        typename T::stored_reference;
        
        { T::enable_view } -> std::convertible_to<bool>;
        { T::enable_borrowed_range } -> std::convertible_to<bool>;
        { typename T::stored_reference(T::adapt(cont)) };
        { T::access(std::declval<typename T::stored_reference>()) } -> std::ranges::bidirectional_range;
    } && (T::enable_view || !T::enable_view) && (T::enable_borrowed_range || !T::enable_borrowed_range);

    template<utf_encoding Enc, class Container, utf_access_traits_t<Container> Traits = utf_access_traits<Container>>
    class utf_view;
    
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
            requires(Direction == iter_direction::forward && reverse_iterator_of<OtherIt, It>)
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
            requires(Direction == iter_direction::backward && reverse_iterator_of<It, OtherIt>)
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
            requires(Direction == iter_direction::forward && reverse_iterator_of<OtherIt, It>)
            utf_iterator(const utf_iterator<utf32, OtherIt, OtherEndIt, iter_direction::backward> & rev,
                         EndIt last):
                utf_iterator(rev.storage_current().base(), last)
            {}

            template<std::input_iterator OtherIt, std::sentinel_for<OtherIt> OtherEndIt>
            requires(Direction == iter_direction::backward && reverse_iterator_of<It, OtherIt>)
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
    
    template<std::ranges::random_access_range Container>
    struct utf_access_traits<Container>
    {
        using stored_reference = std::add_pointer_t<std::remove_reference_t<Container>>;
        
        static constexpr bool enable_view = true;
        static constexpr bool enable_borrowed_range = true;
        
        static decltype(auto) adapt(std::add_lvalue_reference_t<Container> src) noexcept
            { return &src; }
        static std::add_lvalue_reference_t<Container> access(stored_reference ptr) noexcept
            { return *ptr; }
    };

    
    template<utf_encoding Enc, class Container, utf_access_traits_t<Container> Traits>
    class utf_view
    {
    private:
        using stored_reference = typename Traits::stored_reference;
        
        static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<decltype(Traits::access(std::declval<stored_reference>()))>>;
        
        using access_iterator = decltype(std::begin(Traits::access(std::declval<stored_reference>())));
        using access_sentinel = decltype(std::end(Traits::access(std::declval<stored_reference>())));
        using access_reverse_iterator = decltype(std::rbegin(Traits::access(std::declval<stored_reference>())));
        using access_reverse_sentinel = decltype(std::rend(Traits::access(std::declval<stored_reference>())));
    public:
        using iterator = util::utf_iterator<Enc, access_iterator, access_sentinel, util::iter_direction::forward>;
        using const_iterator = iterator;
        using reverse_iterator = util::utf_iterator<Enc, access_reverse_iterator, access_reverse_sentinel, util::iter_direction::backward>;
        using const_reverse_iterator = reverse_iterator;
        
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using const_reference = reference;
        using pointer = typename iterator::pointer;
        using const_pointer = pointer;
        
        using iter_direction = util::iter_direction;
        
        static constexpr auto forward = iter_direction::forward;
        static constexpr auto backward = iter_direction::backward;
    public:
        utf_view(const Container & src) noexcept(noexcept(stored_reference(Traits::adapt(src)))) :
            m_ref(Traits::adapt(src))
        {}
        SYS_STRING_FORCE_INLINE iterator begin() const
            { return iterator(std::begin(Traits::access(this->m_ref)), std::end(Traits::access(this->m_ref))); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t end() const
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_iterator cbegin() const
            { return begin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t cend() const
            { return end(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const
            { return reverse_iterator(std::rbegin(Traits::access(this->m_ref)), std::rend(Traits::access(this->m_ref))); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const
            { return rbegin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const
            { return rend(); }

        reverse_iterator reverse(iterator it) const
            { return reverse_iterator(it, std::rend(Traits::access(this->m_ref))); }

        iterator reverse(reverse_iterator it) const
            { return iterator(it, std::end(Traits::access(this->m_ref))); }
        
        template<class Func>
        decltype(auto) each(Func func) const
        {
            return utf_converter<source_encoding, Enc>::for_each_converted(Traits::access(this->m_ref), func);
        }
        
    private:
        stored_reference m_ref;
    };
}

namespace std::ranges {
    template<sysstr::utf_encoding Enc, class Container, sysstr::utf_access_traits_t<Container> Traits>
    constexpr bool enable_view<sysstr::utf_view<Enc, Container, Traits>> = Traits::enable_view;
    
    template<sysstr::utf_encoding Enc, class Container, sysstr::utf_access_traits_t<Container> Traits>
    constexpr bool enable_borrowed_range<sysstr::utf_view<Enc, Container, Traits>> = Traits::enable_borrowed_range;
}


#endif
