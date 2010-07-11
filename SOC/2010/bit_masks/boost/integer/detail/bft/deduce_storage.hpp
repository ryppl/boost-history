//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_DEDUCE_STORAGE_HPP
#define BOOST_BITFIELD_TUPLE_DEDUCE_STORAGE_HPP

#include <boost/cstdint.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/static_assert.hpp>
#include <boost/config.hpp>

namespace boost { namespace details {

/** Meta functions used for deducing the type of storage that should be used if
 *  the user doesn't supply a storage type to a bitfield_tuple.
 */
template <typename BitsUsed>
struct deduce_storage_type {
    #ifndef BOOST_NO_INT64_T
    BOOST_STATIC_ASSERT(( BitsUsed::value <= 64 ));
    #else
    BOOST_STATIC_ASSERT(( BitsUsed::value <= 32 ));
    #endif
    typedef typename mpl::if_<
        mpl::less_equal< BitsUsed, mpl::size_t<8> >,
        uint_least8_t,
        typename mpl::if_<
            mpl::less_equal< BitsUsed, mpl::size_t<16> >,
            uint_least16_t,
            #ifndef BOOST_NO_INT64_T
            typename mpl::if_<
                mpl::less_equal< BitsUsed, mpl::size_t<32> >,
                uint_least32_t,
                uint_least64_t
            >::type
            #else
            uint_least32_t
            #endif
        >::type
    >::type         type;
};

}} // end boost::details

#endif
