
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Show how to use some optimizations (inlining, auto, register, etc).

//[add_optimizers_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    BOOST_LOCAL_FUNCTION(
    (inline) (int) (add)(           // Inlined local function.
            (auto)(int)(x)          // Auto parameter.
            (register)(int)(y)      // Register parameter.
        )
    ) {
        return x + y;
    } BOOST_LOCAL_FUNCTION_END(add)

    std::cout << add(3, 5) << std::endl;

    return 0;
}
//]

