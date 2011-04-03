
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_local_class_cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    struct local {
        static void add(double num,
                // Unfortunately, cannot bind so repeat local variable types.
                double& sum, const int& factor) {
            // Body uses normal C++ syntax.
            sum += factor * num;
            std::cout << "Summed: " << sum << std::endl;
        }
    };

    // Unfortunately, cannot pass as template parameter to `std::for_each`.
    for (size_t i = 0; i < v.size(); ++i) {
        // Unfortunately, explicitly pass variables `sum` and `factor`.
        local::add(v[i], sum, factor);
    }

    std::cout << sum << std::endl;
    return 0;
}
//]

