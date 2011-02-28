
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/local/function.hpp>

int main() {
    // No params, no const binds, no plain binds (empty parenthesis).
    void BOOST_LOCAL_FUNCTION_PARAMS() {
    } BOOST_LOCAL_FUNCTION_NAME(l)
    l();

    return 0;
}

