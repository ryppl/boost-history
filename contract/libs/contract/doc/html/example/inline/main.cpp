
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

//[ inline_cpp

class z {

    CONTRACT_CLASS( (z) )

public:
    inline void inl(int x)
    CONTRACT_FUNCTION(
            (public) (inline) (void) (inl)( (int)(x) )
    (body) ({
        std::cout << x << std::endl;
    }) )
};

inline void inl(int x)
CONTRACT_FUNCTION(
        (inline) (void) (inl)( (int)(x) )
(body) ({
    std::cout << x << std::endl;
}) )

//]

int main() {
    z zz;
    zz.inl(1);
    inl(2);
    return 0;
}

