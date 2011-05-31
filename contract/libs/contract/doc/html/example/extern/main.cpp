
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

//[ extern_cpp

extern void ext(int x) // Some external linkage.
CONTRACT_FUNCTION( (extern) (void) (ext)( (int)(x) )
(body) ({
    std::cout << x << std::endl;
}) )

extern "C" void ext_c(int x) // External linkage callable from C.
CONTRACT_FUNCTION( (extern "C") (void) (ext_c)( (int)(x) )
(body) ({
    std::cout << x << std::endl;
}) )

//]

int main() {
    ext(1);
    ext_c(2);
    return 0;
}

