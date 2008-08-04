// Boost coding_fwd.hpp header file  -----------------------------------------//
/** \file
    \brief  Forward declarations of Boost.Coding components.

    \author  Daryle Walker

    Contains the forward declarations of Boost.Coding's public structures,
    classes, and templates thereof, and any type aliases.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_FWD_HPP
#define BOOST_CODING_FWD_HPP

#include <cstddef>  // for std::size_t


namespace boost
{
/** \brief Name-space for Boost.Coding

    Primary name-space for the public components of a library providing methods
    of summarizing blocks of data, i.e. messages, as (relatively) compact values
    to use for cryptography and data integrity.
 */
namespace coding
{


//  From <boost/coding/coding_shell.hpp>  ------------------------------------//

template < class ByteProcessor >
class byte_coding_shell;

template < class BitProcessor, bool BigEndian >
class bit_to_byte_processor;

template < class BitProcessor, bool BigEndian >
class bit_coding_shell;


//  From <boost/coding/operations.hpp>  --------------------------------------//

template < class Derived, typename SizeType = std::size_t >
class byte_processing_base;

template < class Derived, typename SizeType = std::size_t >
class bit_processing_b_base;

template < class Derived, typename LengthType, LengthType QueueLength, typename
 SizeType = std::size_t >
class queued_bit_processing_base;


//  From <boost/coding/md5_digest_core.hpp>  ---------------------------------//

struct md5_digest;

// Also has operator free-functions


//  From <boost/coding/md5_digest.hpp>  --------------------------------------//

// Only has operator free-functions, #includes "md5_digest_core.hpp"


//  From <boost/coding/md5.hpp>  ---------------------------------------------//

class md5_computerX;

class md5_context;
class md5_computer;

// Also has a free function


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_FWD_HPP
