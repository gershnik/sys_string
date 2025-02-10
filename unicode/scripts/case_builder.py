
from dataclasses import dataclass
from textwrap import dedent
from typing import Optional

from common import chars_len_in_utf16, format_utf16_string, indent_insert
from trie_builder import trie_builder


class case_builder:
    @dataclass
    class __char_info:
        uppercase: Optional[list[int]] = None
        lowercase: Optional[list[int]] = None
        folding: Optional[list[int]] = None
        prop: int = 0

    def __init__(self):
        self.__info_map: dict[int, case_builder.__char_info] = {}
        self.__mapped_data = []
        self.__builder = trie_builder()
        self.__cased_data_len = 0
        self.index_mask = 0x0FFF

    def set_uppercase(self, c: int, val: list[int]):
        info = self.__info_map.setdefault(c, case_builder.__char_info())
        info.uppercase = val

    def set_lowercase(self, c: int, val: list[int]):
        info = self.__info_map.setdefault(c, case_builder.__char_info())
        info.lowercase = val

    def set_folding(self, c: int, val: list[int]):
        info = self.__info_map.setdefault(c, case_builder.__char_info())
        info.folding = val

    def set_props(self, start: int, end: int, flag: int):
        for c in range(start, end):
            info = self.__info_map.setdefault(c, case_builder.__char_info())
            info.prop |= flag

    def generate(self):
        char_idx = 0
        for char in sorted(self.__info_map.keys()):
            info = self.__info_map[char]
            added_len = 0
            lower_len = 0
            upper_len = 0
            folding_len = 0
            if info.lowercase is not None:
                lower_len = chars_len_in_utf16(info.lowercase)
                self.__mapped_data += info.lowercase
                added_len += lower_len
            if info.uppercase is not None:
                upper_len = chars_len_in_utf16(info.uppercase)
                self.__mapped_data += info.uppercase
                added_len += upper_len
            if info.folding is not None:
                if info.folding == info.lowercase:
                    folding_len = 0x0F
                elif info.folding == info.uppercase:
                    folding_len = 0x0E
                else:
                    folding_len = chars_len_in_utf16(info.folding)
                    self.__mapped_data += info.folding
                    added_len += folding_len
            
            value = 0
            if (lower_len != 0 or upper_len != 0 or folding_len != 0):
                value = ((char - char_idx) & self.index_mask) + (lower_len << 16) + (upper_len << 20) + ((folding_len & 0x0F) << 24)
            value += (info.prop << 28)
            if value != 0:
                self.__builder.add_chars(char, char + 1, value)
            char_idx += added_len
            if char_idx > self.index_mask:
                raise RuntimeError('char_idx exceeds index mask')
            if char_idx > 0xFFFF:
                raise RuntimeError('char_idx exceeds 16 bit')

        self.__cased_data_len = char_idx + 1
        size = self.__cased_data_len * 2
        size += self.__builder.generate()
        return size
    
    def print_header(self):
        ret = f'''
        class case_mapper 
        {{
        private:
            {indent_insert(self.__builder.print_header("lookup", None), 12)}

            static const char16_t cased_data[{self.__cased_data_len}];
        '''
        ret += '''  
        public:
            static constexpr size_t data_size = lookup::data_size + sizeof(cased_data);

            struct fold 
            {
                static constexpr size_t max_mapped_length = 8; 

                template<utf_encoding Enc, class OutIt>
                SYS_STRING_FORCE_INLINE
                static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
                {
                    auto res = lookup::get(src);
                    uint8_t fold_len = (res >> 24) & 0x0F;
                    if (fold_len == 0)
                        return write_unsafe<Enc>(src, dest);
                    uint8_t fold_offset;
                    if (fold_len == 0x0F)
                    {
                        fold_len = (res >> 16) & 0x0F;
                        fold_offset = 0;
                    }
                    else if (fold_len == 0x0E)
                    {
                        fold_len = (res >> 20) & 0x0F;
                        fold_offset = (res >> 16) & 0x0F;
                    }
                    else
                    {
                        fold_offset = ((res >> 16) & 0x0F) + ((res >> 20) & 0x0F);
                    }
                    size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & ''' + f'0x{self.index_mask:04X}' + ''';
                    const char16_t * entry = cased_data + entry_offset;
                    return write_unsafe<Enc>(entry + fold_offset, entry + fold_offset + fold_len, dest);
                }
            };

            struct to_lower
            {
                static constexpr size_t max_mapped_length = 8; 

                template<utf_encoding Enc, class OutIt>
                SYS_STRING_FORCE_INLINE
                static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
                {
                    auto res = lookup::get(src);
                    uint8_t lower_len = (res >> 16) & 0x0F;
                    if (lower_len == 0)
                        return write_unsafe<Enc>(src, dest);
                    size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & ''' + f'0x{self.index_mask:04X}' + ''';
                    const char16_t * entry = cased_data + entry_offset;
                    return write_unsafe<Enc>(entry, entry + lower_len, dest);
                }
            };

            struct to_upper
            {
                static constexpr size_t max_mapped_length = 8; 

                template<utf_encoding Enc, class OutIt>
                SYS_STRING_FORCE_INLINE
                static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
                {
                    auto res = lookup::get(src);
                    uint8_t upper_len = (res >> 20) & 0x0F;
                    if (upper_len == 0)
                        return write_unsafe<Enc>(src, dest);
                    uint8_t upper_offset = (res >> 16) & 0x0F;
                    size_t entry_offset = (size_t(src) - (res & 0xFFFF)) & ''' + f'0x{self.index_mask:04X}' + ''';
                    const char16_t * entry = cased_data + entry_offset;
                    return write_unsafe<Enc>(entry + upper_offset, entry + upper_offset + upper_len, dest);
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
            
                SYS_STRING_FORCE_INLINE
                static auto get(char32_t c) noexcept
                {
                    auto res = lookup::get(c);
                    return value(res >> 28);
                }
            };
        };
        '''
        return dedent(ret)
    
    def print_impl(self):
        ret = f'''
        inline constexpr char16_t case_mapper::cased_data[{self.__cased_data_len}] = 
            {indent_insert(format_utf16_string(self.__mapped_data), 12)};

        {indent_insert(self.__builder.print_impl("case_mapper::lookup"), 8)}
        '''
        return dedent(ret)