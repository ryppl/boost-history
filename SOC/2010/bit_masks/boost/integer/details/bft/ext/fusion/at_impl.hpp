//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_AT_IMPL_HPP
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_AT_IMPL_HPP

namespace boost {
    
struct bitfield_group_tag;

namespace fusion { namespace extension {

   
template <typename> struct at_impl;

template <>
struct at_impl< bitfield_group_tag > {

    template <typename BitfieldTuple, typename N>
    struct apply {
        typedef typename BitfieldTuple::
            template bit_ref<
                mpl::at<
                    typename BitfieldTuple::members,
                    N
                >::type
            >                             type;

        // non-const at function.
        static type call(BitfieldTuple& bft) {
            return type( bft.data() );
        }
    };
};

}}} // end boost::fusion::extension




#endif
