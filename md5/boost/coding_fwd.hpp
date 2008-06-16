// Boost coding_fwd.hpp header file  -----------------------------------------//

// (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for documentation.

/** \file
    \brief  Forward declarations of Boost.Coding components

    Contains the forward declarations of Boost.Coding's public structures,
    classes, and templates thereof, and any type aliases.
 */

#ifndef BOOST_CODING_FWD_HPP
#define BOOST_CODING_FWD_HPP


namespace boost
{
/** \brief Name-space for Boost.Coding

    Primary name-space for the public components of a library providing methods
    of summarizing blocks of data, i.e. messages, as (relatively) compact values
    to use for cryptography and data integrity.
 */
namespace coding
{


//  From <boost/coding/md5.hpp>  ---------------------------------------------//

class md5_digest;

class md5_computer;

// Also has a free function


}  // namespace coding
}  // namespace boost


#endif  // BOOST_CODING_FWD_HPP
