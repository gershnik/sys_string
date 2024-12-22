# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import hashlib

from common import bytes_for_bits

class trie_builder:
    bits_per_fanout = 3
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
        self.linear = [[0]*self.fanout]

    def bits_per_index(self):
        return len(self.linear).bit_length()
    
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
        ret = '{\n        '
        for idx, entry in enumerate(self.linear):
            if idx > 0:
                ret += ', '
                if idx % 8 == 0:
                    ret += '\n        '
            
            indices = ', '.join([f'{val}' for val in entry])
            ret += f'{{{indices}}}'
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
        
        self.__collect_values(self.root)
        indices_by_hash = {}
        self.__calc_linear(self.root, indices_by_hash)

        bytes_per_entry = bytes_for_bits(self.bits_per_index()) * self.fanout
        total_data_size = len(self.linear) * bytes_per_entry
        
        bytes_per_value = bytes_for_bits(self.bits_per_value())
        total_data_size += len(self.values) * bytes_per_value

        return total_data_size
    
    def __collect_values(self, current: node):
        value_idx = self.values.get(current.value)
        if value_idx is None:    
            value_idx = len(self.values)
            self.values[current.value] = value_idx
            self.max_value = max(self.max_value, current.value)
            terminal_idx = len(self.linear)
            self.linear.append([terminal_idx] * trie_builder.fanout)

        for child in current.children:
            if child is not None:
                self.__collect_values(child)


    def __calc_linear(self, current: node, indices_by_hash: dict):
        current_idx = indices_by_hash.get(current.hash)
        if current_idx is not None:
            return current_idx
        
        value_idx = self.values.get(current.value)

        if current.is_leaf():
            indices_by_hash[current.hash] = value_idx
            return value_idx
            
        current_idx = len(self.linear)
        indices_by_hash[current.hash] = current_idx
        self.linear.append([])
        current_linear: list = self.linear[current_idx]

        for child in current.children:
            if child is not None:
                current_linear.append(self.__calc_linear(child, indices_by_hash))
            else:
                current_linear.append(value_idx)
        
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
