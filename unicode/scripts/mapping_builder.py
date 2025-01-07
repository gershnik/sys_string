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
    

    def __init__(self) -> None:
        self.mapping = {}
        self.max_mapped_len = 1
        self.total_mapped_len = 0
        self.sorted_keys = None

    def set_values(self, char, values):
        self.mapping[char] = values
        self.max_mapped_len = max(self.max_mapped_len, len(values))

    def generate(self):
        self.sorted_keys = sorted(self.mapping.keys())
        ret = 0
        for values in self.mapping.values():
            ret += 4 # for source
            for value in values:
                if value < 0x10000:
                    self.total_mapped_len += 1
                    ret += 2
                else:
                    self.total_mapped_len += 2
                    ret += 4
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
    
    @staticmethod
    def print_common_header():
        ret = '''
        struct char_lookup
        {
            char32_t offset:11;
            char32_t value:21;
        };

        template<class Derived>
        class mapper 
        {
        public:
            template<utf_encoding Enc, class OutIt>
            static auto map_char(char32_t src, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                const char_lookup * const lookup_start = Derived::source_chars.data();
                const char_lookup * const lookup_end = lookup_start + Derived::source_chars.size() - 1;
                const char16_t * const mapped = Derived::mapped_chars;

                auto lower = std::lower_bound(lookup_start, lookup_end, char_lookup{0, src}, [](char_lookup lhs, char_lookup rhs) {
                    return lhs.value < rhs.value;
                });
                if (lower == lookup_end || lower->value != src) 
                    return write<Enc>(src, dest); 
                auto start = lower->offset;
                auto end = (++lower)->offset; //safe - there is one behind end
                return write<Enc>(mapped + start, mapped + end, dest);
            }
        private:
            template<utf_encoding Enc, class OutIt>
            static auto write(char32_t c, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                if constexpr (Enc == utf32)
                {
                    *dest++ = c;
                    return dest;
                }
                else
                {
                    utf_codepoint_encoder<Enc, false> encoder;
                    encoder.put(c);
                    return std::copy(encoder.begin(), encoder.end(), dest);
                }
            }

            template<utf_encoding Enc, class OutIt>
            static auto write(const char16_t * begin, const char16_t * end, OutIt dest) noexcept(noexcept(*dest++ = utf_char_of<Enc>())) -> OutIt
            {
                if constexpr (Enc == utf16)
                {
                    return std::copy(begin ,end, dest);
                }
                else
                {
                    utf_codepoint_decoder<utf16> decoder;
                    while(begin != end)
                    {
                        decoder.put(*begin++);
                        if (!decoder.done())
                            decoder.put(*begin++); //no need to bounds check, we know end is good
                        dest = write<Enc>(decoder.value(), dest);
                    }
                    return dest;
                }
            }
        };
        '''
        return dedent(ret)

    def print_header(self, name):
        ret = f'''
        class {name} : public mapper<{name}>
        {{
        friend mapper<{name}>;
        private:
            static const std::array<char_lookup, {len(self.mapping) + 1}> source_chars;
            static const char16_t mapped_chars[{self.total_mapped_len + 1}];
        public:
            static constexpr size_t max_mapped_length = {self.max_mapped_len};
            static constexpr size_t data_size = sizeof(source_chars) + sizeof(mapped_chars);
        }};
        '''
        return dedent(ret)

    def print_impl(self, name):
        ret = f'''
            const std::array<char_lookup, {len(self.mapping) + 1}> {name}::source_chars = {{{{
                {indent_insert(self.make_source_chars(), 16)}
            }}}};

            const char16_t {name}::mapped_chars[] = 
                {indent_insert(self.make_values_string(), 16)}
            ;
        '''
        return dedent(ret)