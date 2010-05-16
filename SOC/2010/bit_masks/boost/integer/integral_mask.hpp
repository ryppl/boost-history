//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 



#ifndef BOOST_INTEGRAL_MASK_HPP
#define BOOST_INTEGRAL_MASK_HPP
#include <boost/integer/high_low_bits.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/integer/bit_width.hpp>
#include <limits>


namespace boost {

/** Integral Mask.
 *  This integral Mask is defined similar to an integral constant.
 *  The following preconditions apply to the integral_mask type.
 *  Preconditions
 *
 *  1. T must be an integral type. NOTE: This is documented, but NOT enforeced.
 *  
 *  2. Value must be in the domain of the integral type T.
 */
template <typename T, T Value>
struct integral_mask
    : // details::integral_mask_preconditions<T,Value>,
    integral_constant<T, Value>
{
    // precondition 1.
    // BOOST_STATIC_ASSERT(( is_integral<T>::value ));
    
    // precondition 2.
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::max >= Value ));
    BOOST_STATIC_ASSERT((std::numeric_limits<T>::min <= Value ));

    typedef integral_mask<T,Value> type;

    T operator()() {
        return type::value;
    }
};

} // namespace boost

#endif
