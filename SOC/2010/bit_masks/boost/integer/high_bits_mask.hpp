//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_HIGH_BITS_MASK_HPP
#define BOOST_HIGH_BITS_MASK_HPP

// #include <boost/type_traits.hpp>
#include <boost/integer/details/high_low_impl.hpp>
#include <boost/integer/integral_mask.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/make_unsigned.hpp>

namespace boost {


/** Creates a mask of the supplied width in side type T, from the lower portion 
 *  of the integer starting from the left most bit moving towards the right.
 */
template <typename T, unsigned int Width>
struct high_bits_mask
    :details::high_bits_preconditions<T,Width>,
    integral_mask<T, ~( ~ (typename make_unsigned<T>::type(0)) >> Width) >
{    
    typedef high_bits_mask<T,Width>  type;

    BOOST_STATIC_CONSTANT(unsigned int, offset = 0);

    BOOST_STATIC_CONSTANT(unsigned int, width  = Width);
};

} // namespace boost 

#endif
