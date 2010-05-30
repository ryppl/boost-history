//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 



#ifndef BOOST_INTEGRAL_MASK_HPP
#define BOOST_INTEGRAL_MASK_HPP
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
 *  1. T must be an integral type. 
 *  NOTE: This is documented, but NOT enforeced.
 *
 *  2. Value must be in the domain of the integral type T.
 *  NOTE: Documented Requirement but Not enforeced.
 */
template <typename T, T Value>
struct integral_mask
    : integral_constant<T, Value>
{
    typedef integral_mask<T,Value> type;

    operator T() const{
        return type::value;
    }

};

} // namespace boost


#endif
