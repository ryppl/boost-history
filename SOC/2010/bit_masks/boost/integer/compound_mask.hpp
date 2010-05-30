//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_COMPOIND_BIT_MASK_HPP
#define BOOST_COMPOIND_BIT_MASK_HPP

#include <boost/integer/bit_mask.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bitor.hpp>

namespace boost {



/** \brief This is a compound bitmask composed of 1 or up to 10 other bitmasks
 *  via bitwise or.
 *  There are a lot of issues which come with this particular meta function
 *  to start, what types should be allowed to compose the compound mask? 
 *      For now the answer to that question is only mask which all have the same
 *      value type. In the future it may be OK to allow the user to use 
 *      differing value_types but for now this is going to be restring the
 *      useage of this meta-function to types which all share the same type.
 */
template <  typename Mask0,
            typename Mask1  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask2  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask3  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask4  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask5  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask6  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask7  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask8  = integral_mask< typename Mask0::value_type, 0 >,
            typename Mask9  = integral_mask< typename Mask0::value_type, 0 >
        >
struct compound_mask
    :integral_mask<
        typename Mask0::value_type,
            Mask0::value |
            Mask1::value |
            Mask2::value |
            Mask3::value |
            Mask4::value |
            Mask5::value |
            Mask6::value |
            Mask7::value |
            Mask8::value |
            Mask9::value
    >
{
    /** Re-exposing the composing masks.
     *  These aren't not meant to be iterated over as this is not a sequence
     *  which is the justification for not providing a tuple like interface
     *  for the this meta-function.
     */ 
    typedef Mask0 mask0;
    typedef Mask1 mask1;
    typedef Mask2 mask2;
    typedef Mask3 mask3;
    typedef Mask4 mask4;
    typedef Mask5 mask5;
    typedef Mask6 mask6;
    typedef Mask7 mask7;
    typedef Mask8 mask8;
    typedef Mask9 mask9;
};


} // namespace boost

#endif
