//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFELD_TUPLE_FUSION_EXT_SIZE_HPP
#define BOOST_BITFELD_TUPLE_FUSION_EXT_SIZE_HPP
#include <boost/mpl/size.hpp>


namespace boost {
    
struct bitfield_tuple_tag;

namespace fusion { namespace extension {

   
template <typename> struct size_impl;

template <>
struct size_impl< bitfield_tuple_tag > {

    template <typename BitfieldTuple>
    struct apply {
        typedef typename mpl::size<
            typename BitfieldTuple::members
        >::type type;

        // non-const at function.
        static type call(BitfieldTuple& bft) {
            return type();
        }
    };
};

}}} // end boost::fusion::extension
#endif
