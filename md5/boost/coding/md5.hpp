// Boost coding/md5.hpp header file  -----------------------------------------//
/** \file
    \brief  Declarations of MD5 computation components.

    \author  Daryle Walker

    Contains the declaration of types and functions used for computing MD5
    message digests of given data blocks and granting I/O capability to any
    applicable types.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_MD5_HPP
#define BOOST_CODING_MD5_HPP

#include <boost/coding_fwd.hpp>
#include <boost/coding/md5_digest.hpp>    // for boost::coding::md5_digest
#include <boost/coding/md5_context.hpp>   // for b:c:md5_(byte_)context
#include <boost/coding/md5_computer.hpp>  // for boost::coding::md5_computer

#include <cstddef>  // for std::size_t


namespace boost
{
namespace coding
{


//  MD5 message-digest computation function definition  ----------------------//

/** \brief  Immediate MD5 message-digest computation

    Determines the MD5 message-digest of a given block of data, without
    requiring the setup of a computation object.

    \pre  \p buffer must point to a valid region of memory that contains at
          least \p byte_count bytes past the given pointer.

    \param buffer      Points to the beginning of the data block to be
                       processed.
    \param byte_count  The length of the data block to be processed, in bytes.

    \return  The MD5 message digest of the data block.

    \see  boost::coding::md5_digest
    \see  boost::coding::md5_byte_context
 */
inline md5_digest  md5( void const *buffer, std::size_t byte_count )
{
    md5_byte_context       c;
    unsigned char const *  b = static_cast<unsigned char const *>( buffer );

    while ( byte_count-- )
        c( *b++ );
    return c();
}


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_MD5_HPP
