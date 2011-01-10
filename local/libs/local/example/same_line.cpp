
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Show that local functions can be defined on the same line of code.

#include <boost/local/function.hpp>
#include <iostream>

int main() {
    BOOST_LOCAL_FUNCTION( (void) (l)( (int)(x) ) ) { std::cout << x << std::endl; } BOOST_LOCAL_FUNCTION_END(l) BOOST_LOCAL_FUNCTION( (void) (m)( (int)(x) ) ) { std::cout << x << std::endl; } BOOST_LOCAL_FUNCTION_END(m)
    
    l(1);
    m(2);

    return 0;
}

