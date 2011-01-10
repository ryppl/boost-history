
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Show who to rename a local function and pass it as template parameter.

//[ renamed_print_cpp
#include <boost/local/function.hpp>
#include <boost/local/function_ref.hpp>
#include <algorithm>
#include <iostream>

int main() {
    BOOST_LOCAL_FUNCTION(
    (void) (print)( (int)(x) )
    ) {
        // Only original `print` name available within local function body.
        std::cout << x << std::endl;
    } BOOST_LOCAL_FUNCTION_END_RENAME(new_print)
    // Only new `new_print` name available within enclosing scope.

    int a[3] = {1, 2, 3};

    // std::for_each(a, a + 3, new_print); // Error: Passing renamed function.

    boost::local::function_ref< void (int) > print_ref = new_print;
    std::for_each(a, a + 3, print_ref); // OK: Passing function reference.

    return 0;
}
//]

