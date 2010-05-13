
#include <boost/test/execution_monitor.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>
// Testing of low_bits function.
using namespace boost;

// basic typedef
typedef low_bits<unsigned int, 3> lbits;

int main() {

    // making sure that the value type is transfered correctly.
    BOOST_ASSERT(( is_same< lbits::value_type, unsigned int >::value ));

    // assert that mask is correct.
    BOOST_ASSERT(( lbits::value == 7 ));

    // assert that type returns the correct typedef.
    BOOST_ASSERT(( is_same<lbits::type, lbits>::value ));

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
    
    return 0;
}
