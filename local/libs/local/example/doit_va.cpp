
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

//[ doit_va_cpp
#include <boost/local/function.hpp>
#include <boost/function.hpp>
#include <iostream>

// Using `function` allows to use the same functor for all calls (regardless of
// which set of default parameters is specified).
void doit(boost::local::function< int (int, int), 2 > l) {
    std::cout << l(1, 2) << std::endl;
    std::cout << l(1) << std::endl;
    std::cout << l() << std::endl;
}

// Using `function` requires different functors for calls with different set of
// default parameters.
void doit2(boost::function< int (int, int) > l)
    { std::cout << l(1, 2) << std::endl; }
void doit1(boost::function< int (int) > l) { std::cout << l(1) << std::endl; }
void doit0(boost::function< int () > l) { std::cout << l() << std::endl; }

int main() {
    boost::function<int (int, int)> l2;

    {
        int i = 2;
    
        int BOOST_LOCAL_FUNCTION_PARAMS(int x, default 1, int y, default 2,
                bind i) {
            return x + i * y;
        } BOOST_LOCAL_FUNCTION_NAME(linear)

        // Assign local functions variables.
        boost::local::function< int (int, int), 2 > l = linear;
        l(1, 2); l(1); l(); // All calls because of default parameters.
        l2 = linear;
        l2(1, 2); // Only one call operation (without default parameters).

        // Similarly, pass local functions as a function parameter.
        doit(linear);
        std::cout << std::endl;
        doit2(linear);
        doit1(linear);
        doit0(linear);
    }
        
    if (false) {
        l2(1, 2); // This wouuld raise run-time error because invalid reference.
    }

    return 0;
}
//]

#endif

