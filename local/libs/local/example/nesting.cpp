
// Local functions, blocks, and exits can be arbitrarily nested into each other.

#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <iostream>
#include <cassert>

int main() {
    int x = 0;

    // Local functions nested into one another.
    void BOOST_LOCAL_FUNCTION_PARAMS( (bind& x) ) {
        std::cout << "l: " << x << std::endl;

        void BOOST_LOCAL_FUNCTION_PARAMS( (bind& x) ) {
            std::cout << "m: " << x << std::endl;
        } BOOST_LOCAL_FUNCTION_NAME(m)

        x--; m();
    } BOOST_LOCAL_FUNCTION_NAME(l)
    
    // Local functions, blocks and exits nested into each other.
    void BOOST_LOCAL_FUNCTION_PARAMS( (bind& x) ) {
        BOOST_LOCAL_EXIT( (bind& x) ) {
            x = 0;
            BOOST_LOCAL_BLOCK( (const bind& x) ) {
                assert(x == 0);
            } BOOST_LOCAL_BLOCK_END
        } BOOST_LOCAL_EXIT_END
        std::cout << "n: " << x << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(n)

    x--; l();
    x--; n();
    return 0;
}

