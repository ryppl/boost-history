//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_TAG_OF
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_TAG_OF
#include <boost/integer/bitfield_tuple_fwd.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>

namespace boost { namespace fusion {

    struct bitfield_tuple_tag;
    
    namespace traits {

    /** bit_T_group specilization. */
    template <  typename T0, typename T1, typename T2, typename T3,
                typename T4, typename T5, typename T6, typename T7,
                typename T8, typename T9 >
    struct tag_of< boost::bitfield_tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> >{
        typedef bitfield_tuple_tag type;
    };


}}} // end boost::fusion::traits
#endif

