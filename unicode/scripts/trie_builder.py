# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import hashlib

class trie_builder:
    bits_per_fanout = 2
    fanout = 1 << bits_per_fanout
    single_mask = (1 << bits_per_fanout) - 1
    char_mask_start = ((21 // bits_per_fanout) + bool(21 % bits_per_fanout)) * bits_per_fanout

    class node:
        def __init__(self):
            self.children = [None] * trie_builder.fanout
            self.value = 0
            self.hash = None

        def is_leaf(self):
            for child in self.children:
                if child is not None:
                    return False
            return True

        def descend(self, idx):
            ret = self.children[idx]
            if ret is None:
                ret = trie_builder.node()
                self.children[idx] = ret
            return ret
        
        
        def calc_size(self):
            ret = 1
            for child in self.children:
                if child is not None:
                    ret += child.calc_size()
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
        return trie_builder.fanout * bits_per_index + min(bits_per_value_index, bits_per_value)
    
    def entris_count(self):
        return len(self.linear)
    
    def bits_per_value(self):
        return self.max_value.bit_length()
    
    def values_count(self):
        return len(self.values)

    def add_chars(self, start, end, flag):
        
        for c in range(start, end):
            node = self.root
            for bit_start in range(self.char_mask_start, 0, -trie_builder.bits_per_fanout):
                mask = trie_builder.single_mask << (bit_start - trie_builder.bits_per_fanout)
                child_idx = (c & mask) >> (bit_start - trie_builder.bits_per_fanout)
                node = node.descend(child_idx)
            node.value |= flag

    def get_char(self, c):
        start = ((21 // trie_builder.bits_per_fanout) + bool(21 % trie_builder.bits_per_fanout)) * trie_builder.bits_per_fanout
        node = self.root
        for bit_start in range(start, 0, -trie_builder.bits_per_fanout):
            mask = trie_builder.single_mask << (bit_start - trie_builder.bits_per_fanout)
            child_idx = (c & mask) >> (bit_start - trie_builder.bits_per_fanout)
            if node.children[child_idx] is not None:
                node = node.children[child_idx]
            else:
                break
        return node.value

    def make_entries(self):
        bits_per_index = self.bits_per_index()
        ret = '{\n        '
        for idx, entry in enumerate(self.linear):
            if idx > 0:
                ret += ', '
                if idx % 8 == 0:
                    ret += '\n        '
            
            val = (entry[1] << (self.fanout * bits_per_index))
            for idx, index in enumerate(entry[0]):
                val |= index << (bits_per_index * idx)
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
        if bits_per_entry > 64:
            raise RuntimeError("more than 64 bits in entry is currently not supported")
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
                self.linear.append(([terminal_idx] * trie_builder.fanout, value_idx))
                terminals_by_value[value_idx] = terminal_idx
            indices_by_hash[current.hash] = terminal_idx
            return terminal_idx
            
        current_idx = len(self.linear)
        indices_by_hash[current.hash] = current_idx
        self.linear.append(([], 0))
        current_linear = self.linear[current_idx]

        for child in current.children:
            if child is not None:
                current_linear[0].append(self.__calc_linear(child, indices_by_hash, terminals_by_value))
            else:
                terminal_idx = terminals_by_value.get(value_idx)
                if terminal_idx is None:
                    terminal_idx = len(self.linear)
                    self.linear.append(([terminal_idx] * trie_builder.fanout, value_idx))
                    terminals_by_value[value_idx] = terminal_idx
                current_linear[0].append(terminal_idx)
        
        return current_idx


    def __trim(self, current: node):
        for child in current.children:
            if child is not None:
                self.__trim(child)
        if current.children[0] is None or not current.children[0].is_leaf():
            return
        for idx in range(1, len(current.children)):
            if (current.children[idx] is None or not current.children[idx].is_leaf() or 
                current.children[idx].value != current.children[0].value):
                return
        current.value = current.children[0].value
        for i in range(0, trie_builder.fanout):
            current.children[i] = None
        

    def __hash(self, current: node):
        m = hashlib.sha256()
        for idx in range(0, trie_builder.fanout):
            if current.children[idx] is not None:
                current.children[idx] = self.__hash(current.children[idx])
                m.update(current.children[idx].hash)
            else:
                m.update(b'none')
        
        m.update(current.value.to_bytes(4, 'little'))
        current.hash = m.digest()
        existing = self.byhash.get(current.hash)
        if existing is not None:
            return existing
        self.byhash[current.hash] = current
        return current
