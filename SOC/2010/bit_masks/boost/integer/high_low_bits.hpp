//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_HIGH_LOW_BIT_HPP
#define BOOST_HIGH_LOW_BIT_HPP

#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits.hpp>

namespace boost {

template <typename T, T Width>
struct low_bits
    :integral_constant<T, ~(~T(0) << Width) >
{
    typedef low_bits<T,Width>  type;
};

} // namespace boost 

#endif
