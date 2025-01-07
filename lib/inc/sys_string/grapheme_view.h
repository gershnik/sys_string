//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED
#define HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED

#include <sys_string/impl/unicode/algorithms.h>

#include <iterator>
#include <ranges>

namespace sysstr
{
    namespace util
    {
        template<std::forward_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction>
        class grapheme_iterator
        {
        private:
            static constexpr utf_encoding source_encoding = utf_encoding_of<std::iter_value_t<It>>;
            static constexpr bool is_forward = (Direction == iter_direction::forward);
            static constexpr bool is_reverse = !is_forward;
        public:
            using difference_type = std::iter_difference_t<It>;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = std::ranges::subrange<It, It>;
            using reference = const value_type;
            using pointer = void;

            using iterator_category = std::forward_iterator_tag;
        public:
            grapheme_iterator() = default;

            grapheme_iterator(It first, EndIt last):
                m_current(first),
                m_next(first),
                m_last(last),
                m_finder(this->m_next, this->m_last)
            {}

            value_type operator*() const noexcept
                { return value_type{this->m_current, this->m_next}; }

            grapheme_iterator & operator++()
            {
                this->m_current = this->m_next;
                this->m_finder(this->m_next, this->m_last);
                return *this;
            }

            grapheme_iterator operator++(int)
            {
                grapheme_iterator ret = *this;
                ++(*this);
                return ret;
            }

            friend constexpr bool operator==(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return lhs.m_current == rhs.m_current; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }
            
            friend constexpr bool operator==(const grapheme_iterator & lhs, std::default_sentinel_t)
                { return lhs.m_current == lhs.m_last; }
            friend constexpr bool operator==(std::default_sentinel_t, const grapheme_iterator & rhs)
                { return rhs.m_current == rhs.m_last; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }

        private:
            It m_current;
            It m_next;
            SYS_STRING_NO_UNIQUE_ADDRESS EndIt m_last;
            std::conditional_t<is_forward, grapheme_cluster_break_finder, grapheme_cluster_break_reverse_finder> m_finder;
        };
    }
    
    template<std::ranges::forward_range Range>
    requires std::ranges::view<Range>
    class grapheme_view
    {
    private:
        using range = Range;
        
        static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<range>>;
        static constexpr bool is_reversible = ranges::reverse_traversable_range<std::remove_reference_t<range>>;
        
        using access_iterator = decltype(std::ranges::begin(std::declval<const range &>()));
        using access_sentinel = decltype(std::ranges::end(std::declval<const range &>()));
        using access_reverse_iterator = std::conditional_t<is_reversible, 
                                                           decltype(std::ranges::rbegin(std::declval<const range &>())),
                                                           void>;
        using access_reverse_sentinel = std::conditional_t<is_reversible, 
                                                           decltype(std::ranges::rend(std::declval<const range &>())),
                                                           void>;

    public:
        using iterator = util::grapheme_iterator<access_iterator, access_sentinel, util::iter_direction::forward>;
        using const_iterator = iterator;
        using reverse_iterator = std::conditional_t<is_reversible, 
                                                    util::grapheme_iterator<access_reverse_iterator, access_reverse_sentinel, util::iter_direction::backward>,
                                                    void>;
        using const_reverse_iterator = reverse_iterator;
        
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using const_reference = reference;
        using pointer = typename iterator::pointer;
        using const_pointer = pointer;

    public:
        grapheme_view(const Range & src) noexcept(noexcept(range(src))) :
            m_src(src)
        {}

        grapheme_view(Range && src) noexcept(noexcept(range(std::move(src)))) :
            m_src(std::move(src))
        {}

        template<class... Args>
        grapheme_view(Args && ...args) noexcept(noexcept(range(std::forward<Args>(args)...))) 
        requires(std::is_constructible_v<range, Args...>) :
            m_src(std::forward<Args>(args)...)
        {}


        SYS_STRING_FORCE_INLINE iterator begin() const
            { return iterator(std::ranges::begin(m_src), std::ranges::end(m_src)); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t end() const
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_iterator cbegin() const
            { return begin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t cend() const
            { return end(); }
        SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const requires(is_reversible)
            { return reverse_iterator(std::ranges::rbegin(m_src), std::ranges::rend(m_src)); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const requires(is_reversible)
            { return std::default_sentinel; }
        SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const requires(is_reversible)
            { return rbegin(); }
        SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const requires(is_reversible)
            { return rend(); }

        bool empty() const
            { return std::ranges::empty(m_src); }
        explicit operator bool() const
            { return !std::ranges::empty(m_src); }
        decltype(auto) front() const
            { return *this->begin();}
        decltype(auto) back() const requires(is_reversible)
            { return *this->rbegin();}

        reverse_iterator reverse(iterator it) const requires(is_reversible)
            { return reverse_iterator(it, std::ranges::rend(m_src)); }

        iterator reverse(reverse_iterator it) const requires(is_reversible)
            { return iterator(it, std::ranges::end(m_src)); }
        
    private:
        range m_src;
    };
    template <class Range>
    grapheme_view(Range &&) -> grapheme_view<std::views::all_t<Range>>;

    struct graphemes_func 
    #if __cpp_lib_ranges >= 202202L
        : 
        #if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < 190000
            //libc++ lies about __cpp_lib_ranges
            public std::__range_adaptor_closure<graphemes_func>
        #else
            public std::ranges::range_adaptor_closure<graphemes_func>
        #endif
    #endif
    {
        template <class Range>
        [[nodiscard]] constexpr auto operator()(Range && range) const
            noexcept(noexcept(grapheme_view(std::forward<Range>(range))))
                  -> decltype(grapheme_view(std::forward<Range>(range))) {
                       return grapheme_view(std::forward<Range>(range));
        }
    };

    inline constexpr auto graphemes = graphemes_func{};
}

namespace std::ranges {
    template<std::ranges::forward_range Range>
    constexpr bool enable_view<sysstr::grapheme_view<Range>> = true;
    
    template<std::ranges::forward_range Range>
    constexpr bool enable_borrowed_range<sysstr::grapheme_view<Range>> = std::ranges::borrowed_range<Range>;
}

#endif
