# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import argparse
from pathlib import Path
from textwrap import dedent
from common import read_ucd_file, write_file, char_name, parse_char_range, indent_insert
from trie_builder import trie_builder
from lookup_builder import lookup_builder
from case_builder import case_builder
from norm_builder import norm_builder

parser = argparse.ArgumentParser()

parser.add_argument('datadir')
parser.add_argument('hfile')
parser.add_argument('test_grapheme_data_h')
parser.add_argument('test_grapheme_data_15_h')
parser.add_argument('test_normalization_data_h')

args = parser.parse_args()

datadir = Path(args.datadir)
hfile = Path(args.hfile)
testfiles = (
    Path(args.test_grapheme_data_h),
    Path(args.test_grapheme_data_15_h),
    Path(args.test_normalization_data_h)
)

test_cases = ([], [], [])

case_info_builder = case_builder()
norm_info_builder = norm_builder()

whitespaces = lookup_builder()

case_prop_values = {
    'Cased':            (0b01, 'cased'),
    'Case_Ignorable':   (0b10, 'case_ignorable')
}

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

grapheme_cluster_break_prop_builder = trie_builder()

def parse_unicode_data(line:str):
    fields = line.split(';')
    char = int(fields[0].strip(), 16)
    comb_class = fields[3].strip()
    decomp = fields[5].strip()
    uppercase = fields[12].strip()
    lowercase = fields[13].strip()
    if len(uppercase) != 0:
        case_info_builder.set_uppercase(char, [int(uppercase, 16)])
    if len(lowercase) != 0:
        case_info_builder.set_lowercase(char, [int(lowercase, 16)])
    if len(comb_class) != 0:
        norm_info_builder.set_comb_class(char, int(comb_class))
    if len(decomp) != 0 and not decomp.startswith('<'):
        values = [int(x, 16) for x in decomp.split(' ')]
        norm_info_builder.set_decomp(char, values)
        

def parse_case_folding(line):
    (code, status, folding, _) = line.split('; ')
    if status != 'C' and status != 'F':
        return
    code = int(code, 16)
    folding = [int(x, 16) for x in folding.split(' ')]
    case_info_builder.set_folding(code, folding)

def parse_special_casing(line):
    fields = line.split('; ')
    if len(fields) == 6: #has condition
        return
    code = int(fields[0].strip(), 16)
    uppercase = [int(x, 16) for x in fields[3].strip().split(' ')]
    lowercase = [int(x, 16) for x in fields[1].strip().split(' ')]
    if len(uppercase) != 1 or uppercase[0] != code:
        case_info_builder.set_uppercase(code, uppercase)
    if len(lowercase) != 1 or lowercase[0] != code:
        case_info_builder.set_lowercase(code, lowercase)

def parse_properties(line):
    (char_range, prop) = line[:line.index('# ')].split('; ')
    char_range = char_range.strip()
    prop = prop.strip()
    if prop == 'White_Space':
        start, end = parse_char_range(char_range)
        whitespaces.add_chars(start, end)

def parse_derived_properties(line):
    (char_range, props) = line[:line.index('# ')].split('; ', 1)
    char_range = char_range.strip()
    props = tuple(prop.strip() for prop in props.split('; '))
    if len(props) == 1:
        props = props[0]
    if (prop_val := case_prop_values.get(props)) is not None:
        start, end = parse_char_range(char_range)
        case_info_builder.set_props(start, end, prop_val[0])
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

def parse_composition_exclusions(line):
    char = line[:line.index('# ')].rstrip()
    code = int(char, 16)
    norm_info_builder.set_exclusion(code)

def parse_normalization_props(line):
    parts = line[:line.index(' #')].split('; ')
    if len(parts) != 3:
        return
    char_range, prop, val = parts
    prop = prop.strip()
    if prop == 'NFC_QC':
        val = val.strip()
        if val in ('N', 'M'):
            char_range = char_range.strip()
            start, end = parse_char_range(char_range)
            for char in range(start, end):
                norm_info_builder.set_nfc_qc_not_yes(char) 

def parse_grapheme_tests(dest, line):
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
    dest.append((data, comment, source, expected))

def parse_normalization_tests(dest, line):
    if line.startswith('@'):
        return
    comment_start = line.index('# ')
    data = line[:comment_start].strip()
    comment = line[comment_start + 1:].strip()
    entries = data.split(';')
    values = []
    for entry in entries[0:5]:
        chars = [char_name(int(code.strip(), 16)) for code in entry.split(' ')]
        values.append(''.join(chars))
    dest.append((values, comment))
    

def print_enum(mappings, masks=None):
    if masks is None:
        masks = {}
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


