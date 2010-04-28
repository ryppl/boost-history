
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_BITSTREAM_ENDIAN_HPP
#define BOOST_HASH_BITSTREAM_ENDIAN_HPP

namespace boost {
namespace hash {

namespace bitstream_endian {
    enum type {
        big_byte_big_bit,
        little_byte_big_bit,
        big_byte_little_bit,
        little_byte_little_bit,
    };
}

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_BITSTREAM_ENDIAN_HPP
