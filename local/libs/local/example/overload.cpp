
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[overload_cpp
#include <boost/local/function/overload.hpp> // Include header for overloading.
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

int print_add(int x, int y) {
    std::cout << x << " + " << y << " = ";
    return x + y;
}

int main() {
    char end = '\n';

    void BOOST_LOCAL_FUNCTION_PARAMS( (const std::string& item)
            (const bind& end ) ) {
        std::cout << item << " (string)" << end;
    } BOOST_LOCAL_FUNCTION_NAME(print_string)

    void BOOST_LOCAL_FUNCTION_PARAMS( (const double& item)
            (const char* name)(default " (double)") (const bind& end) ) {
        std::cout << item << name << end;
    } BOOST_LOCAL_FUNCTION_NAME(print_double)
    
    boost::local::function::overload<
          void (const std::string&)
        , void (const double&) // Overload 1st param type.
        , void (const double&, const char*) // Overload giving default param.
        , int (int, int) // Overload giving 2 params (from function pointer).
    > print(print_string, print_double, print_double, print_add);

    std::vector<std::string> s(3);
    s[0] = "ab"; s[1] = "cd"; s[2] = "ef";
    std::for_each(s.begin(), s.end(), print); // Call `print_string` 1-param.
    
    std::vector<double> d(2);
    d[0] = 1.2; d[1] = 3.4;
    std::for_each(d.begin(), d.end(), print); // Call `print_double` 1-param.

    print(-1.234, " (real)"); // Call `print_double` 2-params.
    
    std::cout << print(2, 3) << std::endl; // Call `print_add`.

    return 0;
}
//]

