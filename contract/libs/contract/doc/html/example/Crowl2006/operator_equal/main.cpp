
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "equal.hpp"
#include "not_equal.hpp"
#include <iostream>

struct number { double value; };

int main() {
    number n;
    n.value = 1.23;

    std::cout << (n == n) << std::endl;
    std::cout << (n != n) << std::endl;

    return 0;
}

