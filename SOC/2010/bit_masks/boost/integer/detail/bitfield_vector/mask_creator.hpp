//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP
#define BOOST_BITFIELD_VECTOR_MASK_CREATOR_HPP

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/set.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/greater.hpp>
#include <cstddef>

namespace boost { namespace detail {



/*
template <std::size_t Width>
inline std::size_t get_mask_array_size(std::size_t offset) {
    BOOST_ASSERT(( offset < 8));
    std::size_t total_bits = Width + offset;
    std::size_t ret = 0;
    ret = total_bits / 8;
    if(total_bits % 8) {
        ret += 1;
    }
    return ret;
}
*/

/** This calculates the size of the array needed to hold the entire mask
 *  based on an Offset and a Width.
 */
template <typename Offset, typename Width>
struct mask_size {
    BOOST_STATIC_ASSERT( Offset::value < 8 );
    
    typedef typename mpl::plus<Offset,Width>::type total_bits;
    typedef typename mpl::divides<total_bits,mpl::size_t<8> >::type ret;
    typedef typename mpl::if_<
        mpl::greater<
            typename mpl::modulus< total_bits, mpl::size_t<8> >::type,
            mpl::size_t<0>
        >,
        typename mpl::plus<ret, mpl::size_t<1> >::type,
        ret
    >::type type;
};

/*
template <std::size_t Offset, std::size_t Width>
struct mask_info {
    BOOST_STATIC_CONSTANT(std::size_t, offset   = Offset);
    BOOST_STATIC_CONSTANT(std::size_t, width    = Width );
    
};
*/
/** This is the main interface class which will be responsible for
 *  returning a reference to static mask data member which can be used
 *  by the reference type within bitfield_vector.
 */
// template <std::size_t Width>


}} // end booss::detail
#endif
