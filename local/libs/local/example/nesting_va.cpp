
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[nesting_va_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <iostream>
#include <cassert>

int main() {
    int x = 0;

    // Local functions nested into one another.
    void BOOST_LOCAL_FUNCTION_PARAMS(bind& x) {
        std::cout << "l: " << x << std::endl;

        void BOOST_LOCAL_FUNCTION_PARAMS(bind& x) {
            std::cout << "m: " << x << std::endl;
        } BOOST_LOCAL_FUNCTION_NAME(m)

        x--; m();
    } BOOST_LOCAL_FUNCTION_NAME(l)
    
    // Local functions, blocks and exits nested into each other.
    void BOOST_LOCAL_FUNCTION_PARAMS(bind& x) {
        BOOST_LOCAL_EXIT(bind& x) {
            x = 0;
            BOOST_LOCAL_BLOCK(const bind& x) {
                assert(x == 0);
            } BOOST_LOCAL_BLOCK_END
        } BOOST_LOCAL_EXIT_END
        std::cout << "n: " << x << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(n)

    x--; l();
    x--; n();
    return 0;
}
//]

#endif

