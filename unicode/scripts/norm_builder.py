

from dataclasses import dataclass
from textwrap import dedent

from common import indent_insert
from trie_builder import trie_builder


class norm_builder:

    @dataclass
    class __char_info:
        decomp: list[int] = None
        comb_class: int = 0

    def __init__(self):
        self.__info_map: dict[int, norm_builder.__char_info] = {}
        self.__mapped_data = []
        self.__decomp_builder = trie_builder(4)
        self.index_mask = 0x0FFF


    def set_comb_class(self, c: int, val: int):
        if val == 0:
            return
        info = self.__info_map.setdefault(c, norm_builder.__char_info([], 0))
        info.comb_class = val

    def set_decomp(self, c: int, val: list[int]):
        info = self.__info_map.setdefault(c, norm_builder.__char_info([], 0))
        info.decomp = val

    def generate(self):
        char_idx = 0
        for char in sorted(self.__info_map.keys()):
            info = self.__info_map[char]
            
            decomp_len = len(info.decomp)
            ccc = [info.comb_class]
            final = True
            if decomp_len != 0:
                first_info = self.__info_map.get(info.decomp[0])
                final = first_info is None or first_info.decomp is None
                ccc[0] = first_info.comb_class if first_info is not None else 0
                if decomp_len == 2:
                    second_info = self.__info_map.get(info.decomp[1])
                    ccc.append(second_info.comb_class if second_info is not None else 0)
            
            
            upper = (final << 2) | decomp_len
            value = ((char - char_idx) & self.index_mask) + (upper << 12)
            self.__decomp_builder.add_chars(char, char + 1, value)
            if decomp_len != 0:
                chars_to_write = info.decomp
            else:
                chars_to_write = [0]
            data = [ (x << 21) | y for x, y in zip(ccc, chars_to_write)]
            self.__mapped_data += data
            char_idx += len(data)
            if char_idx > self.index_mask:
                raise RuntimeError('char_idx exceeds index mask')
            if char_idx > 0xFFFF:
                raise RuntimeError('char_idx exceeds 16 bit')
            
        size = char_idx * 4
        size += self.__decomp_builder.generate()
        return size
    
    def print_header(self):
        ret = f'''
        {indent_insert(self.__decomp_builder.print_header("decomp_info", ""), 8)}
        '''
        ret += '''
        extern const uint32_t decomp_data[];

        struct decomp_mapper 
        {
            template<utf_encoding Enc, class OutIt>
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = uint32_t())) -> OutIt
            {
                auto res = uint16_t(decomp_info::get(src));
                if (res == 0)
                {
                    *dest = src;
                    return ++dest;
                }
                uint16_t idx = res & ''' + f'0x{self.index_mask:04X}' + ''';
                size_t entry_offset = (size_t(src) - idx) & ''' + f'0x{self.index_mask:04X}' + ''';
                const uint32_t * entry = decomp_data + entry_offset;

                res >>= 12;
                int len = res & 0x3;
                if (len == 0)
                {
                    uint32_t val = uint32_t(src) | *entry;
                    *dest = val;
                    return ++dest;
                }
                
                bool final = res >> 2;
                if (!final) 
                {
                    dest = map_char<Enc>(*entry++, dest);
                }
                else
                {
                    *dest = *entry++;
                    ++dest;
                }
                
                if (--len)
                {
                    *dest = *entry++;
                    ++dest;
                }

                return dest;
            }
        };

        
        '''
        return dedent(ret)
    

    def __print_mapped_data(self):
        ret = '\n'
        val_count = 0
        for val in self.__mapped_data:
            ret += f'0x{val:08X}, '
            val_count += 1
            if val_count > 0 and val_count % 16 == 0:
                ret += '\n'
        return ret
    
    def print_impl(self):
        ret = f'''
        const uint32_t decomp_data[] = {{
            {indent_insert(self.__print_mapped_data(), 12)}
        }};

        {indent_insert(self.__decomp_builder.print_impl("decomp_info"), 8)}
        '''
        return dedent(ret)