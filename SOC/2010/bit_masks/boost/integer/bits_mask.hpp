//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BITS_MASK_HPP
#define BOOST_BITS_MASK_HPP
#include <boost/integer/high_bits_mask.hpp>
#include <boost/integer/low_bits_mask.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/integer/bit_width.hpp>


namespace boost {


/** bit_mask.
 *  Mask which creates a mask give type, offset and width of the mask.
 *  Preconditions for bit_mask
 *  Pre-Conditions
 *
 *  1. The width of the masked section must not be longer then the
 *  mask itself.
 *
 *  2. Valid range for mask width is > 0.
 *  
 *  3. The type which is being masked must satisfy the is_integral type trait.
 *  NOTE: This is documented but not enforeced.
 *
 */
template <typename T, unsigned int Offset, unsigned int Width = 1 >
struct bits_mask
    :integral_mask<T, (low_bits_mask<T,Width>::value << Offset) >
{
    // precondition 1.
    BOOST_STATIC_ASSERT(( (Offset + Width) <= ( bit_width<T>::value )  ));

    // precondition 2.
    BOOST_STATIC_ASSERT(( Width > 0 ));

    typedef bits_mask<T, Offset, Width> type;

    BOOST_STATIC_CONSTANT(unsigned int, offset = Offset);

    BOOST_STATIC_CONSTANT(unsigned int, width  = Width);

};

} // namespace boost


#endif

