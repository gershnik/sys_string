# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

from textwrap import dedent
from common import char_name, format_utf16_string, indent_insert

class lookup_builder:

    def __init__(self):
        self.chars = []
        self.max_char = 0
        self.length = 0

    def add_chars(self, start, end):
        for char in range(start, end):
            self.chars.append(char)

    def generate(self):
        for char in self.chars:
            self.length += 1 if char < 0x10000 else 2
            self.max_char = max(self.max_char, char)
        self.length += 1
        return self.length * 2

    def print_header(self, name):
        ret = f'''
        class {name} 
        {{
        private:
            static const char16_t chars[{self.length}];
        public:
            static constexpr char32_t max_char = U'{char_name(self.max_char)}';

            static bool test(char32_t c) noexcept
            {{
                if (c > max_char)
                    return false;
                for(auto p = chars; *p; ++p)
                    if (*p == c)
                        return true;
                return false;
            }}
            
            static constexpr size_t data_size = sizeof(chars);
        }};
        '''
        return dedent(ret)
    
    def print_impl(self, name):
        ret = f'''
        inline constexpr char16_t {name}::chars[{self.length}] = 
            {indent_insert(format_utf16_string(self.chars), 12)};
        '''
        return dedent(ret)
