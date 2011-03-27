
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#ifdef BOOST_NO_LAMBDAS
#include <iostream>
int main() {
    std::cerr << "Error: This program requires C++0x lambdas" << std::endl;
    return 0;
}
#else

//[ add_cpp0x_lambda_cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    // Passed as template parameter and also defined at expression level.
    std::for_each(v.begin(), v.end(), [&sum, &factor](double num) {
        // Unfortunately, cannot make `factor` constant.
        // Body uses normal C++ syntax.
        sum += factor * num;
        std::cout << "Summed: " << sum << std::endl;
    });

    std::cout << sum << std::endl;
    return 0;
}
//]

#endif