def make_grapheme_tests(tests):
    ret =''
    for test in tests:
        data, comment, source, expected = test
        ret += f'//{data}\n//{comment}\ncheck_graphemes(U"{source}", {{'
        for idx, exp in enumerate(expected):
            if idx != 0:
                ret += ', '
            ret += f'U"{exp}"'
        ret += '});\n'
    return ret

def make_normalization_tests(tests):
    ret = dedent('''
    static struct 
    {
        const char32_t * src;
        const char32_t * nfc;
        const char32_t * nfd;
        const char32_t * nfkc;
        const char32_t * nfkd;
    } data[] = {
    ''')
    for idx, test in enumerate(tests):
        values, comment = test
        source, NFC, NFD, NFKC, NFKD = values
        ret += f'    //{idx}: {comment}\n'
        ret += f'    {{ U"{source}", U"{NFC}", U"{NFD}", U"{NFKC}", U"{NFKD}" }},\n'
    
    ret += dedent('''
    };
    
    for (size_t i = 0; i != std::size(data); ++i)
    {
        auto & entry = data[i];
        check_generated(i, entry.src, entry.nfc, entry.nfd, entry.nfkc, entry.nfkd);
    }
    ''')
    return ret

def main():
    read_ucd_file(datadir/'UnicodeData.txt', parse_unicode_data)
    read_ucd_file(datadir/'CaseFolding.txt', parse_case_folding)
    read_ucd_file(datadir/'SpecialCasing.txt', parse_special_casing)
    read_ucd_file(datadir/'PropList.txt', parse_properties)
    read_ucd_file(datadir/'DerivedCoreProperties.txt', parse_derived_properties)
    read_ucd_file(datadir/'GraphemeBreakProperty.txt', parse_grapheme_cluster_break_properties)
    read_ucd_file(datadir/'emoji-data.txt', parse_emoji_data)
    read_ucd_file(datadir/'CompositionExclusions.txt', parse_composition_exclusions)
    read_ucd_file(datadir/'DerivedNormalizationProps.txt', parse_normalization_props)
    read_ucd_file(datadir/'GraphemeBreakTest.txt', lambda line: parse_grapheme_tests(test_cases[0], line))
    read_ucd_file(datadir/'GraphemeBreakTest-15.txt', lambda line: parse_grapheme_tests(test_cases[1], line))
    read_ucd_file(datadir/'NormalizationTest.txt', lambda line: parse_normalization_tests(test_cases[2], line))


    total_data_size = 0
    total_data_size += case_info_builder.generate()
    total_data_size += norm_info_builder.generate()
    total_data_size += whitespaces.generate()
    total_data_size += grapheme_cluster_break_prop_builder.generate()

    write_file(hfile, f'''//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_UNICODE_MAPPINGS_H_INCLUDED
#define HEADER_SYS_STRING_UNICODE_MAPPINGS_H_INCLUDED

#include <sys_string/impl/unicode/mappings_common.h>
           
#include <algorithm>
#include <cstdlib>
#include <climits>
#include <array>
#include <cassert>

namespace sysstr::util::unicode 
{{
    {indent_insert(whitespaces.print_header('is_whitespace'), 4)}

    {indent_insert(case_info_builder.print_header(), 4)}

    {indent_insert(norm_info_builder.print_header(), 4)}

    {indent_insert(grapheme_cluster_break_prop_builder.print_header("grapheme_cluster_break_prop", 
                        print_enum((grapheme_cluster_break_prop_values, grapheme_related_emoji_values, grapheme_related_prop_values), 
                                    grapheme_masks)),
                   4)}


    {indent_insert(whitespaces.print_impl('is_whitespace'), 4)}
    {indent_insert(case_info_builder.print_impl(), 4)}
    {indent_insert(norm_info_builder.print_impl(), 4)}
    {indent_insert(grapheme_cluster_break_prop_builder.print_impl("grapheme_cluster_break_prop"), 4)}
    
    constexpr auto total_data_size = 
        is_whitespace::data_size +
        case_mapper::data_size +
        normalizer::data_size +
        grapheme_cluster_break_prop::data_size;
    static_assert(total_data_size == {total_data_size});
}}

#endif

''')      

    for testfile, tests in zip(testfiles[:2], test_cases[:2]):
        write_file(testfile, f'''
//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

{make_grapheme_tests(tests)}

'''.lstrip())
        
    for testfile, tests in zip(testfiles[2:3], test_cases[2:3]):
        write_file(testfile, f'''
//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

{make_normalization_tests(tests)}

'''.lstrip())

if __name__ == '__main__':
    main()
