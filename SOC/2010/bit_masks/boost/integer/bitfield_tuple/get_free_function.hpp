//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_GET_FREE_FUNCTION_HPP
#define BOOST_BITFIELD_TUPLE_GET_FREE_FUNCTION_HPP
#include <cstddef>
#include "element.hpp"

namespace boost { namespace bitfields {

template <std::size_t Index, typename BitfieldTuple>
inline typename element<BitfieldTuple,Index>::type get(BitfieldTuple& bft) {
    return bft.get<Index>();
}

template <typename Name, typename BitfieldTuple>
inline typename element_n<BitfieldTuple,Name>::type get(BitfieldTuple& bft) {
    return bft.get<Name>();
}

template <std::size_t Index, typename BitfieldTuple>
inline typename element<const BitfieldTuple,Index>::type
get(BitfieldTuple const& bft) {
    return bft.get<Index>();
}

template <typename Name, typename BitfieldTuple>
inline typename element_n<const BitfieldTuple,Name>::type
get(BitfieldTuple const& bft) {
    return bft.get<Name>();
}

}} // boost::bitfields

#endif
