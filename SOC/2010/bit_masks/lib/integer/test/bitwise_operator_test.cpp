//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include "test_type_list.hpp"
#include <boost/integer/high_low_bits.hpp>
#include <boost/integer/bit_mask.hpp>
// #include <boost/integer/details/bit_mask_function_support.hpp>


// Testing for operator overloads.
using namespace boost;

template <typename T>
void test_function() {

    // operator &(T, bit_mask)
    T bit_result = T(0) & bit_mask<T, 1>();

    // operator &(bit_mask, T)
    bit_result = bit_mask<T, 1>() & T(0);
    
    // operator |(T, bit_mask)
    bit_result = T(0) | bit_mask<T,1>();
    
    // operator |(bit_mask, T)
    bit_result = T(0) | bit_mask<T,1>();

    // operator |(T, bit_mask)
    bit_result = bit_mask<T,1>() | T(0);

    // operator ^(bit_mask, T)
    bit_result = T(0) ^ bit_mask<T,1>();

    // operator ^(T, bit_mask)
    bit_result = bit_mask<T,1>() ^ T(0);
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
