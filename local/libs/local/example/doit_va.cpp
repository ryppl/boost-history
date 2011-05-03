
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

void doit1(boost::function< int (int) > l) { std::cout << l(1) << std::endl; }

void doit0(boost::function< int () > l) { std::cout << l() << std::endl; }

int main() {
    boost::function< int (int, int) > l2;

    {
        int i = 2;
    
        int BOOST_LOCAL_FUNCTION_PARAMS(int x, default 1, int y, default 2,
                bind i) {
            return x + i * y;
        } BOOST_LOCAL_FUNCTION_NAME(linear)

        // Assign local functions variables.
        l2 = linear;
        std::cout << l2(1, 2) << std::endl;

        // Similarly, pass local functions as a function parameter.
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

