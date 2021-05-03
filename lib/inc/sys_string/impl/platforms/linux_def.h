//
// Copyright 2020 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//
#ifndef HEADER_SYS_STRING_H_INSIDE
#error This header must not be included directly. Please include sys_string.h
#endif


#include <sys_string/impl/util/generic_buffer.h>

namespace sysstr
{

    struct sys_string_traits
    {
        using storage_type = char;
        using size_type = size_t;
        using hash_type = unsigned;
        
        static constexpr size_type max_size = std::numeric_limits<size_t>::max() / sizeof(char);
    };
}

namespace sysstr::util
{

    template<size_t N> 
    using static_buffer     = generic::static_buffer<sys_string_traits::storage_type, sys_string_traits::size_type, N>;
    using dynamic_buffer    = generic::dynamic_buffer<sys_string_traits::storage_type, sys_string_traits::size_type>;

    using buffer            = generic::buffer<sys_string_traits::storage_type, sys_string_traits::size_type>;

    using builder_impl      = generic::buffer_builder<sys_string_traits::storage_type, sys_string_traits::size_type>;

    sys_string build(builder_impl &) noexcept;
    
    using char_access       = generic::char_access<sys_string_traits::storage_type, sys_string_traits::size_type>;
    
    using storage           = generic::storage<sys_string_traits::storage_type, sys_string_traits::size_type>;

}





