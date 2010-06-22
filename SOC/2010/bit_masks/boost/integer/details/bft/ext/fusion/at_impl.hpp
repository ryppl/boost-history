//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_AT_IMPL_HPP
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_AT_IMPL_HPP
#include <boost/mpl/at.hpp>

namespace boost {
    
struct bitfield_tuple_tag;

namespace fusion { namespace extension {

   
template <typename> struct at_impl;

template <>
struct at_impl< bitfield_tuple_tag > {

    template <typename BitfieldTuple, typename N>
    struct apply {
        typedef typename BitfieldTuple::
            template bit_ref<
                typename mpl::at<
                    typename BitfieldTuple::members,
                    N
                >::type
            >                             type;
        // non-const at function.
        static type call(BitfieldTuple& bft) {
            return type( bft.template get<N::value>() );
        }
    };
};

}}} // end boost::fusion::extension




#endif
