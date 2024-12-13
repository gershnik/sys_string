# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 
import sys
from pathlib import Path
from common import read_ucd_file, write_file, char_name, parse_char_range
from table_builder import table_builder

datadir = Path(sys.argv[1])
cppfile = Path(sys.argv[2])
hfile = Path(sys.argv[3])

total_data_size = 0

class mapping_builder:
    max_len = 1

    def __init__(self) -> None:
        self.mapping = {}

    def set_values(self, char, values):
        self.mapping[char] = values
        mapping_builder.max_len = max(mapping_builder.max_len, len(values))


folding_builder = mapping_builder()
uppercase_builder = mapping_builder()
lowercase_builder = mapping_builder()

whitespaces = []

case_prop_values = {
    'Cased':            (0b01, 'cased'),
    'Case_Ignorable':   (0b10, 'case_ignorable')
}

case_prop_builder = table_builder()

grapheme_cluster_break_prop_values = {
    'Control':               (1, 'control'),
    'Extend':                (2, 'extend'),
    'Regional_Indicator':    (3, 'regional_indicator'),
    'Prepend':               (4, 'prepend'),
    'SpacingMark':           (5, 'spacing_mark'),
    'L':                     (6, 'hangul_l'),
    'V':                     (7, 'hangul_v'),
    'T':                     (8, 'hangul_t'),
    'LV':                    (9, 'hangul_lv'),
    'LVT':                 (0xA, 'hangul_lvt')
}

grapheme_related_prop_values = {
    ('InCB', 'Consonant'): (0x10, 'in_cb_consonant'),
    ('InCB', 'Extend'):    (0x20, 'in_cb_extend'),
    ('InCB', 'Linker'):    (0x30, 'in_cb_linker'),
}

grapheme_related_emoji_values = {
    'Extended_Pictographic':    (0x40, 'extended_pictographic')
}

grapheme_masks = {
    'basic_mask': 0x0F,
    'in_cb_mask': 0x30
}

grapheme_cluster_break_prop_builder = table_builder(separate_values=True)


def parse_case_info(line):
    fields = line.split(';')
    char = int(fields[0].strip(), 16)
    uppercase = fields[12].strip()
    lowercase = fields[13].strip()
    if len(uppercase) != 0:
        lowercase_builder.set_values(char, [int(uppercase, 16)])
    if len(lowercase) != 0:
        uppercase_builder.set_values(char, [int(lowercase, 16)])
        

def parse_case_folding(line):
    global max_mappings_len
    (code, status, folding, _) = line.split('; ')
    if status != 'C' and status != 'F':
        return
    code = int(code, 16)
    folding = [int(x, 16) for x in folding.split(' ')]
    folding_builder.set_values(code, folding)

def parse_special_casing(line):
    global max_mappings_len
    fields = line.split('; ')
    if len(fields) == 6: #has condition
        return
    code = int(fields[0].strip(), 16)
    uppercase = [int(x, 16) for x in fields[3].strip().split(' ')]
    lowercase = [int(x, 16) for x in fields[1].strip().split(' ')]
    if len(uppercase) != 1 or uppercase[0] != code:
        lowercase_builder.set_values(code, uppercase)
    if len(lowercase) != 1 or lowercase[0] != code:
        uppercase_builder.set_values(code, lowercase)

