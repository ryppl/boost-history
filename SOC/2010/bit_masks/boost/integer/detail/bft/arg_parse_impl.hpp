//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_BFT_ARG_PARSE_IMPL_HPP
#define BOOST_BITFIELD_TUPLE_BFT_ARG_PARSE_IMPL_HPP
#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/type_traits/add_pointer.hpp>


#include <boost/integer/bitfield_tuple/storage.hpp>
#include <boost/integer/bitfield_tuple/member.hpp>
#include <boost/integer/bitfield_tuple/flag.hpp>
#include <boost/integer/bitfield_tuple/filler.hpp>
#include <boost/integer/bitfield_tuple/align.hpp>
#include <boost/integer/bitfield_tuple/custom.hpp>
#include <boost/integer/bitfield_tuple/pointer.hpp>

#include <boost/integer/detail/bft/name_lookup.hpp>
#include <boost/integer/detail/bft/arg_parsing_meta_functions.hpp>
#include <boost/integer/detail/bft/pointer_packing_policy.hpp>


namespace boost { namespace detail {



/** This is the empty which prevents the user from supply things which are
 *  not enclosed within either a member template or a storage template.
 *  This enforces a domain requirement on the template parameters of
 *  bitfield_tuple.
 */
template <  typename Param,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
> struct bft_arg_parse_impl;


/** Specialization over mpl::void_. */
template <  typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <mpl::void_, StoragePolicy, FieldVector, Offset>{
    typedef mpl::void_           param;
    typedef FieldVector     field_vector;
    typedef StoragePolicy   storage_policy;
    typedef Offset          offset;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
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

            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::storage<StorageType>,
    StoragePolicy,
    FieldVector,
    Offset >
{
    // make sure that the storage type is not specifed twice
    BOOST_STATIC_ASSERT(( is_same<StoragePolicy,mpl::void_>::value ));

    typedef bitfields::storage<
        StorageType
    >                       param;
    typedef FieldVector     field_vector;
    typedef StorageType     storage_policy;
    typedef Offset          offset;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
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
 *          (This may result in additional overhead during compile time ).
 *      2. The Fieldwidth of a field must not be 0.
 *      3. The Fieldwidth must not exceed the bit_width of the ReturnType.
 */
template <  typename StoragePolicy,
            typename FieldVector,
            std::size_t FieldWidth,
            typename ReturnType,
            typename NameType,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::member <
        ReturnType,
        NameType,
        FieldWidth
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    BOOST_STATIC_ASSERT(( FieldWidth != 0 ));
    BOOST_STATIC_ASSERT(( FieldWidth <= bit_width<ReturnType>::value ));
    // make sure that the name doesn't already exist.
    BOOST_STATIC_ASSERT((
        is_same<
            typename mpl::find_if<
                FieldVector,
                detail::match_name<
                    mpl::_1,
                    NameType
                >
            >::type,
            typename mpl::end<
                FieldVector
            >::type
        >::value            
    ));


    typedef bitfields::member< ReturnType, NameType, FieldWidth > param;

    typedef StoragePolicy   storage_policy;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element<
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

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};

/* Specialization for flag. */
template <  typename StoragePolicy,
            typename FieldVector,
            typename NameType,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::flag <NameType>,
    StoragePolicy,
    FieldVector,
    Offset >
    :bft_arg_parse_impl <
        bitfields::member<bool,NameType,1>,
        StoragePolicy,
        FieldVector,
        Offset
    >
{ };

/* Specialization for filler. */
template <  std::size_t PaddingBits,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::filler<
        PaddingBits
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    typedef bitfields::filler<PaddingBits> param;
    typedef FieldVector     field_vector;
    typedef StoragePolicy   storage_policy;
    typedef mpl::size_t<PaddingBits + Offset::value > offset;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};



/* Specialization for bit_align. */
template <  std::size_t AlignTo,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::bit_align<
        AlignTo
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    typedef bitfields::bit_align<AlignTo> param;
    typedef FieldVector     field_vector;
    typedef StoragePolicy   storage_policy;


