
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

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

    void BOOST_LOCAL_FUNCTION_PARAMS(double num,
            const bind factor, bind& sum)
            throw (std::runtime_error, std::logic_error) { // Throw two except.
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    add(100.0);

    size_t size = 2;
    double* nums = new double[size];
    BOOST_LOCAL_EXIT(const bind& size, bind nums) throw() { // Throw none.
        if (size && nums) delete[] nums;
    } BOOST_LOCAL_EXIT_END

    nums[0] = 90.5; nums[1] = 7.0;
    std::for_each(nums, nums + size, add);

    BOOST_LOCAL_BLOCK(const bind& sum) throw(std::exception) { // Thow one.
        assert(sum == 1975.0);
    } BOOST_LOCAL_BLOCK_END

    return 0;
}
//]

