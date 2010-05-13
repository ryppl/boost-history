//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INTEGRAL_BIT_WIDTH_HPP
#define BOOST_INTEGRAL_BIT_WIDTH_HPP
#include <boost/type_traits.hpp>
#include <boost/integer/details/high_low_impl.hpp>


namespace boost {

/** bit_width is a meta function which returns the size of bits in size 
 *  type T.
 */
template <typename T>
struct bit_width
    :integral_constant< unsigned int, (sizeof(T) * 8)>
{
    typedef bit_width<T> type;
};

} // namespace boost

#endif
