//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include <boost/integer/high_low_bits.hpp>
#include "test_type_list.hpp"

using namespace boost;




template <typename T>
void test_function() {

    // making sure that the value type is transfered correctly.
    BOOST_ASSERT((is_same< typename low_bits<T, 3>::value_type, T >::value));

    
    BOOST_ASSERT(( boost::low_bits<T,1>::value == ~(~(T(0)) << 1) ));
    BOOST_ASSERT(( boost::low_bits<T,2>::value == ~(~(T(0)) << 2) ));
    BOOST_ASSERT(( boost::low_bits<T,3>::value == ~(~(T(0)) << 3) ));
    BOOST_ASSERT(( boost::low_bits<T,4>::value == ~(~(T(0)) << 4) ));
    BOOST_ASSERT(( boost::low_bits<T,5>::value == ~(~(T(0)) << 5) ));
    BOOST_ASSERT(( boost::low_bits<T,6>::value == ~(~(T(0)) << 6) ));
    BOOST_ASSERT(( boost::low_bits<T,7>::value == ~(~(T(0)) << 7) ));
    // BOOST_ASSERT(( boost::low_bits<T,8>::value == ~(~(T(0)) << 8) ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same< 
                    typename low_bits<T, 3>::type,
                    low_bits<T, 3> >::value
              ));
}


struct type_tester {
    template< typename U >
    void operator()(U) {
        test_function<U>();

    }
};


int main( ) {   
    mpl::for_each< test_types   >( type_tester() ); 
    mpl::for_each< test_types_2 >( type_tester() ); 
    mpl::for_each< test_types_3 >( type_tester() ); 
    return 0;
}
