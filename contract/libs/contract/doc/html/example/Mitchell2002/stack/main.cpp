
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "stack.hpp"
#include <string>
#include <iostream>

int main() {
    std::cout << std::endl << "constructor()" << std::endl;
    stack<std::string> s;

    std::cout << std::endl << "count()" << std::endl;
    std::cout << s.count() << std::endl;

    std::cout << std::endl << "put()" << std::endl;
    s.put("Galileo");

    std::cout << std::endl << "item_at()" << std::endl;
    std::cout << s.item_at(1) << std::endl;

    std::cout << std::endl << "remove()" << std::endl;
    s.remove();

    std::cout << std::endl << "is_empty()" << std::endl;
    std::cout << s.is_empty() << std::endl;

    std::cout << std::endl << "destructor()" << std::endl;
    return 0;
}

