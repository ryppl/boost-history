//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_FILLER_HPP
#define BOOST_BITFIELD_TUPLE_FILLER_HPP
#include <cstddef>

namespace boost {

/** Template is used to specify a size or ammound of bits which is 
 *  not to be used but instead to be filled with padding or unused bits.
 */
template < std::size_t N>
struct filler;

} // end boost

#endif
