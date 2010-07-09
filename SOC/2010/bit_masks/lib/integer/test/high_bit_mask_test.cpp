//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include <boost/integer/high_low_bits.hpp>
#include "test_type_list.hpp"
#include <iostream>
#include <typeinfo>

// tesitng for bit width.
using namespace boost;

template <typename T>
void test_function() {

    using namespace std;
    cout << "Starting test. Test Type = " << typeid(T).name() << endl;
    // making sure that the value type is transfered correctly.
    BOOST_ASSERT((is_same< typename high_bits_mask<T, 3>::value_type, T >::value));

    cout << "Mask Value: " << boost::high_bits_mask<T,1>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 1) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,1>::value == ~(~(T(0)) >> 1) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,2>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 2) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,2>::value == ~(~(T(0)) >> 2) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,3>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 3) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,3>::value == ~(~(T(0)) >> 3) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,4>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 4) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,4>::value == ~(~(T(0)) >> 4) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,5>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 5) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,5>::value == ~(~(T(0)) >> 5) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,6>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 6) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,6>::value == ~(~(T(0)) >> 6) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,7>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 7) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,7>::value == ~(~(T(0)) >> 7) ));

    cout << "Mask Value: " << boost::high_bits_mask<T,8>::value << endl;
    cout << "Test Value: " << ~(~(T(0)) >> 8) << endl;
    BOOST_ASSERT(( boost::high_bits_mask<T,8>::value == ~(~(T(0)) >> 8) ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same< 
                    typename high_bits_mask<T, 3>::type,
                    high_bits_mask<T, 3> >::value
              ));
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
