
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_PACK_HPP
#define BOOST_HASH_PACK_HPP

#include <boost/assert.hpp>
#include <boost/hash/bitstream_endian.hpp>
#include <boost/hash/detail/exploder.hpp>
#include <boost/hash/detail/imploder.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace hash {

template <bitstream_endian::type Endianness,
          int InputBits, int OutputBits,
          bool Explode = (InputBits > OutputBits),
          bool Implode = (InputBits < OutputBits)>
struct packer;

template <bitstream_endian::type Endianness,
          int Bits>
struct packer<Endianness, Bits, Bits, false, false> {

    template <typename OutputType, typename InputType>
    static OutputType pack_array(InputType const &in) {
        BOOST_STATIC_ASSERT(OutputType::static_size == InputType::static_size);
        OutputType out;
        unsigned i = 0;
        for (unsigned j = 0; j < InputType::static_size; ++j) {
            out[i++] = in[j];
        }
        return out;
    }

};

template <bitstream_endian::type Endianness,
          int InputBits, int OutputBits>
struct packer<Endianness,
              InputBits, OutputBits,
              true, false> {

    BOOST_STATIC_ASSERT(InputBits % OutputBits == 0);

    template <typename OutputType, typename InputType>
    static OutputType pack_array(InputType const &in) {
        BOOST_STATIC_ASSERT(OutputType::static_size*OutputBits ==
                            InputType::static_size*InputBits);
        OutputType out;
        unsigned i = 0;
        for (unsigned j = 0; j < InputType::static_size; ++j) {
            detail::exploder<Endianness, InputBits, OutputBits>
             ::explode1_array(out, i, in[j]);
        }
        BOOST_ASSERT(i == OutputType::static_size);
        return out;
    }

};

template <bitstream_endian::type Endianness,
          int InputBits, int OutputBits>
struct packer<Endianness,
              InputBits, OutputBits,
              false, true> {

    BOOST_STATIC_ASSERT(OutputBits % InputBits == 0);

    template <typename OutputType, typename InputType>
    static OutputType pack_array(InputType const &in) {
        BOOST_STATIC_ASSERT(OutputType::static_size*OutputBits ==
                            InputType::static_size*InputBits);
        OutputType out;
        unsigned i = 0;
        for (unsigned j = 0; j < OutputType::static_size; ++j) {
            detail::imploder<Endianness, InputBits, OutputBits>
             ::implode1_array(in, i, out[j] = 0);
        }
        BOOST_ASSERT(i == InputType::static_size);
        return out;
    }

};

// TODO: Add specializations to optimize the easy platform-endianness cases

template <bitstream_endian::type Endianness,
          int InBits, int OutBits,
          typename T1, typename T2>
void pack(T1 const &in, T2 &out) {
    typedef packer<Endianness, InBits, OutBits> packer_type;
    out = packer_type::template pack_array<T2>(in);
}

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_PACK_HPP
