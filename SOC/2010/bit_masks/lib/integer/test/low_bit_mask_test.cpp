
#include <boost/test/execution_monitor.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>

// #include <iostream>
// #include <fstream>
// Testing of low_bits function.



using namespace std;
using namespace boost;


typedef low_bits<unsigned int, 3> lbits;

int main() {
    // ofstream log("log.cpp");
    // log << lbits::value << endl;


    // throw(lbits::value);
    // std::cerr << lbits::value << std::endl;
    BOOST_ASSERT(( is_same< lbits::value_type, unsigned int>::value ));
    BOOST_ASSERT(( lbits::value == 7 ));
    // using namespace std;
    
    return 0;
}
