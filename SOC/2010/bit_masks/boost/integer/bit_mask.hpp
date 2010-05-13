//  Boost integer/bit_mask.hpp header file

//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTEGER_BIT_MASK_HPP
#define BOOST_INTEGER_BIT_MASK_HPP

// boost dependencies.


#include <boost/integer/high_low_bits.hpp>
#include <boost/integer/details/bit_mask_impl.hpp>

namespace boost {


/** bit_mask.
 *  Mask which creates a mask give type, offset and width of the mask
 */
template <typename T, unsigned int Offset, unsigned int Width = 1 >
struct bit_mask
    :details::bit_mask_preconditions<T, Offset, Width>,
    integral_constant<T, (low_bits<T,Width>::value << Offset) >
{
    typedef bit_mask<T, Offset, Width> type;
};

/** Integral Mask.
 *  This integral Mask is defined similar to an integral constant.
 */
template <typename T, T Value>
struct integral_mask
{

};

} // namespace boost

#endif
