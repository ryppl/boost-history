
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

// This code does NOT compile -- don't add it to the build.

//[noncopyable_0x_lambda_cpp
#include <boost/noncopyable.hpp>
#include <iostream>

struct x_t: boost::noncopyable {
    int i;
    x_t(int _i): i(_i) {}
};

int main() {
    x_t x(-1);

    auto l = [x]() { // Error: x is non-copyable...
        // ... but if bind `&x` then `x` is not constant.
        std::cout << x.i << std::endl;
    };
    l();

    return 0;
}
//]

