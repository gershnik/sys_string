# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
#

from textwrap import dedent
import textwrap
from common import bytes_for_bits, format_array, type_for_bits, indent_insert


class trie_builder:

    def __init__(self):
        self.__split = (11,5,4)

        self.__ascii = [0] * 128
        self.__stages = [None] * (1 << self.__split[0])
        self.__values = None
        self.__bits_for_values = None
        self.__bits_for_ascii = None

        self.__max_known_char = 0

    def add_char(self, char, flag):

        if char < 128:
            self.__ascii[char] |= flag
        else:
            current = self.__stages
            for bits_idx, bits in enumerate(self.__split[:-1]):
                shift = sum(self.__split[bits_idx + 1:])
                current_idx = (char >> shift) & ((1 << bits) - 1)
                current_val = current[current_idx]
                if current_val is None:
                    current_val = [None] * (1 << self.__split[bits_idx + 1])
                    current[current_idx] = current_val
                current = current_val
            
            current_idx = char & ((1 << self.__split[-1]) - 1)
            current_val = current[current_idx]
            if current_val is None:
                current_val = 0
            current[current_idx] = current_val | flag

        self.__max_known_char = max(self.__max_known_char, char)

        
    def add_chars(self, start, end, flag):
        for c in range(start, end):
            self.add_char(c, flag)

    def get_char(self, char):
        if char < 128:
            return self.__ascii[char]
        
        current = self.__stages
        for bits_idx, bits in enumerate(self.__split[:-1]):
            shift = sum(self.__split[bits_idx + 1:])
            current_idx = (char >> shift) & ((1 << bits) - 1)
            current = current[current_idx]
            if current is None:
                return 0
        
        current_idx = char & ((1 << self.__split[-1]) - 1)
        current_val = current[current_idx]
        if current_val is None:
            return 0
        return current_val

    def __collect_values(self, mapping: list, target_level: int, level: int, values: set):
        for val in mapping:
            if val is None:
                continue
            if level < target_level:
                self.__collect_values(val, target_level, level + 1, values)
            else:
                values.add(val if isinstance(val, int) else tuple(val))

    def __assign_values(self, mapping: list, target_level: int, level: int, values_index: dict):
        if level < target_level:
            for val in mapping:
                if val is None:
                    continue
                self.__assign_values(val, target_level, level + 1, values_index)
        else:
            for idx, val in enumerate(mapping):
                if val is None:
                    mapping[idx] = 0
                else:
                    mapping[idx] = values_index[val if isinstance(val, int) else tuple(val)]


    def generate(self):

        values_lists = []

        for level in range(2, -1, -1):
            values = set()
            values.add(0 if level == 2 else (0,) * (1 << self.__split[level + 1]))
            self.__collect_values(self.__stages, level, 0, values)
            values_lists.append(sorted(values))
            values_index = { val: idx for idx, val in enumerate(values_lists[-1]) }
            self.__assign_values(self.__stages, level, 0, values_index)

        values_lists.append(self.__stages)
        values_lists.reverse()
        self.__values = values_lists
        self.__stages = None

        self.__bits_for_values = []
        total_size = 0
        for values_list in values_lists:
            if isinstance(values_list[0], int):
                max_value:int = max(values_list)
                list_len = len(values_list)
            else:
                max_value:int = max(max(x) for x in values_list)
                list_len = len(values_list) * len(values_list[0])

            bit_length = max_value.bit_length()
            self.__bits_for_values.append(bit_length)
            bytes_per_value = bytes_for_bits(bit_length)
            total_size += bytes_per_value * list_len

        self.__bits_for_ascii = max(self.__ascii).bit_length()
        total_size += bytes_for_bits(self.__bits_for_ascii) * len(self.__ascii)

        return total_size

    def __make_nested(self, values, bits=0):
        ret = ''
        for block_idx, block in enumerate(values):
            if block_idx > 0:
                ret += ','
            ret += '\n{{'
            arr = format_array(block, bits=bits, max_width=200)
            if arr.find('\n') != -1:
                arr = '\n' + textwrap.indent(arr, '  ') + '\n'
            ret += arr
            ret += '}}'
        return ret
    
    def print_header(self, name, values_enum_content):
        ret = f'''
        class {name} 
        {{
        private:
            static const std::array<{type_for_bits(self.__bits_for_ascii)}, {len(self.__ascii)}> ascii;
        '''
        for idx, values in enumerate(self.__values):
            if isinstance(values[0], int):
                ret += f'''
            static const std::array<{type_for_bits(self.__bits_for_values[idx])}, {len(values)}> stage{idx + 1};
        '''
            else:
                ret += f'''
            static const std::array<std::array<{type_for_bits(self.__bits_for_values[idx])}, {len(values[0])}>, {len(values)}> stage{idx + 1};
        '''

        if values_enum_content is not None:
            ret += f'''
        public:
            enum value : decltype(stage{len(self.__values)})::value_type
            {{
                none = 0,
                {indent_insert(values_enum_content, 16)}
            }};
        '''
        else:
            ret += f'''
        public:
            using value = decltype(stage{len(self.__values)})::value_type;
        '''
            
        shift = sum(self.__split[1:])
        ret += f'''
            SYS_STRING_FORCE_INLINE
            static auto get(char32_t c) noexcept
            {{
                if (c < 128)
                    return value(ascii[c]);
                
                size_t stage_idx = (c >> {shift}) & 0x{(1 << self.__split[0]) - 1:X};
                size_t base = stage1[stage_idx];'''
        for idx, bits in enumerate(self.__split[1:-1]):
            shift = sum(self.__split[idx + 2:])
            ret += f'''
                stage_idx = (c >> {shift}) & 0x{(1 << bits) - 1:X};
                base = stage{idx + 2}[base][stage_idx];'''
        ret += f'''
                stage_idx = c & 0x{(1 << self.__split[-1]) - 1:X};
                base = stage{idx + 3}[base][stage_idx];
                return value(stage{idx + 4}[base]);
            }}
        '''
            
        ret += '''
            static constexpr size_t data_size = '''
        for idx in range(0, len(self.__values)):
            ret += f'''                         
                                                sizeof(stage{idx + 1}) +'''
        ret += '''                              
                                                sizeof(ascii);
        };
        '''

        return dedent(ret)

    def print_impl(self, name):
        ret = f'''
        const std::array<{type_for_bits(self.__bits_for_ascii)}, {len(self.__ascii)}> {name}::ascii({{{{
            {indent_insert(format_array(self.__ascii, ishex=True, bits=self.__bits_for_ascii), 12)}
        }}}});
        '''
        for idx, values in enumerate(self.__values):
            if isinstance(values[0], int):
                ret += f'''
        const std::array<{type_for_bits(self.__bits_for_values[idx])}, {len(values)}> {name}::stage{idx + 1}({{{{
            {indent_insert(format_array(values, ishex=idx!=0, bits=self.__bits_for_values[idx]), 12)}
        }}}});
        '''
            else:
                ret += f'''
        const std::array<std::array<{type_for_bits(self.__bits_for_values[idx])}, {len(values[0])}>, {len(values)}> {name}::stage{idx + 1}({{{{
            {indent_insert(self.__make_nested(values, bits=self.__bits_for_values[idx]), 12)}
        }}}});
        '''

            
        return dedent(ret)
