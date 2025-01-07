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
            constexpr auto encoding = utf_encoding_of<std::iter_value_t<It>>;
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
            using difference_type = std::iter_difference_t<It>;
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
            SYS_STRING_NO_UNIQUE_ADDRESS EndIt m_last{};
            utf_codepoint_encoder<OutputEnc, false> m_encoder;
            uint8_t m_char_idx = 0;
        };

        template<std::input_iterator It, std::sentinel_for<It> EndIt, iter_direction Direction>
        class utf_iterator<utf32, It, EndIt, Direction>
        {
        public:
            using difference_type = std::iter_difference_t<It>;
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
            SYS_STRING_NO_UNIQUE_ADDRESS EndIt m_last{};
            value_type m_value = 0;
        };
    
        enum utf_view_type
        {
            byref,
            byval
        };

        template<utf_encoding Enc, std::ranges::input_range Range, utf_view_type ViewType>
        class utf_view
        {
        private:
            using range = std::remove_cvref_t<Range>;
            using stored_type = std::conditional_t<ViewType == byval, range, const range *>;

            SYS_STRING_FORCE_INLINE static auto range_ref(const range & src) -> const range &
                { return src; }
            SYS_STRING_FORCE_INLINE static auto range_ref(const range * src) -> const range &
                { return *src; }

            static constexpr bool is_reversible = ranges::reverse_traversable_range<range>;
            static constexpr auto source_encoding = utf_encoding_of<std::ranges::range_value_t<range>>;
            
            using access_iterator = decltype(std::ranges::begin(range_ref(std::declval<stored_type>())));
            using access_sentinel = decltype(std::ranges::end(range_ref(std::declval<stored_type>())));
            using access_reverse_iterator = std::conditional_t<is_reversible, 
                                                            decltype(std::ranges::rbegin(range_ref(std::declval<stored_type>()))),
                                                            void>;
            using access_reverse_sentinel = std::conditional_t<is_reversible, 
                                                            decltype(std::ranges::rend(range_ref(std::declval<stored_type>()))),
                                                            void>;

        public:
            using iterator = utf_iterator<Enc, access_iterator, access_sentinel, iter_direction::forward>;
            using const_iterator = iterator;
            using reverse_iterator = std::conditional_t<is_reversible, 
                                                        utf_iterator<Enc, access_reverse_iterator, access_reverse_sentinel, iter_direction::backward>,
                                                        void>;
            using const_reverse_iterator = reverse_iterator;
            
            using value_type = typename iterator::value_type;
            using reference = typename iterator::reference;
            using const_reference = reference;
            using pointer = typename iterator::pointer;
            using const_pointer = pointer;

            static const bool borrowed = (ViewType == byref) || 
                                         (ViewType == byval && std::ranges::borrowed_range<range>);
            
        public:
            utf_view(const Range & src) noexcept requires(ViewType == byref) :
                m_src(&src)
            {}

            utf_view(Range && src) noexcept requires(ViewType == byref) = delete;

            utf_view(const Range & src) noexcept(noexcept(stored_type(src))) 
            requires(ViewType == byval && std::is_copy_constructible_v<stored_type>) :
                m_src(src)
            {}
            utf_view(Range && src) noexcept(noexcept(stored_type(std::move(src)))) 
            requires(ViewType == byval && std::is_move_constructible_v<stored_type>) :
                m_src(std::move(src))
            {}

            template<class... Args>
            utf_view(Args && ...args) noexcept(noexcept(stored_type(std::forward<Args>(args)...))) 
            requires(ViewType == byval && std::is_constructible_v<stored_type, Args...>) :
                m_src(std::forward<Args>(args)...)
            {}


            SYS_STRING_FORCE_INLINE iterator begin() const
                { return iterator(std::ranges::begin(range_ref(m_src)), std::ranges::end(range_ref(m_src))); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t end() const
                { return std::default_sentinel; }
            SYS_STRING_FORCE_INLINE const_iterator cbegin() const
                { return begin(); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t cend() const
                { return end(); }
            SYS_STRING_FORCE_INLINE reverse_iterator rbegin() const requires(is_reversible)
                { return reverse_iterator(std::ranges::rbegin(range_ref(m_src)), std::ranges::rend(range_ref(m_src))); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t rend() const requires(is_reversible)
                { return std::default_sentinel; }
            SYS_STRING_FORCE_INLINE const_reverse_iterator crbegin() const requires(is_reversible)
                { return rbegin(); }
            SYS_STRING_FORCE_INLINE std::default_sentinel_t crend() const requires(is_reversible)
                { return rend(); }

            bool empty() const requires(std::ranges::forward_range<Range>)
                { return std::ranges::empty(range_ref(m_src)); }
            explicit operator bool() const requires(std::ranges::forward_range<Range>)
                { return !std::ranges::empty(m_src); }
            decltype(auto) front() const requires(std::ranges::forward_range<Range>)
                { return *this->begin();}
            decltype(auto) back() const requires(is_reversible)
                { return *this->rbegin();}

            reverse_iterator reverse(iterator it) const requires(is_reversible)
                { return reverse_iterator(it, std::ranges::rend(range_ref(m_src))); }

            iterator reverse(reverse_iterator it) const requires(is_reversible)
                { return iterator(it, std::ranges::end(range_ref(m_src))); }
            
            template<class Func>
            decltype(auto) each(Func func) const
                { return utf_converter<source_encoding, Enc>::for_each_converted(range_ref(), func); }
            
        private:
            stored_type m_src;
        };
    }

    template<utf_encoding Enc, std::ranges::input_range Range>
    using utf_ref_view = util::utf_view<Enc, Range, util::byref>;
    template<std::ranges::input_range Range>
    using utf8_ref_view = utf_ref_view<utf8, Range>;
    template<std::ranges::input_range Range>
    using utf16_ref_view = utf_ref_view<utf16, Range>;
    template<std::ranges::input_range Range>
    using utf32_ref_view = utf_ref_view<utf32, Range>;

    template<utf_encoding Enc, std::ranges::input_range Range>
    using utf_owning_view = util::utf_view<Enc, Range, util::byval>;
    template<std::ranges::input_range Range>
    using utf8_owning_view = utf_owning_view<utf8, Range>;
    template<std::ranges::input_range Range>
    using utf16_owning_view = utf_owning_view<utf16, Range>;
    template<std::ranges::input_range Range>
    using utf32_owning_view = utf_owning_view<utf32, Range>;

    template<utf_encoding Enc, class T> struct utf_view_traits;


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
        template <class X>
        requires(
            requires(X && x) { utf_view_traits<Enc, std::remove_cvref_t<X>>::as_utf(std::forward<X>(x)); }
        )
        [[nodiscard]] constexpr auto operator()(X && x) const
            noexcept(noexcept(utf_view_traits<Enc, std::remove_cvref_t<X>>::as_utf(std::forward<X>(x))))
                  -> decltype(utf_view_traits<Enc, std::remove_cvref_t<X>>::as_utf(std::forward<X>(x))) 
                     { return utf_view_traits<Enc, std::remove_cvref_t<X>>::as_utf(std::forward<X>(x)); }

        template <class Range>
        requires(
            !requires(Range && range) { utf_view_traits<Enc, std::remove_cvref_t<Range>>::as_utf(std::forward<Range>(range)); } &&
            std::ranges::view<std::remove_cvref_t<Range>>
        )
        [[nodiscard]] constexpr auto operator()(Range && range) const
            noexcept(noexcept(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range))))
                  -> decltype(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range))) 
                     { return util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range)); }

        template <class Range>
        requires( 
            !requires(Range && range) { utf_view_traits<Enc, std::remove_cvref_t<Range>>::as_utf(std::forward<Range>(range)); } &&
            !std::ranges::view<std::remove_cvref_t<Range>> &&
            requires(Range && range) { util::utf_view<Enc, std::remove_cvref_t<Range>, util::byref>(std::forward<Range>(range)); }
        )
        [[nodiscard]] constexpr auto operator()(Range && range) const
            noexcept(noexcept(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byref>(std::forward<Range>(range))))
                  -> decltype(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byref>(std::forward<Range>(range))) 
                     { return util::utf_view<Enc, std::remove_cvref_t<Range>, util::byref>(std::forward<Range>(range)); }


        template <class Range>
        requires( 
            !requires(Range && range) { utf_view_traits<Enc, std::remove_cvref_t<Range>>::as_utf(std::forward<Range>(range)); } &&
            !std::ranges::view<std::remove_cvref_t<Range>> &&
            !requires(Range && range) { util::utf_view<Enc, std::remove_cvref_t<Range>, util::byref>(std::forward<Range>(range)); }
        )
        [[nodiscard]] constexpr auto operator()(Range && range) const
            noexcept(noexcept(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range))))
                  -> decltype(util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range))) 
                     { return util::utf_view<Enc, std::remove_cvref_t<Range>, util::byval>(std::forward<Range>(range)); }

    };

    template<utf_encoding Enc>
    inline constexpr auto as_utf = as_utf_func<Enc>{};
    inline constexpr auto as_utf8 = as_utf_func<utf8>{};
    inline constexpr auto as_utf16 = as_utf_func<utf16>{};
    inline constexpr auto as_utf32 = as_utf_func<utf32>{};
}

