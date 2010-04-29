
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_DETAIL_EXPLODER_HPP
#define BOOST_HASH_DETAIL_EXPLODER_HPP

#include <boost/hash/bitstream_endian.hpp>
#include <boost/hash/detail/unbounded_shift.hpp>
#include <boost/static_assert.hpp>

#include <climits>
#include <cstring>

namespace boost {
namespace hash {
namespace detail {

template <typename Endianness,
          int InputBits, int OutputBits,
          int k = 0>
struct exploder;

template <int InputBits, int OutputBits,
          int k>
struct exploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, k> {
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        InputValue y = unbounded_shr<InputBits - (OutputBits+k)>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int InputBits, int OutputBits>
struct exploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int InputBits, int OutputBits,
          int k>
struct exploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, k> {
    // Mixed-endian pack explode can only handle bitwidths that are
    // multiples or fractions of bytes
    BOOST_STATIC_ASSERT((InputBits % 8 == 0 || 8 % InputBits == 0) &&
                        (OutputBits % 8 == 0 || 8 % OutputBits == 0));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        int const kb = (k % 8);
        int const kB = k - kb;
        int const shift =
            OutputBits >= 8 ? k :
            InputBits  >= 8 ? kB + (8-(OutputBits+kb)) :
                              InputBits - (OutputBits+kb);
        InputValue y = unbounded_shr<shift>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int InputBits, int OutputBits>
struct exploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int InputBits, int OutputBits,
          int k>
struct exploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, k> {
    // Mixed-endian pack explode can only handle bitwidths that are
    // multiples or fractions of bytes
    BOOST_STATIC_ASSERT((InputBits % 8 == 0 || 8 % InputBits == 0) &&
                        (OutputBits % 8 == 0 || 8 % OutputBits == 0));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        int const kb = (k % 8);
        int const kB = k - kb;
        int const shift =
            OutputBits >= 8 ? InputBits - (OutputBits+k) :
            InputBits  >= 8 ? InputBits - (8+kB) + kb :
                              kb;
        InputValue y = unbounded_shr<shift>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int InputBits, int OutputBits>
struct exploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int InputBits, int OutputBits,
          int k>
struct exploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, k> {
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        InputValue y = unbounded_shr<k>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int InputBits, int OutputBits>
struct exploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int InputBits, int OutputBits,
          int k>
struct exploder<bitstream_endian::platform,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(InputBits  % CHAR_BIT == 0);
    BOOST_STATIC_ASSERT(OutputBits % CHAR_BIT == 0);
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        std::memcpy(&z, (char*)&x + k/CHAR_BIT, OutputBits/CHAR_BIT);
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<bitstream_endian::platform,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int InputBits, int OutputBits>
struct exploder<bitstream_endian::platform,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

} // namespace detail
} // namespace hash
} // namespace boost

#endif // BOOST_HASH_DETAIL_EXPLODER_HPP
