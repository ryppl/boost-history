//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_IMPL_HPP
#define BOOST_BITFIELD_TUPLE_IMPL_HPP



#include <boost/integer/details/storage.hpp>
#include <boost/integer/bits_mask.hpp>
#include <string>
#include <boost/integer/details/member.hpp>
#include <boost/integer/bit_width.hpp>
#include <cstddef>
#include <boost/mpl/void.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/map.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer/details/bft_element.hpp>

namespace boost { namespace details {



/** This is the empty which prevents the user from supply things which are
 *  not enclosed within either a member template or a storage template.
 *  This enforces a domain requirement on the template parameters of
 *  bitfield_tuple.
 */
template <  typename Param,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
> struct bft_impl_;


/** Specialization over mpl::void_. */
template <  typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_impl_ <mpl::void_, StoragePolicy, FieldVector, Offset>{
    typedef mpl::void_           param;
    typedef FieldVector     field_vector;
    typedef StoragePolicy   storage_policy;
    typedef Offset          offset;

    typedef bft_impl_<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_impl_<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};

/** Specilization for storage type.
 *  Preconditions enforced on this function :
 *      For now its going to be documented but not enforeced.
 *      Do NOT set the storage type more then once!
 */
template <  typename StorageType, 
            typename AllocationPolicy,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_impl_ <
    storage<
        StorageType,
        AllocationPolicy
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    typedef typename storage<
        StorageType,
        AllocationPolicy
    >::type                 param;
    typedef FieldVector     field_vector;
    typedef param           storage_policy;
    typedef Offset          offset;

    typedef bft_impl_<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_impl_<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};

/** Specilization for member.
 *  Documented and enforced preconditions
 *      1. The user must not supply the same name for more then 1 parameter
 *      (This may result in additional overhead during compile time ).
 *      Currently not enforced, will take more time then I have at the moment.
 */

// TODO: Implement Precondition 1 listed above!
template <  typename StoragePolicy,
            typename FieldVector,
            std::size_t FieldWidth,
            typename ReturnType,
            typename NameType,
            typename Offset
>
struct bft_impl_ <
    member <
        ReturnType,
        NameType,
        FieldWidth
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    typedef member< ReturnType, NameType, FieldWidth > param;

    // typedef 
    typedef StoragePolicy   storage_policy;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element_<
            ReturnType,
            NameType,
            Offset,
            mpl::size_t<FieldWidth>
        >
    >::type field_vector;

    typedef mpl::size_t< 
        mpl::plus<
            Offset,
            mpl::size_t<FieldWidth>
        >::value
    >                                   offset;

    typedef bft_impl_<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_impl_<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};

/** This structure is going to be used when an allocator isn't assked for.
 *  This means that the entire class is allocated on the stack.
 */
template <typename T>
struct stack_alloc_base_policy {
    /** Default Constructor. */
    stack_alloc_base_policy()
        :_data()
    { }

    /** Copy constructor. */
    stack_alloc_base_policy( stack_alloc_base_policy<T> const& x )
        :_data( x._data )
    { }
    
    /** Value constructor. */
    stack_alloc_base_policy( T x )
        :_data( x )
    { }
    
    stack_alloc_base_policy<T> const& operator=( stack_alloc_base_policy const& x ) {
        _data  =  x._data;
        return *this;
    }

    /** get_data () this function exists within this class and the 
     *      allocator_base_policy.
     *  Returns a copy of _data.
     */
    //@{
    T const get_data() const { return _data; }
    T get_data() { return _data; }
    //@}

    T _data;
};

/** This isn't implemented yet and shouldn't be used just yet.
 *  This is eventually going to wrap up an pointer to an allocated object which
 *  will be retrievable via a alloc_get function.
 */
template <typename T, typename Alloc>
struct allocator_wraper_base_policy {
    T const get_data() const { return _data; }
    T get_data() { return _data; }
    T _data;
};

// Only used for writing clarity and only used once and then undef'ed when
// before the end of the details namespace/after the end of the
// bitfield_tuple_base


#define BOOST_BFT_ARG_PROCESSING_CLASS      \
        details::bft_impl_<\
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
 *  This class is responsible for preforming static assertions and dealing with
 *  the parsing of arguments.
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
    : BOOST_BFT_ARG_PROCESSING_CLASS
{

    
    typedef typename BOOST_BFT_ARG_PROCESSING_CLASS   processed_args;

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
     *  on the allocation policy, number of members etc...
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

#undef BOOST_BFT_ARG_PROCESSING_CLASS
}} // end boost::details

#endif
