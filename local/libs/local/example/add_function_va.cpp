
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

//[ add_function_va_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS(double num, const bind factor,
            bind& sum) {
        sum += factor * num;
        std::cout << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    add(100.0);
    return 0;
}
//]

#endif

