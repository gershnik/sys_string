//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED
#define HEADER_SYS_STRING_GRAPHEME_VIEW_H_INCLUDED

#include <sys_string/utf_view.h>
#include <sys_string/impl/unicode/algorithms.h>

#include <iterator>
#include <ranges>

namespace sysstr
{
    namespace util
    {
        template<std::forward_iterator It, std::sentinel_for<It> EndIt>
        class grapheme_iterator
        {
        private:
            using iter_type = utf_iterator<utf32, It, EndIt, iter_direction::forward>;
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
                m_current(first, last),
                m_next(first, last)
            {
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
            }

            value_type operator*() const noexcept
                { return value_type{this->m_current.storage_current(), this->m_next.storage_current()}; }

            grapheme_iterator & operator++()
            {
                m_current = m_next;
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
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
                { return lhs.m_current == std::default_sentinel; }
            friend constexpr bool operator==(std::default_sentinel_t, const grapheme_iterator & rhs)
                { return rhs.m_current == std::default_sentinel; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }

            
        private:
            iter_type m_current;
            iter_type m_next;
        };

        template<std::input_iterator RawIt, std::sentinel_for<RawIt> RawEndIt>
        class grapheme_iterator<utf_iterator<utf32, RawIt, RawEndIt, iter_direction::forward>, std::default_sentinel_t>
        {
        private:
            using iter_type = utf_iterator<utf32, RawIt, RawEndIt, iter_direction::forward>;
        public:
            using difference_type = std::iter_difference_t<iter_type>;
            using size_type = std::make_unsigned_t<difference_type>;
            using value_type = std::ranges::subrange<iter_type, iter_type>;
            using reference = const value_type;
            using pointer = void;

            using iterator_category = std::forward_iterator_tag;
        public:
            grapheme_iterator() = default;

            grapheme_iterator(iter_type first, iter_type last):
                m_current(first, last),
                m_next(first, last)
            {
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
            }

            value_type operator*() const noexcept
                { return value_type{this->m_current, this->m_next}; }

            grapheme_iterator & operator++()
            {
                m_current = m_next;
                advance_to_grapheme_cluster_break(m_next, std::default_sentinel);
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
                { return lhs.m_current == std::default_sentinel; }
            friend constexpr bool operator==(std::default_sentinel_t, const grapheme_iterator & rhs)
                { return rhs.m_current == std::default_sentinel; }
            friend constexpr bool operator!=(const grapheme_iterator & lhs, std::default_sentinel_t rhs)
                { return !(lhs == rhs); }
            friend constexpr bool operator!=(std::default_sentinel_t lhs, const grapheme_iterator & rhs)
                { return !(lhs == rhs); }

            
        private:
            iter_type m_current;
            iter_type m_next;
        };
    }
    
    template<std::ranges::forward_range Range>
    requires std::ranges::view<Range>
    class grapheme_view
    {
    private:
        using range = Range;
        
        static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<range>>;
        
        using access_iterator = decltype(std::ranges::begin(std::declval<const range &>()));
        using access_sentinel = decltype(std::ranges::end(std::declval<const range &>()));
        // using access_reverse_iterator = std::conditional_t<is_reversible, 
        //                                                 decltype(std::ranges::rbegin(std::declval<range>())),
        //                                                 void>;
        // using access_reverse_sentinel = std::conditional_t<is_reversible, 
        //                                                 decltype(std::ranges::rend(std::declval<range>())),
        //                                                 void>;

    public:
        using iterator = util::grapheme_iterator<access_iterator, access_sentinel>;
        using const_iterator = iterator;
        // using reverse_iterator = std::conditional_t<is_reversible, 
        //                                             utf_iterator<Enc, access_reverse_iterator, access_reverse_sentinel, iter_direction::backward>,
        //                                             void>;
        // using const_reverse_iterator = reverse_iterator;
        
        using value_type = typename iterator::value_type;
        using reference = typename iterator::reference;
        using const_reference = reference;
        using pointer = typename iterator::pointer;
        using const_pointer = pointer;

        // static const bool borrowed = (ViewType == byref) || 
        //                                 (ViewType == byval && std::ranges::borrowed_range<range>);
        
    public:
        grapheme_view(const Range & src) noexcept(noexcept(range(src))) :
            m_src(src)
        {}

        grapheme_view(Range && src) noexcept(noexcept(range(std::move<Range>(src)))) :
            m_src(std::move<Range>(src))
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
        // SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const requires(is_reversible)
        //     { return reverse_iterator(std::ranges::rbegin(range_ref(m_src)), std::ranges::rend(range_ref(m_src))); }
        // SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const requires(is_reversible)
        //     { return std::default_sentinel; }
        // SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const requires(is_reversible)
        //     { return rbegin(); }
        // SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const requires(is_reversible)
        //     { return rend(); }

        // reverse_iterator reverse(iterator it) const requires(is_reversible)
        //     { return reverse_iterator(it, std::ranges::rend(range_ref(m_src))); }

        // iterator reverse(reverse_iterator it) const requires(is_reversible)
        //     { return iterator(it, std::ranges::end(range_ref(m_src))); }
        
        // template<class Func>
        // decltype(auto) each(Func func) const
        //     { return utf_converter<source_encoding, Enc>::for_each_converted(range_ref(), func); }
        
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
