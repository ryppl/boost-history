
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_exit_cpp
#include <boost/local/exit.hpp>
#include <iostream>
#include <cassert>

int main() {
    size_t size = 2;
    double* nums = new double[size];
    
    BOOST_LOCAL_EXIT( (const bind& size) (bind nums) ) { // Local exit.
        if (size && nums) delete[] nums;
        std::cout << "Freed array: " << nums << std::endl;

        return; // Return this local exit (and not the enclosing scope).
        assert(false); // OK: Never executed.
    } BOOST_LOCAL_EXIT_END

    return 0;
} // Local exit executed here.
//]

