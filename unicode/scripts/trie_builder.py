# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import hashlib

class trie_builder:
    class node:
        def __init__(self):
            self.zero = None
            self.one = None
            self.value = 0
            self.hash = None

        def is_leaf(self):
            return self.zero is None and self.one is None

        def descend(self, bit):
            if bit == 0:
                if self.zero is None:
                    self.zero = trie_builder.node()
                return self.zero
            
            if self.one is None:
                self.one = trie_builder.node()
            return self.one
        
        def calc_size(self):
            ret = 1
            if self.zero is not None:
                ret += self.zero.calc_size()
            if self.one is not None:
                ret += self.one.calc_size()
            return ret

    
    def __init__(self):
        self.root = trie_builder.node()
        self.byhash = {}
        self.values = {0:0}
        self.max_value = 0
        self.linear = []

    def bits_per_index(self):
        return len(self.linear).bit_length()
    
    def bits_per_value_index(self):
        return len(self.values).bit_length()
    
    def separate_values(self):
        return self.bits_per_value() > self.bits_per_value_index()

    def bits_per_entry(self):
        bits_per_index = self.bits_per_index()
        bits_per_value_index = self.bits_per_value_index()
        bits_per_value = self.bits_per_value()
        return 2 * bits_per_index + min(bits_per_value_index, bits_per_value)
    
    def entris_count(self):
        return len(self.linear)
    
    def bits_per_value(self):
        return self.max_value.bit_length()
    
    def values_count(self):
        return len(self.values)

    def add_chars(self, start, end, flag):
        for c in range(start, end):
            node = self.root
            for bit_num in range(21, 0, -1):
                bit = (c >> (bit_num - 1)) & 0x1
                node = node.descend(bit)
            node.value |= flag

    def get_char(self, c):
        node = self.root
        for bit_num in range(21, 0, -1):
            bit = (c >> (bit_num - 1)) & 0x1
            if bit == 0:
                if node.zero is not None:
                    node = node.zero
                else:
                    break
            else:
                if node.one is not None:
                    node = node.one
                else:
                    break
        return node.value

    def make_entries(self):
        bits_per_index = self.bits_per_index()
        no_index = (1 << bits_per_index) - 1
        ret = '{\n        '
        for idx, entry in enumerate(self.linear):
            if idx > 0:
                ret += ', '
                if idx % 8 == 0:
                    ret += '\n        '
            val = (entry[2] << (2 * bits_per_index))
            val |= ((entry[1] if entry[1] != -1 else no_index) << bits_per_index)
            val |= (entry[0] if entry[0] != -1 else no_index)
            ret += f'0x{val:08X}'
        ret += '}'
        return ret
    
    def make_values(self):
        ret = '{'
        for idx, value in enumerate(self.values):
            if idx > 0:
                ret += ', '
                if idx % 32 == 0:
                    ret += '\n        '
            ret += f'{value}'
        ret += '}'
        return ret

    def generate(self):
        print(f"Original size: {self.root.calc_size()}")
        self.__trim(self.root)
        print(f"After trim: {self.root.calc_size()}")
        self.root = self.__hash(self.root)
        print(f"After hash: {len(self.byhash)}")
        
        indices_by_hash = {}
        terminals_by_value = {}
        self.__calc_linear(self.root, indices_by_hash, terminals_by_value)

        bits_per_entry = self.bits_per_entry()
        if bits_per_entry > 32:
            raise RuntimeError("more than 32 bits in entry is currently not supported")
        bits_per_entry_type = (1<<(bits_per_entry-1).bit_length())
        bits_per_entry_type = max(8, bits_per_entry_type)
        bytes_per_entry_type = bits_per_entry_type // 8
        total_data_size = len(self.linear) * bytes_per_entry_type
        if self.separate_values():
            bits_per_value = self.bits_per_value()
            bits_per_value_type = (1<<(bits_per_value-1).bit_length())
            bits_per_value_type = max(8, bits_per_value_type)
            bytes_per_value_type = bits_per_value_type // 8
            total_data_size += len(self.values) * bytes_per_value_type

        return total_data_size

    def __calc_linear(self, current: node, indices_by_hash: dict, terminals_by_value: dict):
        current_idx = indices_by_hash.get(current.hash)
        if current_idx is not None:
            return current_idx
        
        value_idx = self.values.get(current.value)
        if value_idx is None:
            value_idx = len(self.values)
            self.values[current.value] = value_idx
            self.max_value = max(self.max_value, current.value)

        if current.is_leaf():
            terminal_idx = terminals_by_value.get(value_idx)
            if terminal_idx is None:
                terminal_idx = len(self.linear)
                self.linear.append([terminal_idx, terminal_idx, value_idx])
                terminals_by_value[value_idx] = terminal_idx
            indices_by_hash[current.hash] = terminal_idx
            return terminal_idx
            
        current_idx = len(self.linear)
        indices_by_hash[current.hash] = current_idx
        self.linear.append([])
        current_linear = self.linear[current_idx]

        for child in (current.zero, current.one):
            if child is not None:
                current_linear.append(self.__calc_linear(child, indices_by_hash, terminals_by_value))
            else:
                terminal_idx = terminals_by_value.get(value_idx)
                if terminal_idx is None:
                    terminal_idx = len(self.linear)
                    self.linear.append([terminal_idx, terminal_idx, value_idx])
                    terminals_by_value[value_idx] = terminal_idx
                current_linear.append(terminal_idx)
        
        current_linear.append(0)

        return current_idx


    def __trim(self, current: node):
        for child in (current.zero, current.one):
            if child is not None:
                self.__trim(child)
        if (current.zero is not None and current.zero.is_leaf() and
            current.one is not None and current.one.is_leaf() and 
            current.zero.value == current.one.value):
            current.value = current.zero.value
            current.zero = None
            current.one = None
        

    def __hash(self, current: node):
        m = hashlib.sha256()
        if current.zero is not None:
            current.zero = self.__hash(current.zero)
            m.update(current.zero.hash)
        else:
            m.update(b'none')
        if current.one is not None:
            current.one = self.__hash(current.one)
            m.update(current.one.hash)
        else:
            m.update(b'none')
        m.update(current.value.to_bytes(4, 'little'))
        current.hash = m.digest()
        existing = self.byhash.get(current.hash)
        if existing is not None:
            return existing
        self.byhash[current.hash] = current
        return current
