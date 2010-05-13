
#include <boost/test/execution_monitor.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/integer/high_low_bits.hpp>
#include <except>
#include <string.h>
// Testing of low_bits function.



using namespace std;
using namespace boost;


struct bit_exception
    :except
{
    void setValue(char* c) {
        m_value = c;
    }

    virtual const char* what() const {
        return "Failure: value is: " + m_value + "\n";
    }

    char* m_value;
};
typedef low_bits<unsigned int, 3>::type lbits;

int main() {

    bit_exception be;
    be.setValue( );
    throw(be);

    // throw(lbits::value);
    // std::cerr << lbits::value << std::endl;
    BOOST_ASSERT(( is_same< lbits::type, unsigned int>::type::value ));
    // BOOST_ASSERT(( lbits::value == 7 ));
    // using namespace std;
    
    return 0;
}
