//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_HIGH_LOW_BIT_HPP
#define BOOST_HIGH_LOW_BIT_HPP

#include <boost/type_traits.hpp>
#include <boost/integer/details/high_low_impl.hpp>

namespace boost {


/** Creates a mask of the supplied width in side type T, from the lower portion 
 *  of the integer starting from the right most bit moving towards the left.
 */
template <typename T, unsigned int Width>
struct low_bits
    :details::low_bits_preconditions<T,Width>,
    integral_constant<T, ~(~T(0) << Width) >
{    
    typedef low_bits<T,Width>  type;
};

/** Creates a mask of the supplied width in side type T, from the lower portion 
 *  of the integer starting from the right most bit moving towards the left.
 */
template <typename T, unsigned int Width>
struct high_bits
    :details::high_bits_preconditions<T,Width>,
    integral_constant<T, ~(~T(0) >> Width) >
{    
    typedef high_bits<T,Width>  type;
};



} // namespace boost 

#endif
