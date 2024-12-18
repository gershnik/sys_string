# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

class table_builder:
    block_size = 256

    def __init__(self, separate_values=False):
        self.stage1 = []
        self.blocks = {}
        self.blocks_by_index = []

        self.max_known_char = 0
        self.max_stage2_value = 0
        self.max_block_idx = 0

        self.separate_values = separate_values
        if separate_values:
            self.values = [0]
            self.value_idx_by_value = {0: 0}
            self.max_value = 0

        self.data_size = 0

    def add_chars(self, start, end, flag):
        for c in range(start, end):
            bucket = c // self.block_size
            in_block_idx = c % self.block_size

            while bucket >= len(self.stage1):
                self.stage1.append([0] * self.block_size)
            
            if not self.separate_values:
                self.stage1[bucket][in_block_idx] |= flag
            else:
                value_idx = self.stage1[bucket][in_block_idx]
                value = self.values[value_idx]
                value |= flag
                value_idx = self.value_idx_by_value.get(value)
                if value_idx is None:
                    value_idx = len(self.values)
                    self.values.append(value)
                    self.value_idx_by_value[value] = value_idx
                self.stage1[bucket][in_block_idx] = value_idx
                self.max_value = max(self.max_value, value)

            self.max_stage2_value = max(self.max_stage2_value, self.stage1[bucket][in_block_idx])

        self.max_known_char = end - 1

    def generate(self):
        for idx, block in enumerate(self.stage1):
            block = tuple(block)
            block_idx = self.blocks.get(block)
            if block_idx is None:
                block_idx = len(self.blocks_by_index)
                self.blocks[block] = block_idx
                self.blocks_by_index.append(block)
                self.max_block_idx = max(self.max_block_idx, block_idx)
            self.stage1[idx] = block_idx

        stage1_block_bits = len(self.stage1) * self.stage1_bits_per_value()
        stage1_block_bytes = (stage1_block_bits // 8) + bool(stage1_block_bits % 8)
        total_data_size = stage1_block_bytes
        
        stage2_block_bits = table_builder.block_size * self.stage2_bits_per_value()
        stage2_block_bytes = (stage2_block_bits // 8) + bool(stage2_block_bits % 8)
        total_data_size += stage2_block_bytes * len(self.blocks_by_index)

        if self.separate_values:
            values_block_bits = len(self.values) * self.values_bits_per_value()
            values_block_bytes = (values_block_bits // 8) + bool(values_block_bits % 8)
            total_data_size += values_block_bytes

        return total_data_size

    def make_stage1(self):
        ret = '{\n        '
        for idx, block_idx in enumerate(self.stage1):
            if idx > 0:
                ret += ', '
                if idx % 32 == 0:
                    ret += '\n        '
            ret += f'{block_idx}'
        ret += '}'
        return ret

    def make_stage2(self):
        ret = '{'
        for block_idx, block in enumerate(self.blocks_by_index):
            if block_idx > 0:
                ret += ','
            ret += '\n        '
            for idx, val in enumerate(block):
                if idx > 0:
                    ret += ', '
                #ret += f'0x{val:01X}'
                ret += f'{val}'
        ret += '\n    }'
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
    
    def stage1_bits_per_value(self):
        return self.max_block_idx.bit_length()
    
    def stage1_size(self):
        return len(self.stage1)
    
    def stage2_bits_per_value(self):
        return self.max_stage2_value.bit_length()
    
    def stage2_size(self):
        return len(self.blocks_by_index) * table_builder.block_size
    
    def values_bits_per_value(self):
        return self.max_value.bit_length() if self.separate_values else -1

    def values_size(self):
        return len(self.values)
