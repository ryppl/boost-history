
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_using_global_functor_cpp
#include <iostream>
#include <vector>
#include <algorithm>

// Unfortunately, cannot be defined locally (so this is not an alternative
// implementation of local functions).
struct global_add { // Unfortunately, boilerplate code to program the class.
    global_add(double& _sum, int _factor): sum(_sum), factor(_factor) {}

    inline void operator()(double num) {
        // Body uses C++ statement syntax.
        sum += factor * num;
        std::cout << "Summed: " << sum << std::endl;
    }

private:
    // Unfortunately, cannot bind so repeat variable types.
    double& sum; // Access `sum` by reference.
    const int& factor; // Make `factor` constant.
};

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(3);
    v[0] = 1.0; v[1] = 2.0; v[2] = 3.0;

    global_add add(sum, factor);
    std::for_each(v.begin(), v.end(), add); // Passed as template parameter.

    std::cout << sum << std::endl;
    return 0;
}
//]

