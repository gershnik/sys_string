# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

from textwrap import dedent
from common import char_name, indent_insert

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

    def __make_content(self):
        ret = 'u"'
        char_count = 0
        for char in self.chars:
            ret += char_name(char)
            char_count += 1
            if char_count > 0 and char_count % 16 == 0:
                ret += '"\nu"'
        ret += '"'
        return ret
    
    @staticmethod
    def print_common_header():
        ret = '''
        template<class Derived>
        class lookup
        {
        public:
            static bool test(char32_t c) noexcept
            {
                if (c > Derived::max_char)
                    return false;
                for(auto p = Derived::chars; *p; ++p)
                    if (*p == c)
                        return true;
                return false;
            }
        };
        '''
        return dedent(ret)
    
    def print_header(self, name):
        ret = f'''
        class {name} : public lookup<{name}>
        {{
        friend lookup<{name}>;
        private:
            static const char16_t chars[{self.length}];
        public:
            static constexpr char32_t max_char = U'{char_name(self.max_char)}';
            
            static constexpr size_t data_size = sizeof(chars);
        }};
        '''
        return dedent(ret)
    
    def print_impl(self, name):
        ret = f'''
        const char16_t {name}::chars[] = 
            {indent_insert(self.__make_content(), 12)};
        '''
        return dedent(ret)
