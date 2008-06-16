// Boost md5.cpp implementation file  ----------------------------------------//

// (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for documentation.

/** \file
    \brief  Definitions of MD5 computation components

    Contains the definitions of constants and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.
 */

#include <boost/coding/md5.hpp>

#include <cstddef>  // for std::size_t


namespace boost
{
namespace coding
{


//  MD5 message-digest class-static member definitions  ----------------------//

int const          md5_digest::bits_per_word;
std::size_t const  md5_digest::words_per_digest;


//  Implementation detail object definitions  --------------------------------//

namespace detail
{

// Nybbles and hexadecimal digits
std::size_t const  md5_constants::nybbles_per_hexadecimal_digit;
std::size_t const  md5_constants::bits_per_nybble;

std::size_t const  md5_constants::number_of_hexadecimal_digits;

char const  md5_constants::hex_digits_lc[] = "0123456789abcdef";
char const  md5_constants::hex_digits_uc[] = "0123456789ABCDEF";

// MD words
std::size_t const  md5_constants::nybbles_per_word;

// MD strings
std::size_t const  md5_constants::characters_per_digest;

}  // namespace detail


//  MD5 message-digest computer class-static member definitions  -------------//

int const  md5_computer::significant_bits_per_length;

std::size_t const  md5_computer::words_per_block;
std::size_t const  md5_computer::bits_per_block;

// Initial values of the MD buffer, taken from RFC 1321, section 3.3.  (Note
// that the RFC lists each number low-order byte first, while numbers need to be
// written high-order byte first in C++.)
md5_computer::ibuffer_type const  md5_computer::initial_buffer_ = {
 {0x67452301ul, 0xEFCDAB89ul, 0x98BACDFEul, 0x10325476ul} };


}  // namespace coding
}  // namespace boost
