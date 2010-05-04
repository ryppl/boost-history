
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_DETAIL_IMPLODER_HPP
#define BOOST_HASH_DETAIL_IMPLODER_HPP

#include <boost/hash/stream_endian.hpp>
#include <boost/hash/detail/unbounded_shift.hpp>
#include <boost/static_assert.hpp>

#include <climits>
#include <cstring>

namespace boost {
namespace hash {
namespace detail {

// By definition, for all imploders, InputBits < OutputBits,
// so we're taking many smaller values and combining them into one value

template <typename Endianness,
          int InputBits, int OutputBits,
          int k = 0>
struct imploder;

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct imploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k> {
    // If this fires, you probably want stream_endian::big_bit instead
    // of big_byte_big_bit or big_octet_big_bit
    BOOST_STATIC_ASSERT(!(InputBits  % UnitBits && UnitBits % InputBits ) &&
                        !(OutputBits % UnitBits && UnitBits % OutputBits));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<InputBits>(OutputValue(z));
        x |= unbounded_shl<OutputBits - (InputBits+k)>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct imploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct imploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<InputBits>(OutputValue(z));
        int const kb = (k % UnitBits);
        int const ku = k - kb;
        int const shift =
            InputBits  >= UnitBits ? k :
            OutputBits >= UnitBits ? ku + (UnitBits-(InputBits+kb)) :
                                     OutputBits - (InputBits+kb);
        x |= unbounded_shl<shift>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct imploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct imploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<InputBits>(OutputValue(z));
        int const kb = (k % UnitBits);
        int const ku = k - kb;
        int const shift =
            InputBits  >= UnitBits ? OutputBits - (InputBits+k) :
            OutputBits >= UnitBits ? OutputBits - (UnitBits+ku) + kb :
                                     kb;
        x |= unbounded_shl<shift>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct imploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct imploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k> {
    // If this fires, you probably want stream_endian::little_bit instead
    // of little_byte_little_bit or little_octet_little_bit
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename InputValue, typename OutputValue>
    static void step(InputValue z, OutputValue &x) {
        OutputValue y = low_bits<InputBits>(OutputValue(z));
        x |= unbounded_shl<k>(y);
    }
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &in, unsigned &i, OutputValue &x) {
        step(in[i++], x);
        imploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k+InputBits>
         ::implode1_array(in, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct imploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, OutputBits> {
    template <typename InputType, typename OutputValue>
    static void implode1_array(InputType const &, unsigned &, OutputValue &) {}
};

} // namespace detail
} // namespace hash
} // namespace boost

#endif // BOOST_HASH_DETAIL_IMPLODER_HPP
