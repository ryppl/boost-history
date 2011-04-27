
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_using_local_functor_cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    struct local_add { // Unfortunately, boilerplate code to program the class.
        local_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}

        inline void operator()(double num) { // Body uses C++ statement syntax.
            sum += factor * num;
            std::cout << "Summed: " << sum << std::endl;
        }

    private:
        // Unfortunately, cannot bind so repeat variable types.
        double& sum; // Access `sum` by reference.
        const int& factor; // Make `factor` constant.
    } add(sum, factor);

    // Unfortunately, cannot pass as template parameter to `std::for_each`.
    for (size_t i = 0; i < v.size(); ++i) add(v[i]);

    std::cout << sum << std::endl;
    return 0;
}
//]

