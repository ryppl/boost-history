
#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>



#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>

// Testing of low_bits function.
using namespace boost;

// testing additional integral types.

// char
// unsigned char
// short
// unsigned short
// long
// unsigned long
// int
// unsigned int
// long long
// unsigned long long

/*
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



// BOOST_AUTO_TEST_CASE( my_test )
// {
//     BOOST_CHECK_EQUAL( (boost::low_bits<int, 3>::value) , 7 );
//}
// BOOST_AUTO_TEST_CASE_TEMPLATE( my_test, T, test_types )
// {
    
    // making sure that the value type is transfered correctly.
    // BOOST_CHECK( bool i(is_same< typename low_bits<T, 3>::value_type, T >::value) );

    // assert that mask is correct.
    // T r = typename ::;
    // BOOST_CHECK( 1 );

    // assert that type returns the correct typedef.
    // BOOST_CHECK( bool i2(is_same< typename low_bits<T, 3>::type, low_bits<T, 3> >::value) );
// }

typedef boost::mpl::list<int,long,unsigned char> test_types;



template <typename T>
void test_function() {

    // making sure that the value type is transfered correctly.
    BOOST_ASSERT((is_same< typename low_bits<T, 3>::value_type, T >::value));

    // assert that mask is correct.
    // T r = typename ;
    BOOST_ASSERT(( boost::low_bits<T,3>::value ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same< typename low_bits<T, 3>::type, low_bits<T, 3> >::value ));
}

// basic typedef
// typedef low_bits<unsigned int, 3> lbits;

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
