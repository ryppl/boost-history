//  Boost integer/bit_mask.hpp header file

//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTEGER_BIT_MASK_HPP
#define BOOST_INTEGER_BIT_MASK_HPP

// boost dependencies.



#include <boost/integer/details/bit_mask_impl.hpp>

namespace boost {


/** bit_mask.
 *  Mask which creates a mask give type, offset and width of the mask
 */
template <typename T, typename Offset, typename Width>
struct bit_mask
{ };

template <typename T, T Value>
struct integral_mask
{ };

} // namespace boost

#endif
