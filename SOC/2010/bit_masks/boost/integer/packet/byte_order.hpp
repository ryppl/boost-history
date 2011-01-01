//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTEGER_PACKET_BYTE_ORDER_HPP
#define BOOST_INTEGER_PACKET_BYTE_ORDER_HPP
#include <cstddef>

namespace boost { namespace bitfields {

/** Represents the order of bytes for a perticular endianness. */
/** The goal is to supply as many ways of representing byte order as reasonable
 *  such that when used it make the interface look the way the user would like.
 */

template <std::size_t ByteOrder> struct byte_order;

template <
    std::size_t ByteOne,
    std::size_t ByteTwo
>
struct byte_order_2bytes;

template <
    std::size_t ByteOne,
    std::size_t ByteTwo,
    std::size_t ByteThree
>
struct byte_order_3bytes;

template <
    std::size_t ByteOne,
    std::size_t ByteTwo,
    std::size_t ByteThree,
    std::size_t ByteFour
>
struct byte_order_4bytes;


}} // end boost::bitfields

#endif
