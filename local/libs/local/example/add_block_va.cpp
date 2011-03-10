
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_block_va_cpp
#include <boost/local/block.hpp>
#include <iostream>
#include <cassert>

int main() {
    double sum = 1975.0;

    BOOST_LOCAL_BLOCK(const bind& sum) {
        assert(sum == 1975.0); // OK: Complier error if `==` confused with `=`.
        std::clog << "Asserted summation: " << sum << std::endl;

        return; // Return this local block (and not the enclosing scope).
        assert(false); // OK: Never executed.
    } BOOST_LOCAL_BLOCK_END

    return 0;
}
//]

