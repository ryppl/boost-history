
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use recursion, default parameters, and bind by non-const reference.

//[ factorial_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

int main () {
    std::vector<int> v;
    v.resize(3);
    v[0] = 1; v[1] = 4; v[2] = 7;
    std::ostringstream factorials;

    BOOST_LOCAL_FUNCTION(
    (int) (factorial)(
            (int)(n)
            (bool)(recursion)(default)(false)
            (bind)((&factorials))
        )
    ) {
        int result = 0;

        if (n < 2 ) result = 1;
        else result = n * factorial(n - 1, true); // Recursive call.

        if (!recursion) factorials << result << " ";
        return result;
    } BOOST_LOCAL_FUNCTION_END(factorial)

    std::for_each(v.begin(), v.end(), factorial);
    std::cout << factorials.str() << std::endl;

    return 0;
}
//]

