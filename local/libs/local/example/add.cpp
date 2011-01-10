
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use all local constructs -- main motivating example for the documentation.
// Adapted from C++0x lambda paper N2529 (added local blocks and exits).
    
// NOTE: In this example `factor` is bound as const reference so to prevent
// the local function to modify it. Alternatively, it could have been bound by
// value as in `(bind)( (&sum) (factor) )` so to prevent modifications of
// `factor` from within the local function to affect `factor` values outside
// the local function scope (as done in N2529). However, binding `factor` by
// value instead that as const reference would have introduced an extra copy
// operation and, more importantly, it would have not enforced the logical
// constraint that `factor` should not be modified at all from within the local
// function body.

//[ add_cpp
/*< Include the header files for this library local functions, local blocks, and local exits. >*/
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <algorithm>
#include <iostream>
#include <cassert>

int main() {
    double sum = 0.0;
    int factor = 10;

    BOOST_LOCAL_FUNCTION( /*< The *local function* declaration macro. This macro takes one parameter specifying the local function signature using the /parenthesized syntax/. This syntax resembles the usual C++ function declaration syntax but it wraps all tokens within parenthesis (see the __Tutorial__ section). The parenthesized syntax for local functions introduces the new "keyword" `bind` which is used in place of a parameter type to indicate that the following sequence of parameters bind to variables in scope. >*/
    (void) (add)( (double)(num) (const bind)((factor)) /*< The variable `factor` is bound by constant value so it cannot be mistakenly modified by the local function body. >*/ (bind)((&sum)) /*< The variable `sum` is instead bound by non-constant reference because the local function needs to change its value to report the summation result to the enclosing scope. >*/ )
    ) { /*< The local function body is programmed using the usual C++ syntax. >*/
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_END(add) /*< The macro ending the local function definition (note how the local function name needs to be repeated here). >*/
    add(100.0); /*< The local function macros declare a functor object local to the enclosing scope named `add`. As indicated by the local function parenthesized signature, the functor `add` has `void` result type, it can be called by specifying one parameter of type `double`, and it has access to the variables in scope `factor` and `sum`. >*/

    size_t size = 2;
    double* nums = new double[size];
    BOOST_LOCAL_EXIT( (const bind)((&size)) (bind)((nums)) ) { /*< The *local exit* declaration macro only specifies the variables in scope to bind (eventually as constants). >*/
        if (size && nums) delete[] nums; /*< The local exit code will be automatically executed when the enclosing scope is exited. >*/
        std::clog << "Freed array: " << nums << std::endl;
    } BOOST_LOCAL_EXIT_END /*< A local exit is anonymous so its ending macro takes no argument. >*/

    nums[0] = 90.5; nums[1] = 7.0;
    std::for_each(nums, nums + size, add); /*< The local function `add` can also be passed as a template parameter to the STL `std::for_each` algorithm. >*/

    BOOST_LOCAL_BLOCK( (const bind)((&sum)) ) { /*< The *local block* declaration macro only specifies the variables in scope to bind (eventually as constants). >*/ 
        // So far: sum = 10 * 100.0 + 10 * 90.5 + 10 * 7.0 = 1975.0
        assert(sum == 1975.0); /*< The local block code is executed "in place" where it is programmed like a usual C++ block of code `{ ... }`. In this example, the compiler will correctly generate an error if the assignment operator `=` is mistakenly used instead of the equality operator `==` because the variable `sum` is bound by constant reference so it cannot be modified. >*/
        std::clog << "Asserted summation: " << sum << std::endl;
    } BOOST_LOCAL_BLOCK_END /*< A local block is anonymous so its ending macro takes no argument. >*/
    
    return 0;
}
//]

