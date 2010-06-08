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
namespace detials {


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
          typename NameVactor  = typename mpl::vector<>,
          std::size_t NextOffset = 0
>
struct bitfield_tuple_info_ {
    typedef Storage storage_policy;
    typedef FieldVector  field_vector;
    typedef NameVactor   name_vector;
    BOOST_STATIC_CONSTANT( std::size_t, next_offset = NextOffset );


    typedef bitfield_tuple_info_ <
        storage_policy,
        field_vector,
        name_vector,
        NextOffset
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
        BOOST_STATIC_ASSERT(( is_same< storage_policy, mpl::void_ >::value  ));
        typedef typename bitfield_tuple_info_ <
            NewStorageType,
            field_vector,
            name_vector,
            NextOffset
        >::type type;
    };

    /** Used to add a single structure into the field_vector. */
    template <typename TypeToAdd>
    struct add_field {

        typedef typename bitfield_tuple_info_ <
            storage_policy,

            // adding parameter into the element type.
            typename mpl::push_back <
                field_vector,
                typename bitfield_element_ <
                    typename TypeToAdd::return_type,
                    typename TypeToAdd::name,
                    NextOffset,
                    TypeToAdd::field_width
                >::type
            >::type,

            // adding name into the name vector.
            typename mpl::push_back< 
                field_vector,
                typename TypeToAdd::name
            >::type,
            NextOffset + TypeToAdd::field_width
        >::type type;
    };
    //@}
};

/** The generic case fails to intanceate because the data structure recieved
 *  something that wasn't supposed to be in the parameter list.
 */
template <typename ParameterToProcess,
          typename InfoType = bitfield_tuple_info_<>
>
struct bft_process_args_;

/**Specilization over mpl::void_ so they get ignored. */
template <typename InfoType>
struct bft_process_args_< mpl::void_, InfoType > {

    typedef  mpl::void_ processed_param;
    typedef InfoType info_type;

    typedef bft_process_args_< processed_param, info_type > type;

    /** The following meta function is used for rebind thing type each time its 
     *  called and effectivly parsing the types which are supplied to it.
     */
    template <typename NextParam>
    struct process {
       typedef typename bft_process_args_<NextParam, info_type>::type type;
    };
};



/** This specilization is used for processing the storage type. */
template <typename InfoType >
template <typename StorageType, typename AllocType>
struct bft_process_args_< storage< StorageType, AllocType >, InfoType > {

    typedef storage< StorageType, AllocType >  processed_param;
    typedef typename InfoType::template set_storage<
        processed_param
    > info_type;

    typedef bft_process_args_< processed_param, info_type > type;

    /** The following meta function is used for rebind thing type each time its 
     *  called and effectivly parsing the types which are supplied to it.
     */
    template <typename NextParam>
    struct process {
       typedef typename bft_process_args_<NextParam, info_type>::type type;
    };
};

/** "member"  specilization.
 *  This mask sure that the members of the bitfield_tuple are delt with
 *  accoringly.
 */
template <typename InfoType>
template <typename ReturnType, typename NameType, std::size_t FieldWidth>
struct bft_process_args_ <
    member< ReturnType, NameType, FieldWidth >,
    InfoType > 
{
    typedef member< ReturnType, NameType, FieldWidth >  processed_param;
    typedef typename InfoType::template add_field<
        processed_param
    > info_type;

    typedef bft_process_args_< processed_param, info_type > type;

    /** The following meta function is used for rebind thing type each time its 
     *  called and effectivly parsing the types which are supplied to it.
     */
    template <typename NextParam>
    struct process {
       typedef typename bft_process_args_<NextParam, info_type>::type type;
    };
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

    typedef typename bft_process_args_<T0>::
        template process<T1>::type::
        template process<T2>::type::
        template process<T3>::type::
        template process<T4>::type::
        template process<T5>::type::
        template process<T6>::type::
        template process<T7>::type::
        template process<T8>::type::
        template process<T9>::type  processed_args;

    typedef typename processed_args::info_type  info_type;
    typedef typename info_type::storage_policy  storage_policy;
    typedef typename info_type::field_vector    field_vector;
    typedef typename info_type::name_vector     name_vector;

    
    typedef typename mpl::if_<
            is_same<
                typename storage_policy::alloc,
                storage_policy_stack
            >,
        bft_stack_allocated_policy<
            typename storage_policy::storage_type
        >,
        typename storage_policy::alloc
    >::type   allocation_policy;
        
        


    // Precondition: user must supply storage type.
    BOOST_STATIC_ASSERT((
        !is_same<
            typename info_type::storage_type,
            mpl::void_
        >::value
    ));


};

} // end details


template <  typename T0,
            typename T2 = mpl::void_,
            typename T3 = mpl::void_,
            typename T4 = mpl::void_,
            typename T5 = mpl::void_,
            typename T6 = mpl::void_,
            typename T7 = mpl::void_,
            typename T8 = mpl::void_,
            typename T9 = mpl::void_
>
class bitfield_tuple {
    
};



} // end boost

#endif
