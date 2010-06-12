//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_IMPL_HPP
#define BOOST_BITFIELD_TUPLE_IMPL_HPP


// Integer Includes.
#include <boost/integer/details/storage.hpp>
#include <boost/integer/details/member.hpp>
#include <boost/integer/bits_mask.hpp>
#include <boost/integer/bit_width.hpp>

// std includes
#include <string>
#include <cstddef>

// Other boost includes.
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

// implementation header files.
#include <boost/integer/details/bft/bft_element.hpp>
#include <boost/integer/details/bft/stack_allocation_policy.hpp>
#include <boost/integer/details/bft/allocator_allocation_policy.hpp>
#include <boost/integer/details/bft/arg_parse_impl.hpp>

namespace boost { namespace details {

// The following preprocessor MACRO only used for writing clarity, it's only 
// used twice and then undef'ed before the end of this file.
#define BOOST_BFT_ARG_PROCESSING      \
        details::bft_arg_parse_impl_<\
            T0, \
            mpl::void_,\
            mpl::vector<>,\
            mpl::size_t<0u>\
        >::\
        template process<T1 >::type::\
        template process<T2>::type::\
        template process<T3>::type::\
        template process<T4>::type::\
        template process<T5>::type::\
        template process<T6>::type::\
        template process<T7>::type::\
        template process<T8>::type::\
        template process<T9>::type 

/** bitfield_tuple base class
 *  This class is responsible for preforming static assertions on some 
 *  preconditions and dealing with the parsed arguments. This is the base class 
 *  for bitfield_tuple. This class is responsible for, among other things, 
 *  the allocation policy for the bitfield_tuple. The default uses stack 
 *  type storage_type (for additional information reguarding the 
 *  stack_allocation_policy please see its header file (
 *  boost/integer/details/bft/stack_allocation_policy.hpp).
 *
 *
 *
 *  Base Class preconditions.
 *
 *  1) Storage policy must not be set to mpl::void_.
 *      (this means that the user didn't specify a stroage policy.)
 *      Enforced, May be removed at a later time when the class is able to 
 *          deduce the ammount of storage space, and set the storage policy to
 *          that.
 *
 *  2) The storage type must be a POD type.
 *      (This may be changed to possibly allow for array types of PODs.)
 *      Enforced, This may be best suited to being part of the unpacking
 *          of the arguments so that it can be caught earlier then inside the 
 *          base class.
 *
 *  3) The bitfields specified must not exceed the bitwidth of the storage type.
 *      Enforced. Not going any where, but may be redefined or altered based
 *          on if I allow the supplied storage type to be an array.
 *
 *
 *
 *
 *
 */
template <  typename T0,
            typename T1,
            typename T2,
            typename T3,
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9
>
struct bitfield_tuple_base
    : BOOST_BFT_ARG_PROCESSING
{

    
    typedef typename BOOST_BFT_ARG_PROCESSING   processed_args;

    // extracting te Arguments from processed_args relating to 
    // the storage policy. Also preforming static assertios 
    // where they can be done.
    typedef typename processed_args::storage_policy     storage_policy;
    typedef typename processed_args::field_vector       field_vector;
    typedef typename processed_args::offset             offset;

    // Precondition:
    //      A storage policy must be supplied.
    BOOST_STATIC_ASSERT((
        !is_same<
            storage_policy,
            typename mpl::void_
        >::value
    ));

    typedef typename storage_policy::storage_type   storage_type;

    // this is only defined if the storage type is a type that is not the
    // storage_policy_stack which makes this class simple use a stack to
    // allocate its storage. This leaves an option for the user to select an
    // allocator. 
    typedef typename mpl::if_<
        is_same<
            typename storage_policy::alloc,
            storage_policy_stack
        >,
        typename mpl::void_,
        storage_policy
        >::type                                     allocator;

    // get the template to be used as base class so that the bitfield_tuple
    // class itself can inherit from it.
    typedef typename mpl::if_<
            is_same<
                allocator,
                mpl::void_
            >,
            stack_alloc_base_policy<storage_type>,
            allocator_wraper_base_policy< storage_type, allocator >
        >::type                                     allocation_base_policy;




    // Precondition: storage_type must not be an array type.
    // I always forget if the is_pod will match array types like int [3] or not.
    BOOST_STATIC_ASSERT(( !is_array<storage_type>::value ));

    // precondition: the storage type must be a pod type (for now).
    // NOTE: this may become a documented requirement only.
    BOOST_STATIC_ASSERT(( is_pod<storage_type>::value ));

    // Precondition: the offet at the end of everything must be the
    // the same as or less then the bit_width of the storage type.
    BOOST_STATIC_ASSERT((
        offset::value
            <=
        bit_width< storage_type >::value
    ));

    /** Meta-calculations used for enabling and disabling functionality based
     *  on the allocation policy, number of members, endianness of storage type
     *  signedness of internal storage type and other such things.
     *  The main reason for creating a class such as this is that it makes 
     *  the readability of bitfield_tupe higher.
     *  This will also allow me to quickly deduce whether or not two bft types
     *  are equivilant based on a simple set of concepts about the type itself.
     *
     */
    typedef typename is_same<
        allocator,
        mpl::void_
    >::type                                 is_allocator_allocated;

    typedef typename is_same<
        typename storage_policy::alloc,
        storage_policy_stack
    >::type                                 is_stack_allocated;

};
}} // end boost::details

#undef BOOST_BFT_ARG_PROCESSING
#endif
