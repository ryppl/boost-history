//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include "test_type_list.hpp"
#include <boost/integer/bit_mask.hpp>

template <typename T>
void test_function() {
    // making sure that the value type is transfered correctly.
    BOOST_ASSERT((is_same< typename  bit_mask<T, 3>::value_type, T >::value));

    // basic testing only using the offset.
    BOOST_ASSERT(( boost::bit_mask<T,0>::value == 1 ));
    BOOST_ASSERT(( boost::bit_mask<T,1>::value == 2 ));
    BOOST_ASSERT(( boost::bit_mask<T,2>::value == 4 ));
    BOOST_ASSERT(( boost::bit_mask<T,3>::value == 8 ));
    BOOST_ASSERT(( boost::bit_mask<T,4>::value == 16 ));


    // testing using offset + width.
    BOOST_ASSERT(( boost::bit_mask<T,3,2>::value == 24 ));
    BOOST_ASSERT(( boost::bit_mask<T,2,3>::value == 28 ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same< 
                    typename bit_mask<T, 3>::type,
                    bit_mask<T, 3> >::value
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
