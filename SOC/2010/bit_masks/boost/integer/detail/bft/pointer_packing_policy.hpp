//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_POINTER_PACKING_POLICY_HPP

namespace boost { namespace detail {

namespace bit_shift {
struct right;
struct left;
struct none;
}

template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width,
    typename IsAligned,
    typename DirectionShifted
>
struct pointer_packing_policy {

    typedef Offset              offset;
    typedef Width               width;
    typedef ValueType           value_type;
    typedef IsAligned           is_aligned;
    typedef DirectionShifted    shift_direction;
    template <typename StorageType>
    struct apply {
        typedef StorageType         storage_type;

        // static value_type get(storage_type const& )
    };

};

}} // end boost::detial
#endif
