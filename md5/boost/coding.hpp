// Boost coding.hpp header file  ---------------------------------------------//
/** \file
    \brief  Group-inclusion of Boost.Coding components.

    \#Includes all of the headers of Boost.Coding and promotes the public names
    to the main Boost namespace.
 
    (C) Copyright Daryle Walker 2008.  Distributed under the Boost Software
    License, Version 1.0. (See the accompanying file LICENSE_1_0.txt or a copy
    at <http://www.boost.org/LICENSE_1_0.txt>.)
 */
// See <http://www.boost.org/libs/coding> for documentation.

#ifndef BOOST_CODING_HPP
#define BOOST_CODING_HPP

#include <boost/coding_fwd.hpp>

#include <boost/coding/md5.hpp>


namespace boost
{


//  Namespace promotions  ----------------------------------------------------//

// Ultimately from <boost/coding/md5.hpp>
using coding::md5_digest;
using coding::md5_context;
using coding::md5_byte_context;
using coding::md5_computer;
using coding::md5;


}  // namespace boost


#endif  // BOOST_CODING_HPP
