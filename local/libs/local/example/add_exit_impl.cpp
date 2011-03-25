
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_exit_impl_cpp
#include <boost/local/exit.hpp>
#include <iostream>
#include <cassert>

int main() {
    size_t size = 2;
    double* nums = new double[size];
    
    // The macro `BOOST_LOCAL_EXIT( (const bind& size) (bind nums) )` expands to:
    void BOOST_LOCAL_FUNCTION_PARAMS( (const bind& size) (bind nums) )
    
    // Local exit body.
    {
        if (size && nums) delete[] nums;
        std::cout << "Freed array: " << nums << std::endl;

        return; // Return this local exit (and not the enclosing scope).
        assert(false); // OK: Never executed.
    } 
    
    // The macro `BOOST_LOCAL_EXIT_END` expands to (at line 22):
    BOOST_LOCAL_FUNCTION_NAME(boost_local_auxXexitXfunction22)
    // When this object goes out of scope, its destructor calls the specified
    // local function which in turn executes the local exit body code.
    ::boost::local::aux::exit_guard boost_local_auxXexitXguard22(
            boost_local_auxXexitXfunction22);

    return 0;
}
//]

