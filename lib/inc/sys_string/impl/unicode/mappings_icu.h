//
// Copyright 2025 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_ICU_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_ICU_H_INCLUDED

#include <unicode/uchar.h>
#include <unicode/ustring.h>
#include <unicode/uset.h>
#include <unicode/ucpmap.h>
#include <unicode/normalizer2.h>

static_assert(U_ICU_VERSION_MAJOR_NUM >= 67, "sys_string requires ICU 67 or higher");

#include <exception>
#include <stdexcept>
#include <atomic>

#include <sys_string/impl/unicode/mappings_common.h>


extern "C" 
{
    //Public, but undocumented stuff from ucase.h

    /* 2-bit constants for types of cased characters */
    #define UCASE_TYPE_MASK     3
    enum {
        UCASE_NONE,
        UCASE_LOWER,
        UCASE_UPPER,
        UCASE_TITLE
    };

    #define UCASE_IGNORABLE     4


    /* Casing locale types for ucase_getCaseLocale */
    enum {
        UCASE_LOC_UNKNOWN,
        UCASE_LOC_ROOT,
        UCASE_LOC_TURKISH,
        UCASE_LOC_LITHUANIAN,
        UCASE_LOC_GREEK,
        UCASE_LOC_DUTCH,
        UCASE_LOC_ARMENIAN
    };

    enum {
        /**
         * For string case mappings, a single character (a code point) is mapped
         * either to itself (in which case in-place mapping functions do nothing),
         * or to another single code point, or to a string.
         * Aside from the string contents, these are indicated with a single int32_t
         * value as follows:
         *
         * Mapping to self: Negative values (~self instead of -self to support U+0000)
         *
         * Mapping to another code point: Positive values >UCASE_MAX_STRING_LENGTH
         *
         * Mapping to a string: The string length (0..UCASE_MAX_STRING_LENGTH) is
         * returned. Note that the string result may indeed have zero length.
         */
        UCASE_MAX_STRING_LENGTH=0x1f
    };

    /**
     * Iterator function for string case mappings, which need to look at the
     * context (surrounding text) of a given character for conditional mappings.
     *
     * The iterator only needs to go backward or forward away from the
     * character in question. It does not use any indexes on this interface.
     * It does not support random access or an arbitrary change of
     * iteration direction.
     *
     * The code point being case-mapped itself is never returned by
     * this iterator.
     *
     * @param context A pointer to the iterator's working data.
     * @param dir If <0 then start iterating backward from the character;
     *            if >0 then start iterating forward from the character;
     *            if 0 then continue iterating in the current direction.
     * @return Next code point, or <0 when the iteration is done.
     */
    typedef UChar32 U_CALLCONV UCaseContextIterator(void *context, int8_t dir);

    /**
     * Get the full lowercase mapping for c.
     *
     * @param csp Case mapping properties.
     * @param c Character to be mapped.
     * @param iter Character iterator, used for context-sensitive mappings.
     *             See UCaseContextIterator for details.
     *             If iter==NULL then a context-independent result is returned.
     * @param context Pointer to be passed into iter.
     * @param pString If the mapping result is a string, then the pointer is
     *                written to *pString.
     * @param caseLocale Case locale value from ucase_getCaseLocale().
     * @return Output code point or string length, see UCASE_MAX_STRING_LENGTH.
     *
     * @see UCaseContextIterator
     * @see UCASE_MAX_STRING_LENGTH
     * @internal
     */
    U_CAPI int32_t U_EXPORT2
    ucase_toFullLower(UChar32 c,
                      UCaseContextIterator *iter, void *context,
                      const UChar **pString,
                      int32_t caseLocale);

    U_CAPI int32_t U_EXPORT2
    ucase_toFullUpper(UChar32 c,
                      UCaseContextIterator *iter, void *context,
                      const UChar **pString,
                      int32_t caseLocale);

    U_CAPI int32_t U_EXPORT2
    ucase_toFullFolding(UChar32 c,
                        const UChar **pString,
                        uint32_t options);

    /** @return UCASE_NONE, UCASE_LOWER, UCASE_UPPER, UCASE_TITLE */
    U_CAPI int32_t U_EXPORT2
    ucase_getType(UChar32 c);

    /** @return like ucase_getType() but also sets UCASE_IGNORABLE if c is case-ignorable */
    U_CAPI int32_t U_EXPORT2
    ucase_getTypeOrIgnorable(UChar32 c);
}


namespace sysstr::util::unicode 
{

    inline void icu_error_to_exception(UErrorCode ec)
    {
        if (U_FAILURE(ec))
        {
            if (ec == U_MEMORY_ALLOCATION_ERROR)
                throw std::bad_alloc();
            throw std::runtime_error(u_errorName(ec));
        }
    }
    inline void icu_error_to_abort(UErrorCode ec) noexcept
    {
        if (U_FAILURE(ec))
            std::terminate();
    }

    template<UProperty Prop>
    requires(Prop >= UCHAR_INT_START && Prop < UCHAR_INT_LIMIT)
    class icu_prop_map
    {
    public:
        static uint32_t lookup(char32_t c)
        {
            return ucpmap_get(get(), c);
        }
    private:
        static const UCPMap * get() 
        {
            auto ret = m_map.load(std::memory_order_acquire);
            if (!ret)
            {
                UErrorCode ec = U_ZERO_ERROR;
                ret = u_getIntPropertyMap(Prop, &ec);
                icu_error_to_abort(ec);
                m_map.store(ret, std::memory_order_release);
            }
            return ret;
        }
    private:
        static inline std::atomic<const UCPMap *> m_map{nullptr};
    };

