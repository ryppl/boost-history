//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP
#include <cstddef>

namespace boost { namespace detail { namespace bit_shift {

template <std::size_t Shift> struct right;
template <std::size_t Shift> struct left;
struct none;

} // end bit_shift
// detial's relating to the set and get functions.
namespace policy_detail {

/** This is specialization deffered because I can't do it completly until this
 *  pointer because I don't have all the infromation so its easiest to do it
 *  here.
 */
template <
    typename Mask,
    typename Offset,
    typename ValueType,
    typename StorageType,
    bool
>
struct apply_impl_no_shift;


/** Specialization where for the storage type and the size of the mask being
 *  the same.
 */
template <typename Mask, typename Offset, typename ValueType, typename StorageType>
struct apply_impl_no_shift < Mask, Offset, ValueType, StorageType, true > {

    typedef Mask        get_mask;
    typedef integral_constant<
        typename get_mask::value_type,
        ~get_mask::value
    >                   set_mask;
    typedef ValueType   value_type;
    typedef StorageType storage_type;

    static value_type get(storage_type storage ) {
        return value_type(storage & get_mask::value);
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            (typename get_mask::value_type(ptr) & get_mask::value)
        );
    }
};


/** This specialization is for systems whichs support interal types
 *  larger then their pointer type OR in other words the storage type is larger
 *  then the type of the mask.
 */
template <typename Mask, typename Offset, typename ValueType, typename StorageType>
struct apply_impl_no_shift < Mask, Offset, ValueType, StorageType, false > {
    typedef ValueType   value_type;
    typedef StorageType storage_type;
    typedef Mask        mask;
    // this is going to be for computers with integral types LARGER then 
    // the size of their pointer type.
    typedef mpl::size_t<
        bit_width<storage_type>::value - bit_width<typename mask::value_type>::value
    >                           shift_amount;

    typedef typename mpl::shift_left<
            integral_constant<
                typename make_unsigned<storage_type>::type,
                mask::value
            >,
            shift_amount
        >::type                     get_mask;

    typedef integral_constant<
        typename get_mask::value_type,
        ~get_mask::value
    >                               set_mask;

    static value_type get(storage_type storage ) {
        return value_type((storage & get_mask::value) >> shift_amount::value) ;
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            (typename get_mask::value_type(ptr) & Mask::value) << shift_amount::value
        );
    }
};


/** This specialization is for systems whichs support interal types
 *  larger then their pointer type OR in other words the storage type is larger
 *  then the type of the mask. This specialization is for pointers which are
 *  aligned to a 32 bit boundry meaning they can be accessed in the same number
 *  of steps as a 32 bit pointer which is stored within int sized type.
 */
template <typename Mask, typename ValueType>
struct apply_impl_no_shift < Mask, mpl::size_t<32>, ValueType, long long, false > {

    typedef integral_constant<unsigned long long, Mask::value> get_mask;
    typedef integral_constant<
        unsigned long long,
        ~ get_mask::value
    >                       set_mask;
    typedef ValueType   value_type;
    typedef long long storage_type;

    static value_type get(storage_type storage ) {
        return value_type(storage & get_mask::value);
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            (typename get_mask::value_type(ptr) & Mask::value)
        );
    }
};

/** This specialization is for systems whichs support interal types
 *  larger then their pointer type OR in other words the storage type is larger
 *  then the type of the mask. This specialization is for pointers which are
 *  aligned to a 32 bit boundry meaning they can be accessed in the same number
 *  of steps as a 32 bit pointer which is stored within int sized type.
 */
template <typename Mask, typename ValueType>
struct apply_impl_no_shift <
    Mask,
    mpl::size_t<32>,
    ValueType,
    unsigned long long,
    false
> {
    typedef integral_constant<unsigned long long, Mask::value> get_mask;
    typedef integral_constant<
        unsigned long long,
        ~get_mask::value
    >                           set_mask;
    typedef ValueType           value_type;
    typedef unsigned long long  storage_type;

    static value_type get(storage_type storage ) {
        return value_type(storage & get_mask::value);
    }

    static storage_type set(storage_type storage, value_type ptr) {
        return storage_type(
            (storage & set_mask::value)
              |
            (typename get_mask::value_type(ptr) & Mask::value)
        );
    }
};

} // end policy_detail

/** Only specilization exist for no shift and shift right however
 *  that doesn't mean that in the future this can't be adapted for preform
 *  left shift on the pointer.
 */
template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width,
    typename IsAligned,
    typename DirectionShifted
> 
struct pointer_packing_policy;

/** Non-shifted policy. */
template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width
>
struct pointer_packing_policy<
    Mask,
    ValueType,
    Offset,
    Width,
    mpl::true_,
    bit_shift::none
> {


    typedef Mask                mask;
    typedef ValueType           value_type;

    template <typename StorageType>
    struct apply
        :policy_detail::apply_impl_no_shift<
            Mask,
            Offset,
            ValueType,
            StorageType,
            sizeof(StorageType) == sizeof(typename mask::value_type)
        > { };
};




/** Specilization for when the pointer is shifted left. */
template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width,
    std::size_t ShiftAmount
>
struct pointer_packing_policy<
    Mask,
    ValueType,
    Offset,
    Width,
    mpl::false_,
    bit_shift::right<ShiftAmount>
>
{
    typedef Mask                mask;
    typedef Offset              offset;
    typedef Width               width;
    typedef ValueType           value_type;

    template <typename StorageType>
    struct apply {
        typedef StorageType         storage_type;

        static value_type get(storage_type storage ) {
            return value_type();
        }

        static storage_type set(storage_type storage, value_type ptr) {
            return storage_type();
        }
    };

};

}} // end boost::detial
#endif
