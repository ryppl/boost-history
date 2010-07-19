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
#include <boost/integer/detail/bft/pointer_parsing_meta_functions.hpp>
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
            typename Policy,
            typename StoragePolicy,
            typename FieldVector,
            typename Offset
>
struct bft_arg_parse_impl <
    bitfields::custom<ReturnType,Name,Mask,Policy>,
    StoragePolicy,
    FieldVector,
    Offset >
{
    /*
    TODO: This needs to be better defined!
    typedef bitfields::custom<ReturnType,Name,Mask,Policy>  param;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element<
            ReturnType,
            NameType,
            Offset,
            mpl::size_t<FieldWidth>
        >
    >::type                 field_vector;

    typedef StoragePolicy   storage_policy;



    // typedef offset;

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
    */
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
    /** What must be calculated writen out so I can see it better.
     *
     *  1.) Make sure that the mask for the pointer isn't 0.
     *      A.) if it is that means that NOTHING is being stored in the pointer
     *      and everything else can be ignroed (Not sure if this should be a
     *      PRECONDITION that causes a static_assertion or not).
     *      B.) Make sure the mask is the same size as a pointer.
     *      If it is NOT the same size as a pointer, then cause a static 
     *      assertion.
     *
     *  2.) Determin the offset into the mask.
     *      A.) What is mean by this is that I need know which bits wihtin the
     *      mask that have value are respected as such and the ones which don't
     *      are able to be used for extra storage.
     *      B.) Summary: Look for leading zeros and look for trailing zeros.
     *      Those are the places which values can be stored within.
     *      C.) PRECONDITION: The offset must be less then or equal to the number
     *      of leading 0's within the value of the pointer.
     *      D.) Behavior in the case that there are leading zeros wihtin the
     *      mask and the offset is not the same as the amound of leading zeros
     *      within the mask. The zeros are treated as filler and the pointer is
     *      always stored relative to the mask provided.
     *      
     *
     *  3.) The offset of the storage location of the pointer shall be relative
     *  to the first 1 within the mask provided.
     */

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
    >::type                             size_of_storage;
/*
Info to know about the curret state of parsing.
    1) Is the curret front_storage_space zero?
        This means that there are no leading zeros.
    2) Is the rear storage space zero.
        there is no trailing zeros.
    3) number of bits being stored for the pointer.
        calculated as the number of bits in the mask -
        (back_storage_space + front_storage_space)
        
*/
    

    // Logic for determining how to actually store the pointer.
    typedef typename mpl::if_c<front_storage_space::value == 0,
    mpl::false_,mpl::true_>::type                  uses_front_storage;

    typedef typename mpl::if_c<back_storage_space::value == 0,
    mpl::false_,mpl::true_>::type                  uses_back_storage;

    // getting starting position of pointer for storage inside of 
    // bitfield_tuples storage.
    typedef typename mpl::if_<
        uses_front_storage,
        typename mpl::if_<
            typename mpl::less<
                front_storage_space,
                Offset
            >::type,
            // the pointer has been pushed because the user has added leading 
            // members to the bitfield_tuple greater then the number of bits
            // the mask allows for them to store directly directly within the
            // pointer. This may need to be a precondition that causes failure.

            // TODO: REMOVE THIS LATER ITS ONLY HERE FOR THE MOMENT TO CAUSE
            // FAILURE because I don't know how I should implement this part
            // just yet and thinking about it is hurting my head.
            mpl::size_t<sizeof(mpl::void_)>::type,
            typename mpl::minus<front_storage_space, Offset>::type
        >::type,
        Offset
    >::type                                 data_offset;

    // calculating next offset/width for the next field.
    typedef typename mpl::plus< size_of_storage, data_offset >::type offset;
 
    typedef bitfields::pointer< ReturnType, Name, Mask > param;

    // if the offset is greater then then number of storage bits
    // the this pointer is not going to be aligned within storage.
    typedef typename mpl::if_<
        mpl::greater<
            Offset,
            front_storage_space
        >,
        typename mpl::if_<
            mpl::and_<
                mpl::not_<
                    uses_front_storage
                >,
                integral_constant<
                    bool,
                    (Offset::value == 32)
                >
            >,
            mpl::true_,
            mpl::false_
        >::type,
        mpl::true_
    >::type                     is_aligned;

    //
    typedef typename mpl::if_<is_aligned,
        bit_shift::none,
        bit_shift::right< mpl::minus<Offset, front_storage_space>::type::value >
    >::type                     alignment_shift;

    typedef StoragePolicy   storage_policy;
    typedef typename mpl::push_back<
        FieldVector,
        bitfield_element<
            ReturnType*,
            Name,
            data_offset,
            size_of_storage,
            Mask,
            pointer_packing_policy<
                Mask,
                ReturnType*,
                offset,
                size_of_storage, 
                is_aligned,
                alignment_shift
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
