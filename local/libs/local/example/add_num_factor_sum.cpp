
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Simple parenthesized syntax example used by the documentation.

//[ add_num_factor_sum_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    double sum = 0.0;
    int factor = 10;

    BOOST_LOCAL_FUNCTION(
    (void) (add)( (double)(num) (const bind)((factor)) (bind)((&sum)) )
    ) {
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_END(add)

    add(100.0);

    return 0;
}
//]

