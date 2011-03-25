
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[transform_va_cpp
#include <boost/local/function.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

int main () {
    int offset = 5;
    std::vector<int> v;
    std::vector<int> w;

    // v = 10 20 30 40 50
    for (int i = 1; i < 6; i++) v.push_back(i * 10);
    w.resize(v.size());

    // w = ++v + 5 = 16 26 36 46 56
    int BOOST_LOCAL_FUNCTION_PARAMS(int i, const bind& offset) {
        // Compiler error if const `offset` modified here by mistake.
        return ++i + offset;
    } BOOST_LOCAL_FUNCTION_NAME(inc)
    std::transform(v.begin(), v.end(), w.begin(), inc);

    offset = 0;

    // v = ++(v + w) + 0 = 27 47 67 87 107
    int BOOST_LOCAL_FUNCTION_PARAMS(int i, int j,
            const bind& inc) { // Bind another local function.
        return inc(i + j); // Call the bound local function.
    } BOOST_LOCAL_FUNCTION_NAME(inc_sum)
    std::transform(v.begin(), v.end(), w.begin(), v.begin(), inc_sum);

    for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i)
        { std::cout << " " << *i; }
    std::cout << std::endl;
    
    return 0;
}
//]

#endif

