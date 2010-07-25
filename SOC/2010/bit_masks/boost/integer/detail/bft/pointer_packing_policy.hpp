//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP
#include <cstddef>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/integer/detail/bft/mask_shift_indicators.hpp>

namespace boost { namespace detail {

// detial's relating to the set and get functions.
namespace policy_detail {

template <
    typename Mask,
    typename ValueType,
    typename StorageType,
    typename Shift
>
struct apply_impl;

template <
    typename Mask,
    typename ValueType,
    typename StorageType
>
struct apply_impl <Mask,ValueType,StorageType,bit_shift::none > {

    typedef ValueType       value_type;
    typedef StorageType     storage_type;

    typedef Mask get_mask;

    typedef integral_constant<
        storage_type,
        ~storage_type( get_mask::value )
    >                       set_mask;



    static value_type get(storage_type storage) {
        return value_type( storage & get_mask::value );
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (set_mask::value & storage)
                |
            (get_mask::value & typename get_mask::value_type(ptr))
        );
    }
};

template <
    typename Mask,
    typename ValueType,
    typename StorageType,
    std::size_t Shift
>
struct apply_impl <Mask,ValueType,StorageType,bit_shift::right<Shift> > {

    typedef ValueType       value_type;
    typedef StorageType     storage_type;

    typedef Mask            get_from_ptr_mask;
    typedef typename mpl::shift_right<    
        integral_constant<
            storage_type,
            Mask::value
        >,
        mpl::size_t<Shift>
    >::type                 get_mask;

    typedef integral_constant<
        storage_type,
        ~get_mask::value
    >                       set_mask;


    static value_type get(storage_type storage) {
        return value_type( (get_mask::value & storage) << Shift);
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            ((storage_type(ptr) & get_from_ptr_mask::value) >> Shift )
        );
    }
};

template <
    typename Mask,
    typename ValueType,
    typename StorageType,
    std::size_t Shift
>
struct apply_impl <Mask,ValueType,StorageType,bit_shift::left<Shift> > {

    typedef ValueType       value_type;
    typedef StorageType     storage_type;
    typedef Mask            get_from_ptr_mask;
    typedef typename mpl::shift_left<    
        integral_constant<
            storage_type,
            Mask::value
        >,
        mpl::size_t<Shift>
    >::type                 get_mask;


    typedef integral_constant<
        storage_type,
        ~get_mask::value
    >                           set_mask;

    static value_type get(storage_type storage) {
        return value_type( (storage & get_mask::value) >> Shift );
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            ((storage_type(ptr) & get_from_ptr_mask::value) << Shift )
        );
    }
};




template<
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width,
    typename StorageType
>
struct deduce_policy {
 
    // getting front storage.
    typedef typename pointer_member::count_leading_zeros<
        Mask>::type                                 front_storage_space;

    typedef typename pointer_member::count_trailing_zeros<
        Mask>::type                                 back_storage_space;

     // getting the basic information for calculating the shift on 
    // a mask 
    typedef typename mpl::minus<
        mpl::int_< bit_width<StorageType>::value >,
        mpl::int_<
            mpl::plus<
                Offset,
                Width,
                back_storage_space
            >::type::value
        >
    >::type             mask_shift_value;

    typedef typename mpl::if_<
        mpl::less<
            mask_shift_value,
            mpl::int_<0>
        >,
        // less then.
        bit_shift::right<
            mpl::minus<mpl::int_<0>, mask_shift_value >::type::value
        >,
        typename mpl::if_<
            mpl::equal_to< mpl::int_<0>, mask_shift_value>,
            // equal to so no shift
            bit_shift::none,
            // greater then so go right.
            bit_shift::left< mask_shift_value::value >
        >::type
    >::type                                 mask_shift;

    typedef apply_impl<Mask,ValueType,StorageType,mask_shift>   type;
};

} // end policy_detail

template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width
> 
struct pointer_packing_policy {
    typedef Mask                mask;
    typedef ValueType           value_type;
    

    template <typename StorageType>
    struct apply
        : policy_detail::deduce_policy<
            Mask,
            ValueType,
            Offset,
            Width,
            StorageType
        >::type
    { };
};

}} // end boost::detial
#endif
