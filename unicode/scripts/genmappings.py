# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import argparse
from pathlib import Path
from common import read_ucd_file, write_file, char_name, parse_char_range, indent_insert
from mapping_builder import mapping_builder
from table_builder import table_builder
from trie_builder import trie_builder

parser = argparse.ArgumentParser()

parser.add_argument('-c', '--comp', type=int, dest='compression', choices=range(0, 8), default=4)
parser.add_argument('datadir')
parser.add_argument('cppfile')
parser.add_argument('hfile')
parser.add_argument('testfile')

args = parser.parse_args()

datadir = Path(args.datadir)
cppfile = Path(args.cppfile)
hfile = Path(args.hfile)
testfile = Path(args.testfile)

if args.compression != 0:
    trie_builder.set_bits_per_fanout(8 - args.compression)

def make_prop_builder(separate_values=False):
    if args.compression == 0:
        return table_builder(separate_values)
    return trie_builder()

total_data_size = 0

folding_builder = mapping_builder()
uppercase_builder = mapping_builder()
lowercase_builder = mapping_builder()

whitespaces = []

case_prop_values = {
    'Cased':            (0b01, 'cased'),
    'Case_Ignorable':   (0b10, 'case_ignorable')
}

case_prop_builder = make_prop_builder()

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
    'Extended_Pictographic':    (0xB, 'extended_pictographic')
}

grapheme_masks = {
    'basic_mask': 0x0F,
    'in_cb_mask': 0x30
}

grapheme_cluster_break_prop_builder = make_prop_builder(separate_values=True)

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
    (code, status, folding, _) = line.split('; ')
    if status != 'C' and status != 'F':
        return
    code = int(code, 16)
    folding = [int(x, 16) for x in folding.split(' ')]
    folding_builder.set_values(code, folding)

def parse_special_casing(line):
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
                ret += ',\n'
            first = False
            ret += f'{name} = {val}'
    if len(masks) != 0:
        ret += ',\n\n'
        first = True
        for name, val in masks.items():
            if not first:
                ret += ',\n'
            first = False
            ret += f'{name} = {val}'
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
    ret =''
    for test in grapheme_tests:
        data, comment, source, expected = test
        ret += f'//{data}\n//{comment}\ncheck_graphemes(U"{source}", {{'
        for idx, exp in enumerate(expected):
            if idx != 0:
                ret += ', '
            ret += f'U"{exp}"'
        ret += '});\n'
    return ret

read_ucd_file(datadir/'UnicodeData.txt', parse_case_info)
read_ucd_file(datadir/'CaseFolding.txt', parse_case_folding)
read_ucd_file(datadir/'SpecialCasing.txt', parse_special_casing)
read_ucd_file(datadir/'PropList.txt', parse_properties)
read_ucd_file(datadir/'DerivedCoreProperties.txt', parse_derived_properties)
read_ucd_file(datadir/'GraphemeBreakProperty.txt', parse_grapheme_cluster_break_properties)
read_ucd_file(datadir/'emoji-data.txt', parse_emoji_data)
read_ucd_file(datadir/'GraphemeBreakTest.txt', parse_grapheme_tests)

total_data_size += folding_builder.generate()
total_data_size += uppercase_builder.generate()
total_data_size += lowercase_builder.generate()
total_data_size += case_prop_builder.generate()
total_data_size += grapheme_cluster_break_prop_builder.generate()

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

#include <array>
#include <cassert>

namespace sysstr::util::unicode 
{{
    struct mapper_data
    {{
        static constexpr size_t max_mapped_length = {mapping_builder.max_len};
    }};

    struct char_lookup
    {{
        char32_t offset:11;
        char32_t value:21;
    }};

    {indent_insert(folding_builder.print_header('case_fold_data'), 4)}

    {indent_insert(uppercase_builder.print_header('to_lower_case_data'), 4)}

    {indent_insert(lowercase_builder.print_header('to_upper_case_data'), 4)}

    {indent_insert(case_prop_builder.__class__.print_common_header(), 4)}

    {indent_insert(case_prop_builder.print_header("case_prop_lookup", print_enum(case_prop_values)), 4)}

    {indent_insert(grapheme_cluster_break_prop_builder.print_header("grapheme_cluster_break_lookup", 
                        print_enum((grapheme_cluster_break_prop_values, grapheme_related_emoji_values, grapheme_related_prop_values), 
                                    grapheme_masks)),
                   4)}
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

    {indent_insert(folding_builder.print_impl('case_fold_data'), 4)}

    {indent_insert(uppercase_builder.print_impl('to_lower_case_data'), 4)}

    {indent_insert(lowercase_builder.print_impl('to_upper_case_data'), 4)}
    
    extern const char16_t whitespaces[] = 
        u"{make_whitespaces()}";

    {indent_insert(case_prop_builder.print_impl("case_prop_lookup"), 4)}
    {indent_insert(grapheme_cluster_break_prop_builder.print_impl("grapheme_cluster_break_lookup"), 4)}
    
    constexpr auto total_data_size = 
        sizeof(case_fold_data::source_chars) + sizeof(case_fold_data::chars) +
        sizeof(to_lower_case_data::source_chars) + sizeof(to_lower_case_data::chars) +
        sizeof(to_upper_case_data::source_chars) + sizeof(to_upper_case_data::chars) +
        sizeof(whitespaces) +
        case_prop_lookup::data_size +
        grapheme_cluster_break_lookup::data_size;
    static_assert(total_data_size == {total_data_size});

}}
''')

write_file(testfile, f'''//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

{make_grapheme_tests()}

''')

