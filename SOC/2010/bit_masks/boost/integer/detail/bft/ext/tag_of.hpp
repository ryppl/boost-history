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
    template <  BOOST_BFT_TEMPLATE_ARGS_NO_DEFAULTS()>
    struct tag_of< ::boost::bitfield_tuple< BOOST_BFT_SPECILIZATION_PARAM_LIST() > >{
        typedef bitfield_tuple_tag type;
    };


}}} // end boost::fusion::traits
#endif

