
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_DETAIL_EXPLODER_HPP
#define BOOST_HASH_DETAIL_EXPLODER_HPP

#include <boost/hash/stream_endian.hpp>
#include <boost/hash/detail/unbounded_shift.hpp>
#include <boost/static_assert.hpp>

#include <climits>
#include <cstring>

namespace boost {
namespace hash {
namespace detail {

// By definition, for all exploders, InputBits > OutputBits,
// so we're taking one value and splitting it into many smaller values

template <typename Endianness,
          int InputBits, int OutputBits,
          int k = 0>
struct exploder;

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct exploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k> {
    // If this fires, you probably want stream_endian::big_bit instead
    // of big_byte_big_bit or big_octet_big_bit
    BOOST_STATIC_ASSERT(!(InputBits  % UnitBits && UnitBits % InputBits ) &&
                        !(OutputBits % UnitBits && UnitBits % OutputBits));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        InputValue y = unbounded_shr<InputBits - (OutputBits+k)>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct exploder<stream_endian::big_unit_big_bit<UnitBits>,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct exploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        int const kb = (k % UnitBits);
        int const ku = k - kb;
        int const shift =
            OutputBits >= UnitBits ? k :
            InputBits  >= UnitBits ? ku + (UnitBits-(OutputBits+kb)) :
                                     InputBits - (OutputBits+kb);
        InputValue y = unbounded_shr<shift>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct exploder<stream_endian::little_unit_big_bit<UnitBits>,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct exploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        int const kb = (k % UnitBits);
        int const ku = k - kb;
        int const shift =
            OutputBits >= UnitBits ? InputBits - (OutputBits+k) :
            InputBits  >= UnitBits ? InputBits - (UnitBits+ku) + kb :
                                     kb;
        InputValue y = unbounded_shr<shift>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct exploder<stream_endian::big_unit_little_bit<UnitBits>,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct exploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k> {
    // If this fires, you probably want stream_endian::little_bit instead
    // of little_byte_little_bit or little_octet_little_bit
    BOOST_STATIC_ASSERT(!((InputBits  % UnitBits) && (UnitBits % InputBits )) &&
                        !((OutputBits % UnitBits) && (UnitBits % OutputBits)));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        InputValue y = unbounded_shr<k>(x);
        z = OutputValue(low_bits<OutputBits>(y));
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct exploder<stream_endian::little_unit_little_bit<UnitBits>,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

template <int UnitBits, int InputBits, int OutputBits,
          int k>
struct exploder<stream_endian::host_unit<UnitBits>,
                InputBits, OutputBits, k> {
    BOOST_STATIC_ASSERT(!(InputBits  % UnitBits) &&
                        !(OutputBits % UnitBits));
    template <typename OutputValue, typename InputValue>
    static void step(OutputValue &z, InputValue x) {
        std::memcpy(&z, (char*)&x + k/CHAR_BIT, OutputBits/CHAR_BIT);
    }
    template <typename OutputType, typename InputValue>
    static void explode1_array(OutputType &out, unsigned &i, InputValue x) {
        step(out[i++], x);
        exploder<stream_endian::host_unit<UnitBits>,
                InputBits, OutputBits, k+OutputBits>
         ::explode1_array(out, i, x);
    }
};
template <int UnitBits, int InputBits, int OutputBits>
struct exploder<stream_endian::host_unit<UnitBits>,
                InputBits, OutputBits, InputBits> {
    template <typename OutputType, typename IntputValue>
    static void explode1_array(OutputType &, unsigned &, IntputValue) {}
};

} // namespace detail
} // namespace hash
} // namespace boost

#endif // BOOST_HASH_DETAIL_EXPLODER_HPP
