

from dataclasses import dataclass
from textwrap import dedent

from common import format_array, indent_insert
from trie_builder import trie_builder


class norm_builder:

    @dataclass
    class __char_info:
        decomp: list[int] = None
        comb_class: int = 0
        nfc_qc_not_yes: bool = False

    @dataclass
    class __composition:
        second: int
        second_ccc: int
        composition: int
        composition_ccc: int


    def __init__(self):
        self.__info_map: dict[int, norm_builder.__char_info] = {}
        self.__exclusions = set()
        self.__decomp_builder = trie_builder()
        self.__values = None
        self.__compositions = None


    def set_comb_class(self, c: int, val: int):
        if val == 0:
            return
        info = self.__info_map.setdefault(c, norm_builder.__char_info([], 0))
        info.comb_class = val

    def set_decomp(self, c: int, val: list[int]):
        info = self.__info_map.setdefault(c, norm_builder.__char_info([], 0))
        info.decomp = val

    def set_exclusion(self, c: int):
        self.__exclusions.add(c)

    def set_nfc_qc_not_yes(self, c: int):
        info = self.__info_map.setdefault(c, norm_builder.__char_info([], 0))
        info.nfc_qc_not_yes = True
        
    def __get_ccc(self, char):
        char_info = self.__info_map.get(char)
        ccc = char_info.comb_class if char_info is not None else 0
        return ccc

    def __collect_compositions(self):
        compositions: dict[int, list[norm_builder.__composition]] = {}
        for src_char, src_info in self.__info_map.items():
            if len(src_info.decomp) == 0:
                continue
            first = src_info.decomp[0]
            second = src_info.decomp[1] if len(src_info.decomp) == 2 else None
            
            if second is not None:
                comp = self.__composition(second=second, second_ccc=self.__get_ccc(second), 
                                          composition=src_char, composition_ccc=self.__get_ccc(src_char))
                if (comp_list := compositions.get(first)) is None:
                    compositions[first] = [comp]
                elif not (comp in comp_list):
                    comp_list.append(comp)
                    comp_list.sort(key=lambda c: c.composition)
            else:
                if (comp_list := compositions.get(first)) is None:
                    compositions[first] = []


        sorted_compositions: dict[int, tuple[int, int, int, list[norm_builder.__composition]]] = {}
        count = 0
        for char in sorted(compositions.keys()):
            ccc = self.__get_ccc(char)
            comps = compositions[char]
            value = (count, char, ccc, comps)
            sorted_compositions[char] = value
            count += (len(value[3]) * 2 + 1)
        
        if count > 0xFFF:
            raise RuntimeError('compositions count exceeds 12 bits')
        
        return sorted_compositions, count


    # Data layout
    # ==========
    # Trie lookup returns either:
    # * Direct value for a character which otherwise has no decompositions and starts
    #   no compositions: 
    #   - bit   12: 1
    #   - bit    8: 1/0 whether NFC_QC is *not* YES
    #   - bits 0-7: ccc
    # * Indirect value: an encoded index into values array.
    #   - bit    12: 0
    #   - bits 0-11: (char - value index) & 0xFFF
    #
    # Values array contains 2 types of 32-bit entries:
    #
    # * Full (decomposition and possibly composition) entry
    #   - bit     31: 0
    #   - bit     30: 1/0 whether NFC_QC is *not* YES
    #   - bit     29: 1/0 whether this decomposition is final or the 
    #                 result can be further decomposed
    #   - bits 24-28: offset of the second char in decomposition from the first.
    #                 if 0 means no second char.
    #   - bits 12-24: index of the first char in decomposition in the compositions
    #                 array
    #   - bits  0-11: index of the character's compositions in the compositions
    #                 array. 0xFFF if none
    #
    # * Composition only entry
    #   - bit     31: 1
    #   - bit     30: 1/0 whether NFC_QC is *not* YES
    #   - bits 12-20: ccc of the character
    #   - bits  0-11: index of the character's compositions in the compositions
    #                 array. Will not be 0xFFF
    #
    # Compositions array contains variable length sequences consiting of 32-bit entries:
    # Each sequence starts with a:
    #
    # * Header entry:
    #   - bit     29: 1/0 whether this is the last entry
    #   - bits 21-28: ccc of the first char of a composition
    #   - bits  0-20: first char of a composition
    #
    # A header entry not having bit 29 set can be followed by one or more trailers 
    # consiting of *2* 32-bit entries
    #
    # * Trailer entry 1:
    #   - bit     29: 1/0 whether this is the last entry
    #   - bits 21-28: ccc of the second char of a composition
    #   - bits  0-20: second char of a composition
    # * Trailer entry 2 is 0 if the composition is blocked. Otherwise
    #   - bits 21-28: ccc of the *result* char of a composition
    #   - bits  0-20: *result* char of a composition
    #                                            
    #
    def generate(self):
        compositions, compositions_count = self.__collect_compositions()

        values = []
        for char in sorted(self.__info_map.keys()):
            info = self.__info_map[char]

            if (comp := compositions.get(char)) is not None:
                comp_start = comp[0]
            else:
                comp_start = 0x0FFF

            nfc_qc_not_yes = info.nfc_qc_not_yes

            decomp_len = len(info.decomp)
            
            if decomp_len != 0:
                first_info = self.__info_map.get(info.decomp[0])
                final = first_info is None or len(first_info.decomp) == 0

                decomp_entry = compositions.get(info.decomp[0])
                decomp_start, _, _, decomp_seconds = decomp_entry
                decomp_idx = 0
                if decomp_len == 2:
                    for idx, decomp_second in enumerate(decomp_seconds):
                        if decomp_second.second == info.decomp[1]:
                            decomp_idx = idx + 1
                            break
                    
                value_idx = len(values)
                values.append((nfc_qc_not_yes << 30) | (final << 29) | (decomp_idx << 24) | (decomp_start << 12) | comp_start)
                if len(values) > 0x0FFF:
                    raise RuntimeError('values count exceeds 12 bits')
                value = ((char - value_idx) & 0x0FFF)
            elif comp_start != 0x0FFF:
                value_idx = len(values)
                values.append((1 << 31) | (nfc_qc_not_yes << 30) | (info.comb_class << 12) | comp_start)
                if len(values) > 0x0FFF:
                    raise RuntimeError('values count exceeds 12 bits')
                value = ((char - value_idx) & 0x0FFF)
            else:
                value = (1 << 12) | (nfc_qc_not_yes << 8) | info.comb_class

            self.__decomp_builder.add_chars(char, char + 1, value)

        # add pure compositions not previously handled via self.__info_map
        for char, comp in compositions.items():
            if len(comp[3]) > 0 and self.__decomp_builder.get_char(char) == 0:
                comp_start = comp[0]
                value_idx = len(values)
                values.append((1 << 31) | comp_start)
                if len(values) > 0x0FFF:
                    raise RuntimeError('values count exceeds 12 bits')
                value = ((char - value_idx) & 0x0FFF)
                self.__decomp_builder.add_chars(char, char + 1, value)

        linear_comps = []
        for _, first, first_ccc, comps in compositions.values():
            is_last = (0 == len(comps))
            vals = [first | (first_ccc << 21) | (is_last << 29)]
            for idx, comp in enumerate(comps):
                is_last = (idx == len(comps) - 1)
                vals.append(comp.second | (comp.second_ccc << 21) | (is_last << 29))
                if not comp.composition in self.__exclusions:
                    vals.append(comp.composition | (comp.composition_ccc << 21))
                else:
                    vals.append(0)
            linear_comps += vals
            
        size = compositions_count * 4
        size += len(values) * 4
        size += self.__decomp_builder.generate()

        self.__compositions = linear_comps
        self.__values = values
        
        return size
    
    def print_header(self):
        ret = f'''
        class normalizer 
        {{
        private:
            {indent_insert(self.__decomp_builder.print_header("lookup", None), 12)}

            static const uint32_t compositions[{len(self.__compositions)}];
            static const uint32_t values[{len(self.__values)}];
        '''
        ret += '''
        public:
            static constexpr size_t data_size = sizeof(compositions) + 
                                                sizeof(values) +
                                                lookup::data_size;

            template<utf_encoding Enc, class OutIt>
            static auto decompose(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = uint32_t())) -> OutIt
            {
                auto res = lookup::get(src);
                if (res == 0)
                {
                    *dest = src;
                    return ++dest;
                }
                if (res & 0x1000)
                {
                    uint32_t shifted_ccc = uint32_t(res & 0xFF) << 21;
                    uint32_t val = uint32_t(src) | shifted_ccc;
                    *dest = val;
                    return ++dest;
                }

                size_t value_offset = ((size_t(src) - res) & 0x0FFF);
                uint32_t value = values[value_offset];
                if (value & (1 << 31))
                {
                    uint32_t shifted_ccc = (value & 0x0FF000) << 9;
                    uint32_t val = uint32_t(src) | shifted_ccc;
                    *dest = val;
                    return ++dest;
                }

                
                value >>= 12;
                uint16_t decomp_start = value & 0xFFF;
                value >>= 12;
                uint16_t decomp_idx = value & 0x1F;
                int final = ((value & (1 << 5)) != 0);
                
                auto * comps = compositions + decomp_start;
                
                uint32_t first = comps[0] & 0x1FFF'FFFF;
                if (!final) 
                {
                    dest = decompose<Enc>(first & 0x1F'FFFF, dest);
                }
                else
                {
                    *dest = first;
                    ++dest;
                }
                
                if (decomp_idx != 0)
                {
                    uint32_t second = comps[decomp_idx * 2 - 1] & 0x1FFF'FFFF;
                    *dest = second;
                    ++dest;
                }

                return dest;
            }

            static auto get_compositions(char32_t src) -> const uint32_t *
            {
                auto res = lookup::get(src);
                if (res == 0)
                    return nullptr;
                
                if (res & 0x1000)
                    return nullptr;

                size_t value_offset = ((size_t(src) - res) & 0x0FFF);
                uint32_t value = values[value_offset];

                uint16_t comp_idx = value & 0xFFF;
                if (comp_idx == 0xFFF)
                    return nullptr;

                auto ret = compositions + comp_idx;
                bool is_last = (ret[0] & (uint32_t(1) << 29));
                if (is_last)
                    return nullptr;
                if (ret[0] & (uint32_t(0xFF) << 21))
                    return nullptr;
                
                ++ret;
                return ret;
            }

            static auto get_comb_class(char32_t c) -> uint8_t
            {
                if (c < 128)
                    return 0;

                auto res = lookup::get(c);
                if (res == 0)
                    return 0;
                
                if (res & 0x1000)
                    return uint8_t(res);

                size_t value_offset = ((size_t(c) - res) & 0x0FFF);
                uint32_t value = values[value_offset];

                // possible but an extra if...
                //if (value & (uint32_t(1) << 30))
                //    return (value >> 12) & 0xFF;
                
                uint16_t comp_idx = value & 0xFFF;
                if (comp_idx == 0xFFF)
                    return 0;

                auto * comps = compositions + comp_idx;
                return (comps[0] >> 21) & 0xFF;
            }

            enum class nfc_qc_status
            {
                bad,
                good,
                stable
            };

            static auto get_nfc_qc_status(char32_t c) -> nfc_qc_status
            {
                if (c < 128)
                    return nfc_qc_status::stable;

                auto res = lookup::get(c);
                if (res == 0)
                    return nfc_qc_status::stable;

                if (res & 0x1000)
                {
                    int is_ccc_zero = uint8_t(res) == 0;
                    int is_nfc_qc_yes = !(res >> 8);
                    return nfc_qc_status(0 + (is_nfc_qc_yes << is_ccc_zero));
                }

                size_t value_offset = ((size_t(c) - res) & 0x0FFF);
                uint32_t value = values[value_offset];

                int is_nfc_qc_yes = !(value & (1 << 30));

                uint16_t comp_idx = value & 0xFFF;
                if (comp_idx == 0xFFF)
                    return nfc_qc_status(0 + (is_nfc_qc_yes << 1));

                auto * comps = compositions + comp_idx;
                int is_ccc_zero = !(comps[0] & (uint32_t(0xFF) << 21));
                
                return nfc_qc_status(0 + (is_nfc_qc_yes << is_ccc_zero));
            }
        };
        '''
        return dedent(ret)
    

    def print_impl(self):
        ret = f'''
        const uint32_t normalizer::compositions[] = {{
            {indent_insert(format_array(self.__compositions, ishex=True, bits=32), 12)}
        }};

        const uint32_t normalizer::values[] = {{
            {indent_insert(format_array(self.__values, ishex=True, bits=32), 12)}
        }};

        {indent_insert(self.__decomp_builder.print_impl("normalizer::lookup"), 8)}
        '''
        return dedent(ret)