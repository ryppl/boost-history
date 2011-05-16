
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

//[noncopyable_boost_local_va_cpp
#include <boost/local/function.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

struct x_t: boost::noncopyable {
    int i;
    x_t(int _i): i(_i) {}
};

int main() {
    x_t x(-1);

    void BOOST_LOCAL_FUNCTION_PARAMS(const bind& x) { // OK...
        std::cout << x.i << std::endl; // ... and constant.
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();

    return 0;
}
//]

#endif

