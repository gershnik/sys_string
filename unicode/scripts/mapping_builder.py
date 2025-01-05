# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

from textwrap import dedent
from common import char_name, indent_insert

class mapping_builder:
    max_len = 1

    def __init__(self) -> None:
        self.mapping = {}

    def set_values(self, char, values):
        self.mapping[char] = values
        mapping_builder.max_len = max(mapping_builder.max_len, len(values))

    def generate(self):
        self.sorted_keys = sorted(self.mapping.keys())
        ret = 0
        for values in self.mapping.values():
            ret += 4 # for source
            for value in values:
                ret += 2 if value < 0x10000 else 4
        ret += (4 + 2)
        return ret


    def make_source_chars(self):
        ret = ''
        range_start = 0
        for idx, code in enumerate(self.sorted_keys):
            if idx > 0:
                ret += ', '
                if idx % 16 == 0:
                    ret += '\n'

            ret += f"{{{range_start} ,U'{char_name(code)}'}}"
            
            chars = self.mapping[code]
            range_len = 0
            for char in chars:
                range_len += (1 if char < 0x10000 else 2)
            range_start += range_len

        ret += f", {{{range_start} , 0}}"
        return ret
    
    def make_values_string(self):
        ret = '\nu"'
        char_count = 0
        for char in self.sorted_keys:
            values = self.mapping[char]
            for value in values:
                ret += char_name(value)
                char_count += 1
                if char_count > 0 and char_count % 16 == 0:
                    ret += '"\nu"'
        ret += '"'
        return ret

    def print_header(self, name):
        ret = f'''
        struct {name}
        {{
            static const char_lookup source_chars[];
            static constexpr size_t source_chars_len = {len(self.mapping)};
            static const char16_t chars[];
        }};
        '''
        return dedent(ret)

    def print_impl(self, name):
        ret = f'''
            const char_lookup {name}::source_chars[] = {{
                {indent_insert(self.make_source_chars(), 16)}
            }};

            const char16_t {name}::chars[] = 
                {indent_insert(self.make_values_string(), 16)}
            ;
        '''
        return dedent(ret)