def parse_properties(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    if prop == 'White_Space':
        start, end = parse_char_range(char_range)
        for char in range(start, end):
            whitespaces.append(char)

def parse_derived_properties(line):
    (char_range, props) = line[:line.index('# ')].split('; ', 1)
    char_range = char_range.strip()
    props = tuple(prop.strip() for prop in props.split('; '))
    if len(props) == 1:
        props = props[0]
    if (prop_val := case_prop_values.get(props)) is not None:
        start, end = parse_char_range(char_range)
        case_prop_builder.add_chars(start, end, prop_val[0])
    elif (prop_val := grapheme_related_prop_values.get(props)) is not None:
        start, end = parse_char_range(char_range)
        grapheme_cluster_break_prop_builder.add_chars(start, end, prop_val[0])


def parse_grapheme_cluster_break_properties(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    prop_val = grapheme_cluster_break_prop_values.get(prop)
    if not prop_val is None:
        start, end = parse_char_range(char_range)
        grapheme_cluster_break_prop_builder.add_chars(start, end, prop_val[0])

def parse_emoji_data(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    prop_val = grapheme_related_emoji_values.get(prop)
    if not prop_val is None:
        start, end = parse_char_range(char_range)
        grapheme_cluster_break_prop_builder.add_chars(start, end, prop_val[0])

grapheme_tests = []
def parse_grapheme_tests(line):
    global grapheme_tests

    comment_start = line.index('# ')
    data = line[:comment_start].strip()
    comment = line[comment_start + 1:].strip()
    graphemes = data.split('÷')
    source = ''
    expected = []
    for g in graphemes:
        g = g.strip()
        if len(g) == 0:
            continue
        cur_expected = ''
        letters = g.split('×')
        for l in letters:
            l = l.strip()
            code = int(l, 16)
            l = char_name(code)
            source += l
            cur_expected += l
        expected.append(cur_expected)
    grapheme_tests.append((data, comment, source, expected))
    



def print_enum(mappings, masks={}):
    ret = ''
    if isinstance(mappings, dict):
        mappings = [mappings]

    first = True
    for mapping in mappings:
        for val, name in mapping.values():
            if not first:
                ret += ',\n            '
            first = False
            ret += f'{name} = {val}'
    if len(masks) != 0:
        ret += ',\n            '
        ret += '\n            '
        first = True
        for name, val in masks.items():
            if not first:
                ret += ',\n            '
            first = False
            ret += f'{name} = {val}'
    return ret

def print_properties_header(name, builder, enums, masks={}):
    ret = f'''
    class {name}
    {{
    protected:
        static constexpr size_t block_len = {builder.block_size};
        static constexpr char32_t max_char = U'{char_name(builder.max_known_char)}';

        static const bit_array<{builder.stage1_bits_per_value()}, {builder.stage1_size()}> stage1;
        static const bit_array<{builder.stage2_bits_per_value()}, {builder.stage2_size()}> stage2;
    '''

    if builder.separate_values:
        ret += f'''
        static const bit_array<{builder.values_bits_per_value()}, {builder.values_size()}> values;

        static constexpr bool separate_values = true;
    '''
    else:
        ret += '''
        static constexpr bool separate_values = false;
    '''

    ret += f'''
    public:
        enum value : decltype(stage2)::value_type
        {{
            none = 0,
            {print_enum(enums, masks)}
        }};
    '''

    if builder.separate_values:
        ret += '''
        static constexpr size_t data_size = sizeof(stage1) + sizeof(stage2) + sizeof(values);
    '''
    else:
        ret += '''
        static constexpr size_t data_size = sizeof(stage1) + sizeof(stage2);
    '''
    
    ret += '''
    };
    '''

    return ret

def print_properties_impl(name, builder):
    ret = f'''
    const bit_array<{builder.stage1_bits_per_value()}, {builder.stage1_size()}> {name}::stage1({builder.make_stage1()});

    const bit_array<{builder.stage2_bits_per_value()}, {builder.stage2_size()}> {name}::stage2({builder.make_stage2()});

    '''

    if builder.separate_values:
        ret += f'''
    const bit_array<{builder.values_bits_per_value()}, {builder.values_size()}> {name}::values({builder.make_values()});
    '''
        
    return ret

def make_source_chars(builder: mapping_builder):
    global total_data_size 
    ret = '{\n        '
    range_start = 0
    for idx, code in enumerate(sorted(builder.mapping.keys())):
        if idx > 0:
            ret += ', '
            if idx % 16 == 0:
                ret += '\n        '

        ret += f"{{{range_start} ,U'{char_name(code)}'}}"
        total_data_size += 4

        chars = builder.mapping[code]
        range_len = 0
        for char in chars:
            range_len += (1 if char < 0x10000 else 2)
        range_start += range_len

    ret += f", {{{range_start} , 0}}"
    total_data_size += 4

    ret += '\n    }'
    return ret

def make_values_string(builder: mapping_builder):
    global total_data_size 
    ret = '\n        u"'
    char_count = 0
    for char in sorted(builder.mapping.keys()):
        values = builder.mapping[char]
        for value in values:
            ret += char_name(value)
            char_count += 1
            total_data_size += 2 if value < 0x10000 else 4
            if char_count > 0 and char_count % 16 == 0:
                ret += '"\n        u"'
    ret += '"'
    total_data_size += 2
    return ret

def make_index(builder: mapping_builder):
    global total_data_size 
    ret = ''
    for idx, code in enumerate(sorted(builder.mapping.keys())):
        if idx > 0:
            ret += ', '
            if idx % 8 == 0:
                ret += '\n        '

        chars = builder.mapping[code]
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

def make_grapheme_tests():
    global grapheme_tests
    ret =''
    for test in grapheme_tests:
        data, comment, source, expected = test
        ret += f'//{data}\n//{comment}\ncheck_graphemes(U"{source}", {{'
        for idx, exp in enumerate(expected):
            if idx != 0:
                ret += ', '
            ret += f'U"{exp}"'
        ret += f'}});\n'
    return ret

read_ucd_file(datadir/'UnicodeData.txt', parse_case_info)
read_ucd_file(datadir/'CaseFolding.txt', parse_case_folding)
read_ucd_file(datadir/'SpecialCasing.txt', parse_special_casing)
read_ucd_file(datadir/'PropList.txt', parse_properties)
read_ucd_file(datadir/'DerivedCoreProperties.txt', parse_derived_properties)
read_ucd_file(datadir/'GraphemeBreakProperty.txt', parse_grapheme_cluster_break_properties)
read_ucd_file(datadir/'emoji-data.txt', parse_emoji_data)
read_ucd_file(datadir/'GraphemeBreakTest.txt', parse_grapheme_tests)

total_data_size += case_prop_builder.generate()
total_data_size += grapheme_cluster_break_prop_builder.generate()
#print(f'{prop_builder.block_size}: {len(prop_builder.stage1)}, {len(prop_builder.blocks)}, {len(prop_builder.stage1) + len(prop_builder.blocks) * prop_builder.block_size // prop_builder.count_per_byte}')


write_file(hfile, f'''//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_DATA_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_DATA_H_INCLUDED
           
namespace sysstr::util::unicode 
{{
    struct mapper_data
    {{
        static constexpr size_t max_mapped_length = {mapping_builder.max_len};
    }};

    {print_properties_header("case_prop_data", case_prop_builder, case_prop_values)}
    {print_properties_header("grapheme_cluster_break_prop_data", grapheme_cluster_break_prop_builder, 
                             (grapheme_cluster_break_prop_values, grapheme_related_prop_values, grapheme_related_emoji_values), 
                             grapheme_masks)}
}}

#endif

''')      


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

    const char_lookup folding_source_chars[] = {make_source_chars(folding_builder)};

    constexpr size_t folding_source_chars_length = {len(folding_builder.mapping)};

    const char16_t case_folded_chars[] = {make_values_string(folding_builder)};

    const char_lookup uppercase_source_chars[] = {make_source_chars(uppercase_builder)};

    constexpr size_t uppercase_source_chars_length = {len(uppercase_builder.mapping)};

    const char16_t lowercase_chars[] = {make_values_string(uppercase_builder)};

    const char_lookup lowercase_source_chars[] = {make_source_chars(lowercase_builder)};

    constexpr size_t lowercase_source_chars_length = {len(lowercase_builder.mapping)};

    const char16_t uppercase_chars[] = {make_values_string(lowercase_builder)};

    
    const mapper mapper::case_fold(folding_source_chars, folding_source_chars_length, case_folded_chars);
    const mapper mapper::to_lower_case(uppercase_source_chars, uppercase_source_chars_length, lowercase_chars);
    const mapper mapper::to_upper_case(lowercase_source_chars, lowercase_source_chars_length, uppercase_chars);

    extern const char16_t whitespaces[] = 
        u"{make_whitespaces()}";

    {print_properties_impl("case_prop_data", case_prop_builder)}
    {print_properties_impl("grapheme_cluster_break_prop_data", grapheme_cluster_break_prop_builder)}
    
    constexpr auto total_data_size = 
        sizeof(folding_source_chars) + 
        sizeof(uppercase_source_chars) + 
        sizeof(lowercase_source_chars) + 
        sizeof(case_folded_chars) +
        sizeof(uppercase_chars) + 
        sizeof(lowercase_chars) + 
        sizeof(whitespaces) +
        case_prop_data::data_size +
        grapheme_cluster_break_prop_data::data_size;
    static_assert(total_data_size == {total_data_size});

}}
''')

write_file(datadir / ('../../test/test_grapheme_data.h'), 
           f'''//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

{make_grapheme_tests()}

''')

