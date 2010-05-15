//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>


#include <boost/detail/lightweight_test.hpp>  // for main, BOOST_TEST
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>

using namespace boost;

/*

// testing additional integral types.
char
unsigned char
short
unsigned short
long
unsigned long
int
unsigned int
long long
unsigned long long

integer types from boost integer:

int_least8_t
int_least16_t
int_least32_t
uint_least8_t
uint_least16_t
uint_least32_t


The following types are available only if, after including <boost/cstdint.hpp>,
the macro BOOST_NO_INT64_T is not defined:

int_least64_t
uint_least64_t

The following fastest minimum-width integer types are provided for all platforms:

int_fast8_t
int_fast16_t
int_fast32_t
uint_fast8_t
uint_fast16_t
uint_fast32_t

The following types are available only if, after including <boost/cstdint.hpp>,
the macro BOOST_NO_INT64_T is not defined:

int_fast64_t
uint_fast64_t

/////
typedef low_bits<T, 3> lbits
*/

// testing type list.
typedef boost::mpl::list<
                            char,
                            unsigned char,
                            short,
                            unsigned short,
                            long,
                            unsigned long,
                            int,
                            unsigned int
#if 0
                            ,
                            long long,
                            unsigned long long
#endif
                        > test_types;


template <typename T>
void test_function() {

    // making sure that the value type is transfered correctly.
    BOOST_TEST((is_same< typename low_bits<T, 3>::value_type, T >::value));

    // assert that mask is correct.
    // T r = typename ;
    BOOST_TEST(( boost::low_bits<T,3>::value ));

    // assert that type returns the correct typedef.
    BOOST_TEST(( is_same< typename low_bits<T, 3>::type, low_bits<T, 3> >::value ));

}


struct type_tester {
    template< typename U >
    void operator()(U) {
        test_function<U>();

    }
};


int main( ) {   
    mpl::for_each<test_types>( type_tester() ); 
    return 0;
}
