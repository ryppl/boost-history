
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ factorial_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

struct calculator {
    std::ostringstream output;

    void factorials(const std::vector<int>& nums,
            const std::string& separator = " ") {
        int BOOST_LOCAL_FUNCTION_PARAMS( (int n) (bool recursion)(default false)
                (const bind& separator) (bind this) ) {
            int result = 0;

            if (n < 2 ) result = 1;
            else result = n * factorial(n - 1, true); // Recursive call.

            if (!recursion) this_->output << result << separator;
            return result;
        } BOOST_LOCAL_FUNCTION_NAME(factorial)
    
        std::for_each(nums.begin(), nums.end(), factorial);
    }
};

int main() {
    std::vector<int> v(3);
    v[0] = 1; v[1] = 4; v[2] = 7;

    calculator calc;
    calc.factorials(v);
    std::cout << calc.output.str() << std::endl;

    return 0;
}
//]

