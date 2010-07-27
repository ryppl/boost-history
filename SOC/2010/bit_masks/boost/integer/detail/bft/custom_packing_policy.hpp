//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_CUSTOM_PACKING_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_CUSTOM_PACKING_POLICY_HPP
#include <boost/integer/detail/bft/policy_creation_detail.hpp>

namespace boost { namespace bitfields{ namespace detail {

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

}}} // end boost::detail

#endif
