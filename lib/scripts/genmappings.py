# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 
import sys
import re
from pathlib import Path
from common import read_ucd_file, write_file, char_name, parse_char_range

datadir = Path(sys.argv[1])
cppfile = Path(sys.argv[2])


folding_char_info = {} 
uppercase_char_info = {} 
lowercase_char_info = {}
max_mappings_len = 1

whitespaces = []

prop_values = {
    'Cased':            0b01,
    'Case_Ignorable':   0b10
}

total_data_size = 0

class table_builder:
    block_size = 256
    count_per_byte = 4

    def __init__(self):
        self.stage1 = []
        self.blocks = {}
        self.blocks_by_index = []
        self.max_known_char = 0

    def add_chars(self, start, end, flag):
        for c in range(start, end):
            bucket = c // self.block_size
            while bucket >= len(self.stage1):
                self.stage1.append([0] * self.block_size)
            in_block_idx = c % self.block_size
            self.stage1[bucket][in_block_idx] |= flag
        self.max_known_char = end - 1

    def generate(self):
        for idx, block in enumerate(self.stage1):
            block = tuple(block)
            block_idx = self.blocks.get(block)
            if block_idx is None:
                block_idx = len(self.blocks_by_index)
                self.blocks[block] = block_idx
                self.blocks_by_index.append(block)
            self.stage1[idx] = block_idx

    def make_stage1(self):
        global total_data_size
        ret = '{\n        '
        for idx, block_idx in enumerate(self.stage1):
            if idx > 0:
                ret += ', '
                if idx % 32 == 0:
                    ret += '\n        '
            ret += f'{block_idx}'
            total_data_size += 1
        ret += '}'
        return ret

    def make_stage2(self):
        global total_data_size
        ret = '{'
        for block_idx, block in enumerate(self.blocks_by_index):
            if block_idx > 0:
                ret += ','
            ret += '\n        '
            for idx in range(0, len(block), self.count_per_byte):
                if idx > 0:
                    ret += ', '
                val = 0
                for i in range(0, self.count_per_byte):
                    val = val << (8 // self.count_per_byte)
                    val = val | block[idx + i]
                ret += f'0x{val:02X}'
                total_data_size += 1
        ret += '\n    }'
        return ret

prop_builder = table_builder()

def parse_case_info(line):
    fields = line.split(';')
    char = int(fields[0].strip(), 16)
    uppercase = fields[12].strip()
    lowercase = fields[13].strip()
    if len(uppercase) != 0:
        lowercase_char_info[char] = [int(uppercase, 16)]
    if len(lowercase) != 0:
        uppercase_char_info[char] = [int(lowercase, 16)]
        

def parse_case_folding(line):
    global max_mappings_len
    (code, status, folding, _) = line.split('; ')
    if status != 'C' and status != 'F':
        return
    code = int(code, 16)
    folding = [int(x, 16) for x in folding.split(' ')]
    if len(folding) > max_mappings_len:
        max_mappings_len = len(folding)
    folding_char_info[code] = folding

def parse_special_casing(line):
    global max_mappings_len
    fields = line.split('; ')
    if len(fields) == 6: #has condition
        return
    code = int(fields[0].strip(), 16)
    uppercase = [int(x, 16) for x in fields[3].strip().split(' ')]
    lowercase = [int(x, 16) for x in fields[1].strip().split(' ')]
    if len(uppercase) != 1 or uppercase[0] != code:
        if len(uppercase) > max_mappings_len:
            max_mappings_len = len(uppercase)
        lowercase_char_info[code] = uppercase
    if len(lowercase) != 1 or lowercase[0] != code:
        if len(lowercase) > max_mappings_len:
            max_mappings_len = len(lowercase)
        uppercase_char_info[code] = lowercase

def parse_properties(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    if prop == 'White_Space':
        start, end = parse_char_range(char_range)
        for char in range(start, end):
            whitespaces.append(char)

def parse_derived_properties(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    prop_val = prop_values.get(prop)
    if not prop_val is None:
        start, end = parse_char_range(char_range)
        prop_builder.add_chars(start, end, prop_val)
            

def make_source_chars(char_info):
    global total_data_size 
    ret = '{\n        '
    range_start = 0
    for idx, code in enumerate(sorted(char_info.keys())):
        if idx > 0:
            ret += ', '
            if idx % 16 == 0:
                ret += '\n        '

        ret += f"{{{range_start} ,U'{char_name(code)}'}}"
        total_data_size += 4

        chars = char_info[code]
        range_len = 0
        for char in chars:
            range_len += (1 if char < 0x10000 else 2)
        range_start += range_len

    ret += f", {{{range_start} , 0}}"
    total_data_size += 4

    ret += '\n    }'
    return ret

def make_values_string(char_info):
    global total_data_size 
    ret = '\n        u"'
    char_count = 0
    for char in sorted(char_info.keys()):
        values = char_info[char]
        for value in values:
            ret += char_name(value)
            char_count += 1
            total_data_size += 2 if value < 0x10000 else 4
            if char_count > 0 and char_count % 16 == 0:
                ret += '"\n        u"'
    ret += '"'
    total_data_size += 2
    return ret

def make_index(char_info):
    global total_data_size 
    ret = ''
    for idx, code in enumerate(sorted(char_info.keys())):
        if idx > 0:
            ret += ', '
            if idx % 8 == 0:
                ret += '\n        '

        chars = char_info[code]
        range_len = 0
        for char in chars:
            range_len += (1 if char < 0x10000 else 2)
        ret += f'{range_len}'
        total_data_size += 1
    return ret

def make_whitespaces():
    global total_data_size 
    ret = ''
    char_count = 0
    for char in whitespaces:
        ret += char_name(char)
        char_count += 1
        total_data_size += 2 if char < 0x10000 else 4
        if char_count > 0 and char_count % 16 == 0:
            ret += '"\n        u"'
    total_data_size += 2
    return ret

read_ucd_file(datadir/'UnicodeData.txt', parse_case_info)
read_ucd_file(datadir/'CaseFolding.txt', parse_case_folding)
read_ucd_file(datadir/'SpecialCasing.txt', parse_special_casing)
read_ucd_file(datadir/'PropList.txt', parse_properties)
read_ucd_file(datadir/'DerivedCoreProperties.txt', parse_derived_properties)

prop_builder.generate()
print(f'{prop_builder.block_size}: {len(prop_builder.stage1)}, {len(prop_builder.blocks)}, {len(prop_builder.stage1) + len(prop_builder.blocks) * prop_builder.block_size // prop_builder.count_per_byte}')


write_file(cppfile, f'''//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

#include <sys_string/impl/unicode/mappings.h>

namespace sysstr::util::unicode 
{{

    static_assert(mapper::max_mapped_length >= {max_mappings_len});
    static_assert(props_block_len == {prop_builder.block_size});
    static_assert(props_count_per_byte == {prop_builder.count_per_byte});

    const char_lookup folding_source_chars[] = {make_source_chars(folding_char_info)};

    constexpr size_t folding_source_chars_length = {len(folding_char_info)};

    const char16_t case_folded_chars[] = {make_values_string(folding_char_info)};

    const char_lookup uppercase_source_chars[] = {make_source_chars(uppercase_char_info)};

    constexpr size_t uppercase_source_chars_length = {len(uppercase_char_info)};

    const char16_t lowercase_chars[] = {make_values_string(uppercase_char_info)};

    const char_lookup lowercase_source_chars[] = {make_source_chars(lowercase_char_info)};

    constexpr size_t lowercase_source_chars_length = {len(lowercase_char_info)};

    const char16_t uppercase_chars[] = {make_values_string(lowercase_char_info)};

    
    const mapper mapper::case_fold(folding_source_chars, folding_source_chars_length, case_folded_chars);
    const mapper mapper::to_lower_case(uppercase_source_chars, uppercase_source_chars_length, lowercase_chars);
    const mapper mapper::to_upper_case(lowercase_source_chars, lowercase_source_chars_length, uppercase_chars);

    extern const char16_t whitespaces[] = 
        u"{make_whitespaces()}";

    extern const char32_t max_props_char = U'{char_name(prop_builder.max_known_char)}';
    extern const uint8_t props_stage1[] = {prop_builder.make_stage1()};

    extern const uint8_t props_stage2[] = {prop_builder.make_stage2()};

    constexpr auto total_data_size = 
        sizeof(folding_source_chars) + 
        sizeof(uppercase_source_chars) + 
        sizeof(lowercase_source_chars) + 
        sizeof(case_folded_chars) +
        sizeof(uppercase_chars) + 
        sizeof(lowercase_chars) + 
        sizeof(whitespaces) +
        sizeof(props_stage1) + 
        sizeof(props_stage2);
    static_assert(total_data_size == {total_data_size});

}}
''')
