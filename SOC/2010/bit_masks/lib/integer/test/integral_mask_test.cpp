//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



#include "test_type_list.hpp"
#include <boost/integer/high_low_bits.hpp>

// =============================================================================
//                      Testing for integral_mask type.
// =============================================================================

using namespace boost;

template <typename T>
void test_function() {

    // making sure that the value type is transfered correctly.
    BOOST_ASSERT((is_same< typename integral_mask<T, 3>::value_type, T >::value));

    
    BOOST_ASSERT(( boost::integral_mask<T,1>::value == 1 ));
    BOOST_ASSERT(( boost::integral_mask<T,2>::value == 2 ));
    BOOST_ASSERT(( boost::integral_mask<T,3>::value == 3 ));
    BOOST_ASSERT(( boost::integral_mask<T,4>::value == 4 ));
    BOOST_ASSERT(( boost::integral_mask<T,5>::value == 5 ));
    BOOST_ASSERT(( boost::integral_mask<T,6>::value == 6 ));
    BOOST_ASSERT(( boost::integral_mask<T,7>::value == 7 ));
    BOOST_ASSERT(( boost::integral_mask<T,8>::value == 8 ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same< 
                    typename integral_mask<T, 3>::type,
                    integral_mask<T, 3> >::value
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


