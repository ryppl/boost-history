
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#ifdef BOOST_NO_LAMBDAS
#include <iostream>
int main() {
    std::cerr << "Error: This program requires C++0x lambdas" << std::endl;
    return 0;
}
#else

//[expensive_copy_0x_lambda_cpp
#include <boost/noncopyable.hpp>
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

    auto l = [x]() { // Expensive copy...
        // ... but if bind `&x` then `x` is not constant.
        std::cout << x.i << std::endl;
    };
    l();

    return 0;
}
//]

#endif

