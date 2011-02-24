
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Show how to use some optimizations (auto, register, etc).

//[add_optimizers_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    int BOOST_LOCAL_FUNCTION_PARAMS( (auto int x) (register int y) ) {
        return x + y;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    std::cout << add(3, 5) << std::endl;

    return 0;
}
//]

