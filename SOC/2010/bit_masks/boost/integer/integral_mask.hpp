//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTEGRAL_MASK_HPP
#define BOOST_INTEGRAL_MASK_HPP

namespace boost {


/** Integral Mask.
 *  This integral Mask is defined similar to an integral constant.
 */
template <typename T, T Value>
struct integral_mask
    :details::integral_mask_preconditions<T,Value>,
    integral_constant<T, Value>
{
    typedef integral_mask<T,Value> type;

    T operator()() {
        return type::value;
    }
};

} // namespace boost

#endif
