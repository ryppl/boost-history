
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

//[ add_function_inline_va_cpp
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS(double num, const bind factor,
            bind& sum) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(inline add) // inlined local function

    std::vector<double> v(1000000);
    std::fill(v.begin(), v.end(), 1.0);

    // On some ISO C++ compilers (e.g., GCC 4.3) inlined local functions are
    // more likely to be optimized for faster run-times. However, inlined local
    // functions cannot be passed at template parameters (i.e., `std::for_each`
    // cannot be used here).
    // On C++0x compilers (e.g., MSVC 8.0 or GCC 4.5) linining has no effect
    // because the local function can always be optimized even if not
    // explicitly specified inline and inlined local functions can be always be
    // passed as template parameters.
    for (size_t i = 0; i < v.size(); ++i) { // Can't use for_each (portably).
        add(v[i]);
    }

    std::cout << sum << std::endl;
    return 0;
}
//]

#endif

