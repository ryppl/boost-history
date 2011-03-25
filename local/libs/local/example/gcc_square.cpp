
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[gcc_square_cpp
#include <boost/local/function.hpp>
#include <iostream>

double add_square(double a, double b) {
    double BOOST_LOCAL_FUNCTION_PARAMS( (double z) ) {
        return z * z;
    } BOOST_LOCAL_FUNCTION_NAME(square)

    return square(a) + square(b);
}

int main() {
    std::cout << add_square(2.0, 3.0) << std::endl;
    return 0;
}
//]

