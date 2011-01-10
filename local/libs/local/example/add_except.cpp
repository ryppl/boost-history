
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use local functions, blocks, and exits from template scope.

//[add_except_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <stdexcept>
#include <algorithm>
#include <cassert>

int main() {
    double sum = 0.0;
    int factor = 10;

    BOOST_LOCAL_FUNCTION(
    (void) (add)( (double)(num) (const bind)((factor)) (bind)((&sum)) )
    ) throw (std::runtime_error, std::logic_error) { // Throw two exceptions.
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_END(add)
    add(100.0);

    size_t size = 2;
    double* nums = new double[size];
    // Throw nothing.
    BOOST_LOCAL_EXIT( (const bind)((&size)) (bind)((nums)) ) throw() {
        if (size && nums) delete[] nums;
    } BOOST_LOCAL_EXIT_END

    nums[0] = 90.5; nums[1] = 7.0;
    std::for_each(nums, nums + size, add);

    // Throw one exception.
    BOOST_LOCAL_BLOCK( (const bind)((&sum)) ) throw(std::exception) {
        assert(sum == 1975.0);
    } BOOST_LOCAL_BLOCK_END

    return 0;
}
//]

