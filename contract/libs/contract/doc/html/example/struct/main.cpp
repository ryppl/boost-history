
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

//[ struct_cpp

struct positive {
    const double& number;

private: // Alway contract class in a private section at top.
    CONTRACT_CLASS( (positive)
    (invariant) ({
        CONTRACT_ASSERT( number > 0.0 );
    }) )

public:
    // Remember to always specify the access level (public, etc).
    positive(const double& n): number(n)
    CONTRACT_CONSTRUCTOR(
            (public) (positive)( (const double&)(n) )
    (precondition) ({
        CONTRACT_ASSERT( n > 0.0 );
    })
    (body) ({
    }) )
};

//]

int main() {
    positive p(1.23);
    std::cout << p.number << std::endl;
    return 0;
}

