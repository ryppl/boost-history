//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>

namespace boost { namespace bitfields {
/** Different fields types which can be used to create/denote the layout of the
 *  packet.
 */
template <typename Type, std::size_t SizeInBytes, typename Endianness>
struct field;

/** Field of bits. */
template <typename Type, std::size_t SizeInBits>
struct bitfield;

/** Creates an in accessable region of bytes. Size given in bytes. */
template <std::size_t SizeInBytes>
struct reserved_field;

/** Creates region in bits which prevents prevents access to that region the
 * size is given in bits.
 */
template <std::size_t SizeInBits>
struct reserved_bitfield;

/** Creates a field that acts as a bit set. */
template <std::size_t SizeInBits, typename Endianness>
struct bitset_field;

/** Need to think about this more and make sure that I can actually enforce
 *  endianness of an array.
 */
template <typename BufferType, std::size_t Length, typename Endianness>
struct buffer;

/** Used to denote information being moved by a packet this gives direct access 
 *  to the underlying data. The size of the payload is given at compile time.
 */
template <typename T, std::size_t Bytes>
struct static_payload;

/** Paload which size is given at runtime. */
template <typename T>
struct dynamic_payload;

/** */
template <std::size_t Size>
struct string_buffer;

}} // end boost::bitfields
