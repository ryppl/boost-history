//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_ELEMENT_HPP
#define BOOST_BITFIELD_TUPLE_ELEMENT_HPP
#include <boost/integer/bitfield_tuple/interface_metafunctions.hpp>
#include <cstddef>

namespace boost { namespace bitfields {

template<typename BitfieldTuple, std::size_t Index>
struct element {
    typedef typename get_proxy_reference_type_by_index<
        BitfieldTuple,
        Index
    >::type type;
};

template<typename BitfieldTuple, typename Name>
struct element_n {
    typedef typename get_proxy_reference_type_by_name<
        BitfieldTuple,
        Name
    >::type type;
};

}} // end boost::bitfields

#endif
