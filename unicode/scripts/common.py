
# 
# Copyright 2020 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 
def char_name(code):
    if code < 0x10000:
        return f"\\u{code:04X}"
    else:
        return f"\\U{code:08X}"

def parse_char_range(text):
    char_range = [int(x, 16) for x in text.split('..')]
    start = char_range[0]
    end = char_range[len(char_range) - 1] + 1
    return (start, end)

def type_for_bits(bits: int):
    if bits <= 8:
        return 'uint8_t'
    if bits <= 16:
        return 'uint16_t'
    if bits <= 32:
        return 'uint32_t'
    if bits <= 64:
        return 'uint64_t'
    raise RuntimeError('too many bits')

def bytes_for_bits(bits: int):
    bits_per_value = (1<<(bits-1).bit_length())
    bits_per_value = max(8, bits_per_value)
    bytes_per_value = bits_per_value // 8
    return bytes_per_value

def read_ucd_file(srcpath, handler):
    with open(srcpath, 'r', encoding='utf-8') as src:
        line_idx = 0
        for line in src:
            line_idx += 1
            try:
                line = line.strip()
                if len(line) == 0 or line.startswith('#'):
                    continue
                handler(line)
            except Exception as ex:
                if getattr(ex, 'add_note', None):
                    ex.add_note(f'in {srcpath} on line {line_idx}')
                    raise
                raise RuntimeError(f'failed to parse {srcpath} on line {line_idx}') from ex

def write_file(dstpath, content):
    if dstpath.exists():
        with open(dstpath, 'r', encoding='utf-8') as dst:
            existing = dst.read()
            if content == existing:
                dstpath.touch()
                return
                
    dstpath.parent.mkdir(parents=True, exist_ok=True)
    with open(dstpath, 'w', encoding='utf-8') as dst:
        dst.write(content)
