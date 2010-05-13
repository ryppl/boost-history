//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_MASK_IMPL_HPP
#define BOOST_BIT_MASK_IMPL_HPP

#include <boost/type_traits.hpp>
#include <boost/mpl/bitwise.hpp>
#include <boost/static_assert.hpp>
#include <boost/integer/bit_width.hpp>

namespace boost { namespace details {



/** Preconditions for bit_mask impl
 *  Pre-Conditions
 *
 *  1. The width of the masked section must not be longer then the
 *  mask itself.
 *
 *  2. Valid range for offset is >= 0.
 *
 *  3. Valid range for mask width is > 0.
 *  
 *  4. The type which is being masked must satisfy the is_integral type trait.
 */
template <typename T, unsigned int Offset, unsigned int Width>
struct bit_mask_preconditions {

    // precondition 1.
    BOOST_STATIC_ASSERT(( (Offset + Width) < ( bit_width<T>::value - 1)  ));

    // precondition 2.
    // this is always true because the offset is specified as an unsigned integer.
    // BOOST_STATIC_ASSERT((  >= 0 ));

    // precondition 3.
    BOOST_STATIC_ASSERT(( Width > 0 ));
    
    // precondition 4.
    BOOST_STATIC_ASSERT(( is_integral<T>::value ));
};

} // end of details namespace.
} // end of boost namespace.

#endif
