//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASKS_TAGGED_HPP
#define BOOST_BIT_MASKS_TAGGED_HPP

namespace boost {


/** Storage type used for moving the type to be used for interenal storage 
 *  around within the template parameters of a bitfield_tuple.
 */
template < typename StorageType>
struct storage;

} // end boost

#endif