    template<UProperty Prop>
    requires(Prop >= UCHAR_BINARY_START && Prop < UCHAR_BINARY_LIMIT)
    class icu_prop_set
    {
    public:
        static bool contains(char32_t c)
        {
            return uset_contains(get(), c);
        }
    private:
        static const USet * get() 
        {
            auto ret = m_set.load(std::memory_order_acquire);
            if (!ret)
            {
                UErrorCode ec = U_ZERO_ERROR;
                ret = u_getBinaryPropertySet(Prop, &ec);
                icu_error_to_abort(ec);
                m_set.store(ret, std::memory_order_release);
            }
            return ret;
        }
    private:
        static inline std::atomic<const USet *> m_set{nullptr};
    };

    struct is_whitespace 
    {
        static bool test(char32_t c) noexcept
        {
            return u_isUWhiteSpace(c);
        }
    };

    class case_mapper 
    {
    private:
        template<class Derived>
        struct mapper 
        {
            static constexpr size_t max_mapped_length = UCASE_MAX_STRING_LENGTH; 

            template<utf_encoding Enc, class OutIt>
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                const char16_t * out = nullptr;
                auto res = Derived::do_map(src, &out);
                if (res < 0)
                {
                    auto c = char32_t(~res);
                    return write_unsafe<Enc>(c, dest);
                }
                if (res > UCASE_MAX_STRING_LENGTH)
                {
                    auto c = char32_t(res);
                    return write_unsafe<Enc>(c, dest);
                }
                return write_unsafe<Enc>(out, out + res, dest);
            }
        };
    public:
        class fold : public mapper<fold>
        {
        friend mapper<fold>;
        private:
            static int32_t do_map(char32_t c, const char16_t ** out)
            {
                return ucase_toFullFolding(c, out, U_FOLD_CASE_DEFAULT);
            }
        };

        class to_lower : public mapper<to_lower>
        {
        friend mapper<to_lower>;
        private:
            static int32_t do_map(char32_t c, const char16_t ** out)
            {
                return ucase_toFullLower(c, nullptr, nullptr, out, UCASE_LOC_ROOT);
            }
        };

        class to_upper : public mapper<to_upper>
        {
        friend mapper<to_upper>;
        private:
            static int32_t do_map(char32_t c, const char16_t ** out)
            {
                return ucase_toFullUpper(c, nullptr, nullptr, out, UCASE_LOC_ROOT);
            }
        };    

        class props
        {
        public:
            enum value : uint8_t
            {
                none = 0,
                cased = 1,
                case_ignorable = 2
            };
        
            static auto get(char32_t c) noexcept
            {
                // uint8_t ret = uint8_t(icu_prop_set<UCHAR_CASED>::contains(c)) | 
                //              (uint8_t(icu_prop_set<UCHAR_CASE_IGNORABLE>::contains(c)) << 1);
                
                auto res = ucase_getTypeOrIgnorable(c);
                uint8_t ret = uint8_t((res & UCASE_TYPE_MASK) != UCASE_NONE) | 
                            (uint8_t(bool(res >> 2)) << 1);

                return value(ret);
            }
        };
    };

    class grapheme_cluster_break_prop 
    {
    public:
        enum value : uint8_t
        {
            none = 0,
            control = U_GCB_CONTROL,
            extend = U_GCB_EXTEND,
            regional_indicator = U_GCB_REGIONAL_INDICATOR,
            prepend = U_GCB_PREPEND,
            spacing_mark = U_GCB_SPACING_MARK,
            hangul_l = U_GCB_L,
            hangul_v = U_GCB_V,
            hangul_t = U_GCB_T,
            hangul_lv = U_GCB_LV,
            hangul_lvt = U_GCB_LVT,
            extended_pictographic = U_GCB_REGIONAL_INDICATOR + 1,
        #if U_ICU_VERSION_MAJOR_NUM >= 76
            in_cb_consonant = (U_INCB_CONSONANT << 4),
            in_cb_extend = (U_INCB_EXTEND << 4),
            in_cb_linker = (U_INCB_LINKER << 4),
        #else
            in_cb_consonant = (1 << 4),
            in_cb_extend = (2 << 4),
            in_cb_linker = (3 << 4),
        #endif
    
            basic_mask = 15,
            in_cb_mask = 48
        };
        static_assert(extended_pictographic < 16);
    
        static auto get(char32_t c) noexcept
        {
            uint8_t ret = none;

            auto cb = UGraphemeClusterBreak(icu_prop_map<UCHAR_GRAPHEME_CLUSTER_BREAK>::lookup(c));
            
            if (cb > 0 && cb <= U_GCB_REGIONAL_INDICATOR) 
            {
                if (cb == U_GCB_CR || cb == U_GCB_LF)
                    ret = control;
                else
                    ret = uint8_t(cb);
            }
            else if (icu_prop_set<UCHAR_EXTENDED_PICTOGRAPHIC>::contains(c))
            {
                return extended_pictographic; 
            }

            #if U_ICU_VERSION_MAJOR_NUM >= 76
                auto icb = UIndicConjunctBreak(icu_prop_map<UCHAR_INDIC_CONJUNCT_BREAK>::lookup(c));
                ret |= uint8_t(icb << 4);
            #endif
        
            return value(ret);
        }
    };

}


#endif 
