//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_BEGIN_HPP
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_BEGIN_HPP

namespace boost {

struct bitfield_tuple_tag;

namespace fusion { namespace extension {

template <typename>
struct begin_impl;

template<>
struct begin_impl< bitfield_tuple_tag > {
    template<typename BitfieldTuple>
    struct apply {
        typedef bitfields::bitfield_tuple_iterator<BitfieldTuple,0> type;
        static type call(BitfieldTuple& bft) {
            return type(bft);
        }
    };
};

}}} // end boost::fusion::extension

#endif
