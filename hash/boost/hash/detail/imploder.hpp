
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_DETAIL_IMPLODER_HPP
#define BOOST_HASH_DETAIL_IMPLODER_HPP

#include <boost/hash/bitstream_endian.hpp>
#include <boost/hash/detail/unbounded_shift.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace hash {
namespace detail {

template <bitstream_endian::type Endianness,
          int InputBits, int OutputBits,
          int k = 0>
struct imploder;

template <int InputBits, int OutputBits,
          int k>
struct imploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, k> {
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<OutputBits>(OutputValue(z));
        x |= unbounded_shl<OutputBits - (InputBits+k)>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int InputBits, int OutputBits>
struct imploder<bitstream_endian::big_byte_big_bit,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int InputBits, int OutputBits,
          int k>
struct imploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, k> {
    // Mixed-endian pack explode can only handle bitwidths that are
    // multiples or fractions of bytes
    BOOST_STATIC_ASSERT((InputBits % 8 == 0 || 8 % InputBits == 0) &&
                        (OutputBits % 8 == 0 || 8 % OutputBits == 0));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<OutputBits>(OutputValue(z));
        int const kb = (k % 8);
        int const kB = k - kb;
        int const shift =
            InputBits  >= 8 ? k :
            OutputBits >= 8 ? kB + (8-(InputBits+kb)) :
                              OutputBits - (InputBits+kb);
        x |= unbounded_shl<shift>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int InputBits, int OutputBits>
struct imploder<bitstream_endian::little_byte_big_bit,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int InputBits, int OutputBits,
          int k>
struct imploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, k> {
    // Mixed-endian pack explode can only handle bitwidths that are
    // multiples or fractions of bytes
    BOOST_STATIC_ASSERT((InputBits % 8 == 0 || 8 % InputBits == 0) &&
                        (OutputBits % 8 == 0 || 8 % OutputBits == 0));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<OutputBits>(OutputValue(z));
        int const kb = (k % 8);
        int const kB = k - kb;
        int const shift =
            InputBits  >= 8 ? OutputBits - (InputBits+k) :
            OutputBits >= 8 ? OutputBits - (8+kB) + kb :
                              kb;
        x |= unbounded_shl<shift>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int InputBits, int OutputBits>
struct imploder<bitstream_endian::big_byte_little_bit,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int InputBits, int OutputBits,
          int k>
struct imploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, k> {
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<OutputBits>(OutputValue(z));
        x |= unbounded_shl<k>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int InputBits, int OutputBits>
struct imploder<bitstream_endian::little_byte_little_bit,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

} // namespace detail
} // namespace hash
} // namespace boost

#endif // BOOST_HASH_DETAIL_IMPLODER_HPP
