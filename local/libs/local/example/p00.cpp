
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(1000000);
    std::fill(v.begin(), v.end(), 10);

    for(int i = 0; i < 10000; ++i) {
        std::for_each(v.begin(), v.end(), (
            boost::phoenix::ref(sum) += factor * boost::phoenix::arg_names::_1
        ));
    }

    std::cout << sum << std::endl;
    return 0;
}

