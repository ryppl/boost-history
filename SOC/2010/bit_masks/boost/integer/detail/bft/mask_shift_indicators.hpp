//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_MASK_SHIFT_INDICATORS_HPP
#define BOOST_BITFIELD_TUPLE_MASK_SHIFT_INDICATORS_HPP
#include <cstddef>

namespace boost { namespace bitfields{ namespace detail { namespace bit_shift {

template <std::size_t Shift> struct right;
template <std::size_t Shift> struct left;
struct none;

}}}} // end boost::bitfields::detail::bit_shift

#endif
