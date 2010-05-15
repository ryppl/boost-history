//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include "test_type_list.hpp"
#include <boost/integer/bit_width.hpp>

// tesitng for bit width.
using namespace boost;


template <typename T>
void test_function() {
    BOOST_ASSERT(( bit_width<T>::value == sizeof(T)*8 ));
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

    typedef bit_width<char> bw;
    BOOST_ASSERT(( is_same< bw::value_type, unsigned int>::value ));
    BOOST_ASSERT(( is_same<bw::type, bw>::value ));

    return 0;
}

/*
int main() {
    typedef bit_width<char> bw;
    BOOST_ASSERT(( bw::value == 8 ));
    BOOST_ASSERT(( is_same< bw::value_type, unsigned int>::value ));
    BOOST_ASSERT(( is_same<bw::type, bw>::value ));
    return 0;
}
*/
