//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFELD_TUPLE_FUSION_EXT_END_HPP
#define BOOST_BITFELD_TUPLE_FUSION_EXT_END_HPP
#include <boost/mpl/size.hpp>


namespace boost {
    
struct bitfield_tuple_tag;

namespace fusion { namespace extension {

   
template <typename> struct end_impl;

template <>
struct end_impl< bitfield_tuple_tag > {

    template <typename BitfieldTuple>
    struct apply {
        typedef bitfield_tuple_iterator<
            BitfieldTuple,
            mpl::size<
                typename BitfieldTuple::members
            >::value
        >                                   type;

        // non-const at function.
        static type call(BitfieldTuple& bft) {
            return type( bft );
        }
    };
};

}}} // end boost::fusion::extension
#endif
