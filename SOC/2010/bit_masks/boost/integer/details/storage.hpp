//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASKS_TAGGED_HPP
#define BOOST_BIT_MASKS_TAGGED_HPP

#include <allocator>
#include <boost/mpl/void.hpp>
namespace boost {

/** This forces the storage policy to allocate using the stack. 
 *  in stead of using an allocator.
 */
struct storage_policy_stack;

/** This structure is used for specifying the storage type within
 *  bitfield tuple and bit_mask_tuple.
 *  This allos allows the user to specify their own allocator for the internal
 *  internal bit data. Thinking about adding a possible specialization 
 *  for stack allocating the underlying type via a tag or something similar.
 *  In the case of the default allocator there is no need to rebind the 
 *  allocator however if the user wishes to supply their own allocator
 *  rebinding must be done just in case its specified incorrectly.
 */
template < typename StorageType,
           typename Alloc = typename std::allocator< StorageType >
>
struct storage {
    typedef StorageType                                 storage_type;
    typedef Alloc::template rebind<StorageType>::type   alloc;
    typedef storage<StorageType, Alloc>                 type;
};


/** The storage policy specifed is to use stack allocation, which means 
 *  that an instance of Storage type will need to be created. The instance
 *  is NOT the responsibility of this class. The responsibility of this class
 *  is only to hold policies related to the underlying storage type. It is
 *  the responsibility of the class which revieves this as a template parameter
 *  to then deal with it accordingly.
 */ 
template < typename StorageType>
struct storage<StorageType< StorageType, storage_policy_stack > {
    typedef StorageType                                 storage_type;
    typedef storage_policy_stack                        alloc;
    typedef storage<StorageType, storage_policy_stack>  type;
};

} // end boost

#endif
