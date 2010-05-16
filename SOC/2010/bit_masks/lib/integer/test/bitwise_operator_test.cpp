//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include "test_type_list.hpp"
#include <boost/integer/high_low_bits.hpp>
#include <boost/integer/bit_mask.hpp>
#include <boost/integer/details/bit_mask_function_support.hpp>

void ignore_unused(...) { }

// Testing for operator overloads.
using namespace boost;

template <typename T>
void test_function() {
    T bitwise_and = T(0) & bit_mask<T, 1>()();
    ignore_unused( bitwise_and );
}


struct type_tester {
    template< typename U >
    void operator()(U) {
        test_function<U>();
    }
};


int main() {
    mpl::for_each< test_types   >( type_tester() ); 
    // mpl::for_each< test_types_2 >( type_tester() ); 
    // mpl::for_each< test_types_3 >( type_tester() ); 
    char bitwise_and = char(0) & bit_mask<char, 1>();
    ignore_unused( bitwise_and );
    return 0;
}
