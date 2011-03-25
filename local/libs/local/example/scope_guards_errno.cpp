
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[scope_guards_errno_cpp
#include <boost/local/exit.hpp>
#include <iostream>

int f() {
    int error = 0; // No error to start with.

    BOOST_LOCAL_EXIT( (void) ) { // Same as D's `scope(exit)`.
        std::cout << "exit" << std::endl;
    } BOOST_LOCAL_EXIT_END

    BOOST_LOCAL_EXIT( (const bind& error) ) { // Sane as D's `scope(success)`.
        if (!error) std::cout << "success" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    BOOST_LOCAL_EXIT( (const bind& error) ) { // Same as D's `scope(failure)`.
        if (error) std::cout << "failure" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    return error = -1; // Must set error.
}

int main() {
    return f();
}
//]

