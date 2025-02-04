# 
# Copyright 2024 Eugene Gershnik
# 
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file or at
# https://github.com/gershnik/sys_string/blob/master/LICENSE
# 

import hashlib

from textwrap import dedent
from common import bytes_for_bits, hex_format_for_bits, type_for_bits, indent_insert

class _trie_builder_config:
    def __init__(self, bits: int):
        if bits < 1 or bits > 7:
            raise RuntimeError('bits must be between 1 and 7 inclusive')
        self.bits_per_fanout = bits
        self.fanout = 1 << self.bits_per_fanout
        self.single_mask = (1 << self.bits_per_fanout) - 1
        self.char_mask_start = ((21 // self.bits_per_fanout) + bool(21 % self.bits_per_fanout)) * self.bits_per_fanout

    def print_base_class(self):
        ret = f'''
        template<class Derived>
        class trie_lookup<{self.bits_per_fanout}, Derived>
        {{
        public:
            SYS_STRING_FORCE_INLINE
            static auto get(char32_t c) noexcept
            {{
                size_t idx = Derived::values.size();
        '''
        for bit_start in range(self.char_mask_start, 0, -self.bits_per_fanout):
            ret += f'''
                {{
                    int char_idx = (c >> {bit_start - self.bits_per_fanout}) & 0x{self.single_mask:X};
                    auto & entry = Derived::entries[idx];
                    idx = entry[char_idx];
                }}
        '''

        ret += '''
               assert(idx < Derived::values.size());
               return typename Derived::value(Derived::values[idx]);
            }
        };
        '''

        return dedent(ret)

class trie_builder:

    __configs: dict[int, _trie_builder_config] = {}
    

    class node:
        def __init__(self, child_count):
            self.children = [None] * child_count
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
                ret = trie_builder.node(len(self.children))
                self.children[idx] = ret
            return ret
        
        
        def calc_size(self):
            ret = 1
            for child in self.children:
                if child is not None:
                    ret += child.calc_size()
            return ret

    
    def __init__(self, bits):
        self.config = trie_builder.__configs.setdefault(bits, _trie_builder_config(bits))
        self.root = trie_builder.node(self.config.fanout)
        self.byhash = {}
        self.values = {0:0}
        self.max_value = 0
        self.linear = [[0]*self.config.fanout]

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
            for bit_start in range(self.config.char_mask_start, 0, -self.config.bits_per_fanout):
                mask = self.config.single_mask << (bit_start - self.config.bits_per_fanout)
                child_idx = (c & mask) >> (bit_start - self.config.bits_per_fanout)
                node = node.descend(child_idx)
            node.value |= flag

    def get_char(self, c):
        node = self.root
        for bit_start in range(self.config.char_mask_start, 0, -self.config.bits_per_fanout):
            mask = self.config.single_mask << (bit_start - self.config.bits_per_fanout)
            child_idx = (c & mask) >> (bit_start - self.config.bits_per_fanout)
            if node.children[child_idx] is not None:
                node = node.children[child_idx]
            else:
                break
        return node.value

    def generate(self):
        #print(f"Original size: {self.root.calc_size()}")
        self.__trim(self.root)
        #print(f"After trim: {self.root.calc_size()}")
        self.root = self.__hash(self.root)
        #print(f"After hash: {len(self.byhash)}")
        
        self.__collect_values(self.root)
        self.__calc_linear()

        bytes_per_entry = bytes_for_bits(self.bits_per_index()) * self.config.fanout
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
            self.linear.append([terminal_idx] * self.config.fanout)

        for child in current.children:
            if child is not None:
                self.__collect_values(child)


    # def __calc_linear(self, current: node, indices_by_hash: dict):
    #     current_idx = indices_by_hash.get(current.hash)
    #     if current_idx is not None:
    #         return current_idx
        
    #     value_idx = self.values.get(current.value)

    #     if current.is_leaf():
    #         indices_by_hash[current.hash] = value_idx
    #         return value_idx
            
    #     current_idx = len(self.linear)
    #     indices_by_hash[current.hash] = current_idx
    #     self.linear.append([])
    #     current_linear: list = self.linear[current_idx]

    #     for child in current.children:
    #         if child is not None:
    #             current_linear.append(self.__calc_linear(child, indices_by_hash))
    #         else:
    #             current_linear.append(value_idx)
        
    #     return current_idx

    def __calc_linear(self):
        indices_by_hash = {}
        queue = [(self.root, None, -1)]

        while len(queue) != 0:
            current_node, parent_linear, child_idx = queue[0]
            del queue[0]
            
            current_idx = indices_by_hash.get(current_node.hash)
            if current_idx is not None:
                if parent_linear is not None:
                    parent_linear[child_idx] = current_idx
                continue

            value_idx = self.values.get(current_node.value)

            if current_node.is_leaf():
                if parent_linear is not None:
                    parent_linear[child_idx] = value_idx
                indices_by_hash[current_node.hash] = value_idx
                continue

            
            current_idx = len(self.linear)
            indices_by_hash[current_node.hash] = current_idx
            self.linear.append([value_idx] * self.config.fanout)
            current_linear: list = self.linear[current_idx]
            if parent_linear is not None:
                parent_linear[child_idx] = current_idx
            
            for child_idx, child in enumerate(current_node.children):
                if child is not None:
                    queue.append((child, current_linear, child_idx))



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
        for i in range(0, self.config.fanout):
            current.children[i] = None
        

    def __hash(self, current: node):
        m = hashlib.sha256()
        for idx in range(0, self.config.fanout):
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
    
    def make_entries(self):
        ret = ''
        line_len = 0
        for idx, entry in enumerate(self.linear):
            if idx > 0:
                ret += ', '
                line_len += 2
                if line_len >= 120:
                    ret += '\n'
                    line_len = 0
            indices = ', '.join([f'{val}' for val in entry])
            addition = f'{{{{{indices}}}}}'
            line_len += len(addition)
            ret += addition
        return ret
    
    def make_values(self):
        hf = hex_format_for_bits(self.bits_per_value())
        ret = ''
        for idx, value in enumerate(self.values):
            if idx > 0:
                ret += ', '
                if idx % 8 == 0:
                    ret += '\n'
            ret += ('0x{:'+ hf + '}').format(value)
            #f'0x{value:X}'
        return ret
    
    @staticmethod
    def print_common_header():
        if len(trie_builder.__configs) == 0:
            return ''
        
        ret = '''
        template<size_t N, class Derived>
        class trie_lookup;
        '''

        for config in trie_builder.__configs.values():
            ret += f'''
        {indent_insert(config.print_base_class(), 8)}
        '''
        
        return dedent(ret)

    def print_header(self, name, values_enum_content):
        ret = f'''
        class {name} : public trie_lookup<{self.config.bits_per_fanout}, {name}>
        {{
        friend trie_lookup<{self.config.bits_per_fanout},{name}>;
        private:
            using entry_type = std::array<{type_for_bits(self.bits_per_index())}, {self.config.fanout}>;
            using value_type = {type_for_bits(self.bits_per_value())};

            static const std::array<entry_type, {self.entris_count()}> entries;
        
            static const std::array<value_type, {self.values_count()}> values;

        public:
        '''
        if values_enum_content is not None:
            ret += f'''
            enum value : value_type
            {{
                none = 0,
                {indent_insert(values_enum_content, 16)}
            }};
        '''
        else:
            ret += '''
            using value = value_type;
        '''
        ret += '''
            static constexpr size_t data_size = sizeof(entries) + sizeof(values);
        };
        '''

        return dedent(ret)

    def print_impl(self, name):
        ret = f'''
        const std::array<{name}::entry_type, {self.entris_count()}> {name}::entries = {{{{
            {indent_insert(self.make_entries(), 12)}
        }}}};
    
        const std::array<{name}::value_type, {self.values_count()}> {name}::values = {{{{
            {indent_insert(self.make_values(), 12)}
        }}}};
        '''
            
        return dedent(ret)

