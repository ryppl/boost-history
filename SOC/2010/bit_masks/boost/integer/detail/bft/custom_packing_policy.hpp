//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_CUSTOM_PACKING_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_CUSTOM_PACKING_POLICY_HPP
#include <cstddef>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/integer/detail/bft/mask_shift_indicators.hpp>

namespace boost { namespace detial {


template <
    typename Mask,
    typename ValueType,
    typename Offset,
    typename Width
>
struct custom_packing_policy {
    typedef Mask                mask;
    typedef ValueType           value_type;
    
    template <typename StorageType>
    struct apply {
        typedef StorageType     storage_type;
        static value_type get(storage_type storage);
        static storage_type set(storage_type storage, value_type ptr);
    };
};

}} // end boost::detail

#endif
