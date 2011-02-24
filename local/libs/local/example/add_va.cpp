
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use all local constructs -- main motivating example for the documentation.
// Adapted from C++0x lambda paper N2529 (added local blocks and exits).
// Simplified syntax for variadic macros only.
    
// NOTE: In this example `factor` is bound as const reference so to prevent
// the local function to modify it. Alternatively, it could have been bound by
// value as in `(bind)( (&sum) (factor) )` so to prevent modifications of
// `factor` from within the local function to affect `factor` values outside
// the local function scope (as done in N2529). However, binding `factor` by
// value instead that as const reference would have introduced an extra copy
// operation and, more importantly, it would have not enforced the logical
// constraint that `factor` should not be modified at all from within the local
// function body.

//[ add_va_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>

#include <algorithm>
#include <iostream>
#include <cassert>

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS(double num,
            const bind factor, bind& sum) {
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    add(100.0);

    size_t size = 2;
    double* nums = new double[size];
    BOOST_LOCAL_EXIT(const bind& size, bind nums) {
        if (size && nums) delete[] nums;
        std::clog << "Freed array: " << nums << std::endl;
    } BOOST_LOCAL_EXIT_END

    nums[0] = 90.5; nums[1] = 7.0;
    std::for_each(nums, nums + size, add); // `add` as template parameter

    BOOST_LOCAL_BLOCK(const bind& sum) {
        assert(sum == 1975.0); // so far `sum` is 10*100+10*90.5+10*7=1975
        std::clog << "Asserted summation: " << sum << std::endl;
    } BOOST_LOCAL_BLOCK_END
    
    return 0;
}
//]

