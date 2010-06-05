//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include "test_type_list.hpp"
#include <boost/integer/high_low_bits.hpp>
#include <boost/integer/bits_mask.hpp>



// Testing for operator overloads.
using namespace boost;

template <typename T>
void test_function() {

    T mask_value( bits_mask<T,1>::value );

    // operator &(T, bits_mask)
    T bit_result = T(0) & bits_mask<T, 1>();
    BOOST_ASSERT(( bit_result == (T(0) & mask_value) ));

    // operator &(bits_mask, T)
    bit_result = bits_mask<T, 1>() & T(0);
    BOOST_ASSERT(( bit_result == (mask_value & T(0)) ));
    
    // operator |(T, bits_mask)
    bit_result = T(0) | bits_mask<T,1>();
    BOOST_ASSERT(( bit_result == (T(0) | mask_value) ));

    // operator |(T, bits_mask)
    bit_result = bits_mask<T,1>() | T(0);
    BOOST_ASSERT(( bit_result == (mask_value | T(0)) ));

    // operator ^(bits_mask, T)
    bit_result = T(0) ^ bits_mask<T,1>();
    BOOST_ASSERT(( bit_result == (T(0) ^ mask_value) ));

    // operator ^(T, bits_mask)
    bit_result = bits_mask<T,1>() ^ T(0);
    BOOST_ASSERT(( bit_result == (mask_value ^ T(0)) ));

    // operator~(bits_mask)
    bit_result = ~ (bits_mask<T,1>());
    mask_value = ~mask_value;
    BOOST_ASSERT(( bit_result == mask_value ));
}


struct type_tester {
    template< typename U >
    void operator()(U) {
        test_function<U>();
    }
};


int main() {
    mpl::for_each< test_types   >( type_tester() ); 
    mpl::for_each< test_types_2 >( type_tester() ); 
    mpl::for_each< test_types_3 >( type_tester() ); 

    return 0;
}
