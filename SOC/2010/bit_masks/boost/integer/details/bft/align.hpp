//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_ALIGNED_HPP
#define BOOST_BITFIELD_TUPLE_ALIGNED_HPP
#include <cstddef>

namespace boost {


/** Allows the alignment to be set to the next bit which has the same alignment
 *  as the as AlignTo specifies relative to the current offset in bits.
 */
template <std::size_t AlignTo>
struct bit_align;

} // end boost

#endif