    // computing the position of the next bit which is aligned
    // to the current value of AlignTo.
    
    // if the modulus result is 0 then we are aligned to the current position.
    // If its not then we actually have to adjust the position and move to the 
    // next bit position which is aligned to to AlignTo's value

    typedef mpl::size_t<AlignTo> align_to;
    typedef typename mpl::modulus<
        Offset,
        align_to
    >::type                 mod_result;

    typedef typename mpl::if_c< mod_result::value == 0, // then
        Offset,
        // else
        typename mpl::plus<
            Offset,
            typename mpl::minus<
                align_to,
                mod_result
            >::type            
        >::type
    >::type                             offset;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };
};





/** Specialization for custom. */
template <  typename ReturnType,
            typename Name,
            typename Mask,
            template<typename,typename,typename,typename> class Policy,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::custom<
        ReturnType,
        Name,
        Mask,
        Policy
    >,
    StoragePolicy,
    FieldVector,
    Offset >
{
    // PRECONDITION: Value bits of the mask Can't be 0.
    BOOST_STATIC_ASSERT(( Mask::value != 0 ));

    typedef typename pointer_member::count_leading_zeros<
        Mask
    >::type                             front_storage_space;

    typedef typename pointer_member::count_trailing_zeros<
        Mask
    >::type                             back_storage_space;
    
    typedef typename mpl::minus<
        mpl::size_t< bit_width< void* >::value>,
        typename mpl::plus<
            front_storage_space,
            back_storage_space
        >::type
    >::type                             size_of_value_mask;


    typedef typename mpl::plus< Offset, size_of_value_mask>::type offset;

    typedef bitfields::custom< ReturnType, Name, Mask, Policy > param;

    typedef StoragePolicy               storage_policy;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element<
            ReturnType,
            Name,
            Offset,
            size_of_value_mask,
            Mask,
            Policy<
                Mask,
                ReturnType,
                Offset,
                size_of_value_mask
            >
        >
    >::type                             field_vector;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };  
};


/** Specialization for pointer. */
template <  typename ReturnType,
            typename Name,
            typename Mask,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::pointer<ReturnType,Name,Mask>,
    StoragePolicy,
    FieldVector,
    Offset >
{
    // PRECONDITION: Value bits of the mask Can't be 0.
    BOOST_STATIC_ASSERT(( Mask::value != 0 ));

    // PRECONDTION: type of mask::value_type must be the same size as a pointer.
    BOOST_STATIC_ASSERT(( sizeof(typename Mask::value_type) == sizeof(void*) ));


    typedef typename pointer_member::count_leading_zeros<
        Mask>::type    front_storage_space;

    typedef typename pointer_member::count_trailing_zeros<
        Mask>::type   back_storage_space;
    
    typedef typename mpl::minus<
        mpl::size_t< bit_width< void* >::value>,
        typename mpl::plus<
            front_storage_space,
            back_storage_space
        >::type
    >::type                             size_of_value_mask;


    typedef typename mpl::plus< Offset, size_of_value_mask>::type offset;

    typedef bitfields::pointer< ReturnType, Name, Mask > param;

    typedef StoragePolicy   storage_policy;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element<
            ReturnType*,
            Name,
            Offset,
            size_of_value_mask,
            Mask,
            pointer_packing_policy<
                Mask,
                ReturnType*,
                Offset,
                size_of_value_mask
            >
        >
    >::type                     field_vector;

    typedef bft_arg_parse_impl<param,storage_policy,field_vector,offset> type;

    template <typename NextParam>
    struct process {
        typedef bft_arg_parse_impl<
            NextParam,
            storage_policy,
            field_vector,
            offset
        > type;
    };  
};


}} // end boost::detail

#endif
