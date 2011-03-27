
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_boost_local_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    // Unfortunately, cannot be defined at expression level.
    void BOOST_LOCAL_FUNCTION_PARAMS( (double num)
            // Variable `factor` bound as constant.
            (bind& sum) (const bind& factor) ) {
        // Body uses normal C++ syntax.
        sum += factor * num;
        std::cout << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    // Local function `add` passed as template parameter.
    std::for_each(v.begin(), v.end(), add);

    std::cout << sum << std::endl;
    return 0;
}
//]

#endif

