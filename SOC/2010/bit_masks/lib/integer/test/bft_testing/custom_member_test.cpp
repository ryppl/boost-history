//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits/is_same.hpp>

using namespace boost;
using namespace boost::bitfields;

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

struct cust;

int main() {
    {
        typedef bitfield_tuple<
            custom<
                unsigned int,
                cust,
                bits_mask<unsigned int, 3, 20>,
                custom_packing_policy
            >
        >                       custom_t1;

        custom_t1 t1;
    }
    return 0;
}
