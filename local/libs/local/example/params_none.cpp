
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use all different syntaxes for no parameter.

#include <boost/local/function.hpp>

int main() {
    // No params, no const binds, no plain binds (token).
    void BOOST_LOCAL_FUNCTION_PARAMS(void) {
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();
    
    // No params, no const binds, no plain binds (sequence).
    void BOOST_LOCAL_FUNCTION_PARAMS( (void) ) {
    } BOOST_LOCAL_FUNCTION_NAME(m)
    m();

    return 0;
}

