
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

int main() {
    for (int i = 0; i < 100; ++i) {
        // Invariant will intentionally fail.
        CONTRACT_ASSERT_BLOCK_INVARIANT( i < 10 );
        
        std::cout << i << std::endl;
    }

    return 0;
}

