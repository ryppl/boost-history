
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>

#include <algorithm>
#include <iostream>
#include <cassert>

int main() {
    double sum = 0.0;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS( (double num)
            (const bind factor) (bind& sum) ) {
        sum += factor * num;
        std::cout << "Summed: " << sum << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    add(100.0);

    size_t size = 2;
    double* nums = new double[size];
    BOOST_LOCAL_EXIT( (const bind& size) (bind nums) ) {
        if (size && nums) delete[] nums;
        std::cout << "Freed array: " << nums << std::endl;
    } BOOST_LOCAL_EXIT_END

    nums[0] = 90.5; nums[1] = 7.0;
    std::for_each(nums, nums + size, add); // `add` as template parameter

    BOOST_LOCAL_BLOCK( (const bind& sum) ) {
        assert(sum == 1975.0); // so far `sum` is 10*100+10*90.5+10*7=1975
        std::cout << "Asserted summation: " << sum << std::endl;
    } BOOST_LOCAL_BLOCK_END
    
    return 0;
}
//]

