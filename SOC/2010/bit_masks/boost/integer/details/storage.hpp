//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASKS_TAGGED_HPP
#define BOOST_BIT_MASKS_TAGGED_HPP

#include <allocator>

namespace boost {

/** This structure is used for specifying the storage type within
 *  bitfield tuple and bit_mask_tuple.
 *  This allos allows the user to specify their own allocator for the internal
 *  internal bit data. Thinking about adding a possible specialization 
 *  for stack allocating the underlying type via a tag or something similar.
 *
 *
 */
template <  typename StorageType,
            typename Alloc = typename std::allocator<StorageType>
    >
struct storage {
    typedef NameType            name;
    typedef T                   value;
    typedef tagged<NameType, T> type;
};

} // end boost

#endif
