
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[expensive_copy_boost_local_cpp
#include <boost/local/function.hpp>
#include <iostream>

struct x_t {
    int i;
    x_t(int _i): i(_i) {}
    x_t(x_t const& o): i(o.i) { // Some time consuming copy.
        for (unsigned long i = 0; i < 10000000000; ++i); 
    }
};

int main() {
    x_t x(-1);

    void BOOST_LOCAL_FUNCTION_PARAMS( (const bind& x) ) { // No copy...
        std::cout << x.i << std::endl; // ... and constant.
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();

    return 0;
}
//]

