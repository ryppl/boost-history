
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "string.hpp"
#include <iostream>

int main() {
    std::cout << std::endl << "constructor()" << std::endl;
    String s("Galileo");

    std::cout << std::endl << "operator[]" << std::endl;
    std::cout << s[0] << std::endl;

    std::cout << std::endl << "destructor()" << std::endl;
    return 0;
}

