
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

def read_ucd_file(scrpath, handler):
    with open(scrpath, 'r') as src:
        for line in src:
            line = line.strip()
            if len(line) == 0 or line.startswith('#'):
                continue
            handler(line)

def write_file(dstpath, content):
    if dstpath.exists():
        with open(dstpath, 'r') as dst:
            existing = dst.read()
            if content == existing:
                dstpath.touch()
                return
                
    dstpath.parent.mkdir(parents=True, exist_ok=True)
    with open(dstpath, 'w') as dst:
        dst.write(content)
