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
>
 {

    /** Masks for retrieving the value and setting the value of the storage.*/
    //@{
    typedef Mask                get_mask;
    typedef integral_constant<
        typename get_mask::value_type,
        ~get_mask::value
    >                           set_mask;
    //@}

    typedef Offset              offset;
    typedef Width               width;
    typedef ValueType           value_type;

    template <typename StorageType>
    struct apply {
        typedef StorageType         storage_type;

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
