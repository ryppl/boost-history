//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 
#ifndef BOOST_HIGH_LOW_IMPL_HPP
#define BOOST_HIGH_LOW_IMPL_HPP

#include <boost/static_assert.hpp>
#include <boost/integer/bit_width.hpp>

namespace boost { namespace details {

/** Preconditions for low_bits metafunction.
 *  Precondition: 
 *  1. The width must not be greater then the size in bits of type T.
 */
template <typename T, unsigned int Width>
struct low_bits_preconditions {
    BOOST_STATIC_ASSERT(( bit_width<T>::value >= Width ));
};

/** Preconditions for high_bits metafunction.
 *  Precondition: 
 *  1. The width must not be greater then the size in bits of type T.
 */
template <typename T, unsigned int Width>
struct high_bits_preconditions {
    BOOST_STATIC_ASSERT(( bit_width<T>::value >= Width ));
};


} } // namespace boost::details

#endif
