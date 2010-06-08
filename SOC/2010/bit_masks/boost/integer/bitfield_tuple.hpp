//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_HPP
#define BOOST_BITFIELD_TUPLE_HPP
#include <boost/integer/details/storage.hpp>
#include <boost/integer/details/member.hpp>
#include <cstddef>
#include <boost/mpl/void.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/map.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/if.hpp>


namespace boost {
namespace details {


/** This is the type which sits inside the mpl vector type.
 *  This has all information for constructing a bitfield.
 */
template <  typename ReturnType,
            typename NameType, 
            std::size_t Offset,
            std::size_t FieldWidth
>
struct bitfield_element_ {
    typedef ReturnType  return_type;
    typedef NameType    name;
    BOOST_STATIC_CONSTANT( std::size_t, offset = Offset );
    BOOST_STATIC_CONSTANT( std::size_t, width = FieldWidth );
    typedef bitfield_element_<return_type, name, offset, width> type;
};

/** Template Parameter descriptions and behaviors.
 *  Storage - Is the storage type. The behavior for this is going to be
 *      static assertion if set_storage is called and storage_type is something
 *      other then mpl::void_.
 *
 *  MapType - This is the associative sequence used for associating names
 *      with internal types and bitfields. The intended end use of this
 *      structure is to provided a simplified interface for a tuple via
 *      tag parameters.
 *
 *  VectorType - This sequence may be the only thing which is actually needed
 *      to store all information about types the different parameters.
 *
 */
template <typename Storage = mpl::void_,
          typename FieldVector = typename mpl::vector<>,
          std::size_t NextOffset = 0
>
struct bitfield_tuple_info_ {
    typedef Storage         storage_policy;
    typedef FieldVector     field_vector;
    BOOST_STATIC_CONSTANT( std::size_t, next_offset = NextOffset );


    typedef bitfield_tuple_info_ <
        storage_policy,
        field_vector,
        next_offset
    > type;

    /** Internal meta functions which are used to abstract overly complex 
     *  operations which would normally be done via rebinding but that it
     *  a massive pain. This is simplier to deal with from a usability
     *  stand point as well as readability.
     */
    //@{

    /** Used for setting the storage type associated with the current template.
     *  Preconditions
     *      Must not be called twice.
     *      Unless called with mpl::void_ multiple times
     *      (which one shouldn't do)(that is in a row before its set to any
     *      thing else).
     */
    template <typename NewStorageType>
    struct set_storage {
        // BOOST_STATIC_ASSERT(( !is_same< storage_policy, mpl::void_ >::value  ));
        
        typedef bitfield_tuple_info_ <
            NewStorageType,
            field_vector,
            next_offset
        > type;
    };

    /** Used to add a single structure into the field_vector. */
    template <typename TypeToAdd>
    struct add_field {

        typedef bitfield_tuple_info_ <
            storage_policy,

            // adding parameter into the element type.
            typename mpl::push_back <
                field_vector,
                typename bitfield_element_ <
                    typename TypeToAdd::return_type,
                    typename TypeToAdd::name,
                    next_offset,
                    TypeToAdd::field_width
                >::type
            >::type,
            NextOffset + TypeToAdd::field_width
        > type;
    };
    //@}
};



// this is forward declaration for something I'm going to do later.
template <typename T> struct bft_stack_allocated_policy;

/** bft means bitfield_tuple its just shorter to write then bitfield tuple.
 *  This class is used to select the correct base classes which the bitfield
 *  tuple should inherit from, as well as deal with the messy meta programming.
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
struct bft_impl_ {
/*
    typedef typename bft_process_args_< T0, bitfield_tuple_info_ < > >::type pa1;
    BOOST_STATIC_ASSERT((
        !is_same<
            typename pa1::info_type::storage_policy,
            mpl::void_
         >::value
    ));

    typedef typename pa1::template process< T1 >::type pa2;
    BOOST_STATIC_ASSERT((
        !is_same<
            typename pa1::info_type::storage_policy,
            mpl::void_
         >::value
    ));
    typedef typename pa2::template process< T2 >::type pa3;
    BOOST_STATIC_ASSERT((
        !is_same<
            typename pa1::info_type::storage_policy,
            mpl::void_
         >::value
    ));
    typedef typename pa3::template process< T3 >::type pa4;
    typedef typename pa4::template process< T4 >::type pa5;
    typedef typename pa5::template process< T5 >::type pa6;
    typedef typename pa6::template process< T6 >::type pa7;
    typedef typename pa7::template process< T7 >::type pa8;
    typedef typename pa8::template process< T8 >::type pa9;
    typedef typename pa9::template process< T9 >::type pa10;

                template process<T2>::type::
                template process<T3>::type::
                template process<T4>::type::
                template process<T5>::type::
                template process<T6>::type::
                template process<T7>::type::
                template process<T8>::type::
                template process<T9>::type  processed_args;

    typedef pa10 processed_args;

    typedef typename processed_args::info_type  info_type;
    typedef typename info_type::storage_policy  storage_policy;
    typedef typename info_type::field_vector    field_vector;


    // Precondition: user must supply storage type.
//    BOOST_STATIC_ASSERT((
//        !is_same<
//            typename info_type::storage_policy,
//            mpl::void_
//        >::value
//    ));
    
    typedef typename mpl::if_<
            is_same<
                typename storage_policy::alloc,
                storage_policy_stack
            >,
        // true
        bft_stack_allocated_policy<
            typename storage_policy::storage_type
        >
        , // else
        typename storage_policy::alloc
    >::type                                     allocation_policy;
*/
};

} // end details


template <  typename T0,
            typename T1 = mpl::void_,
            typename T2 = mpl::void_,
            typename T3 = mpl::void_,
            typename T4 = mpl::void_,
            typename T5 = mpl::void_,
            typename T6 = mpl::void_,
            typename T7 = mpl::void_,
            typename T8 = mpl::void_,
            typename T9 = mpl::void_
>
class bitfield_tuple
    :public details::bft_impl_<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
{

};



} // end boost

#endif
