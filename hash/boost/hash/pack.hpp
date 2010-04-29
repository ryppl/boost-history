
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_PACK_HPP
#define BOOST_HASH_PACK_HPP

#include <boost/assert.hpp>
#include <boost/hash/stream_endian.hpp>
#include <boost/hash/detail/exploder.hpp>
#include <boost/hash/detail/imploder.hpp>
#include <boost/static_assert.hpp>

#ifndef BOOST_HASH_NO_OPTIMIZATION
#include <boost/detail/endian.hpp>
#endif

namespace boost {
namespace hash {

template <typename Endianness,
          int InputBits, int OutputBits,
          bool Explode = (InputBits > OutputBits),
          bool Implode = (InputBits < OutputBits),
          bool BytesOnly = (InputBits % CHAR_BIT == 0 &&
                            OutputBits % CHAR_BIT == 0),
          bool Prefer = true>
struct packer;

template <typename Endianness,
          int Bits,
          bool BytesOnly>
struct packer<Endianness, Bits, Bits, false, false, BytesOnly, false> {

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

template <typename Endianness,
          int InputBits, int OutputBits,
          bool BytesOnly>
struct packer<Endianness,
              InputBits, OutputBits,
              true, false, BytesOnly, false> {

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

template <typename Endianness,
          int InputBits, int OutputBits,
          bool BytesOnly>
struct packer<Endianness,
              InputBits, OutputBits,
              false, true, BytesOnly, false> {

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

// Forward Prefer to !Prefer if nothing better matches
template <typename Endianness,
          int InputBits, int OutputBits,
          bool Explode, bool Implode, bool BytesOnly>
struct packer<Endianness,
              InputBits, OutputBits,
              Explode, Implode, BytesOnly, true>
 : packer<Endianness,
              InputBits, OutputBits,
              Explode, Implode, BytesOnly, false> {};

#ifndef BOOST_HASH_NO_OPTIMIZATION

#ifdef BOOST_LITTLE_ENDIAN
template <int InputBits, int OutputBits, bool Ex, bool Im>
struct packer<stream_endian::little_byte_big_bit,
              InputBits, OutputBits,
              Ex, Im, true, true>
 : packer<stream_endian::host_byte,
          InputBits, OutputBits,
          Ex, Im, true, true> {};
template <int InputBits, int OutputBits, bool Ex, bool Im>
struct packer<stream_endian::little_byte_little_bit,
              InputBits, OutputBits,
              Ex, Im, true, true>
 : packer<stream_endian::host_byte,
          InputBits, OutputBits,
          Ex, Im, true, true> {};
#endif

#ifdef BOOST_BIG_ENDIAN
template <int InputBits, int OutputBits, bool Ex, bool Im>
struct packer<stream_endian::big_byte_big_bit,
              InputBits, OutputBits,
              Ex, Im, true, true>
 : packer<stream_endian::host_byte,
          InputBits, OutputBits,
          Ex, Im, true, true> {};
template <int InputBits, int OutputBits, bool Ex, bool Im>
struct packer<stream_endian::big_byte_little_bit,
              InputBits, OutputBits,
              Ex, Im, true, true>
 : packer<stream_endian::host_byte,
          InputBits, OutputBits,
          Ex, Im, true, true> {};
#endif

#endif

template <typename Endianness,
          int InBits, int OutBits,
          typename T1, typename T2>
void pack(T1 const &in, T2 &out) {
    typedef packer<Endianness, InBits, OutBits> packer_type;
    out = packer_type::template pack_array<T2>(in);
}

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_PACK_HPP