namespace std::ranges {
    template<sysstr::utf_encoding Enc, class Range, sysstr::util::utf_view_type ViewType>
    constexpr bool enable_view<sysstr::util::utf_view<Enc, Range, ViewType>> = true;
    
    template<sysstr::utf_encoding Enc, class Range, sysstr::util::utf_view_type ViewType>
    constexpr bool enable_borrowed_range<sysstr::util::utf_view<Enc, Range, ViewType>> = sysstr::util::utf_view<Enc, Range, ViewType>::borrowed;
}

namespace sysstr
{
    template<utf_encoding InputEnc, std::output_iterator<char32_t> Out>
    class utf_output_decoder
    {
    private:
        struct sink
        {
            Out dest;

            void operator()(char32_t c)
            { 
                *dest = c;
                ++dest;
            }
        };

        struct proxy
        {
            utf_output_decoder * decoder;

            template<class Char>
            requires(utf_encoding_of<Char> == InputEnc)
            SYS_STRING_FORCE_INLINE
            auto operator=(Char c) const -> const proxy &
            {
                decoder->put(c);
                return *this;
            }
        };
    public:
        class iterator
        {
        public:
            using difference_type = ptrdiff_t;
            using iterator_category = std::output_iterator_tag;
        public:
            iterator(utf_output_decoder & decoder):
                m_decoder(&decoder)
            {}
            auto operator*() -> proxy
                { return proxy{m_decoder}; }
            auto operator++() -> iterator &
                { return *this; }
            auto operator++(int) -> iterator
                { return *this; }

            
            friend constexpr bool operator==(const iterator &, std::default_sentinel_t)
                { return false; }
            friend constexpr bool operator==(std::default_sentinel_t, const iterator &)
                { return false; }
            friend constexpr bool operator!=(const iterator &, std::default_sentinel_t)
                { return true; }
            friend constexpr bool operator!=(std::default_sentinel_t, const iterator &)
                { return true; }
        private:
            utf_output_decoder * m_decoder;
        };
    public:
        utf_output_decoder(Out dest):
            m_impl(sink{dest})
        {}

        auto begin() -> iterator
            { return iterator(*this); }
        auto end() -> std::default_sentinel_t
            { return std::default_sentinel; }

        template<class Char>
        requires(utf_encoding_of<Char> == InputEnc)
        SYS_STRING_FORCE_INLINE
        void put(Char c)
            { m_impl.put(c); }

        SYS_STRING_FORCE_INLINE
        void flush()
            { m_impl.flush(); }
    private:
        utf32_decoder<InputEnc, sink> m_impl;
    };

    template<utf_encoding InputEnc, std::output_iterator<char32_t> Out>
    auto make_utf_output_decoder(Out dest)
        { return utf_output_decoder<InputEnc, Out>(dest); }
}


#endif
