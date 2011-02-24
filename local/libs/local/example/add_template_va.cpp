
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Use local functions, blocks, and exits from template scope.
// Simplified syntax for variadic macros only.

//[add_template_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <algorithm>
#include <cassert>

template<typename T>
T total(const T& x, const T& y, const T& z) {
    T sum = T();
    int factor = 10;

    // Use `..._TPL()` macros within templates.
    void BOOST_LOCAL_FUNCTION_PARAMS_TPL(T num, const bind factor,
            bind& sum) {
        sum += factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    add(x);

    size_t size = 2;
    T* nums = new T[size];
    BOOST_LOCAL_EXIT_TPL(const bind& size, bind nums) {
        if (size && nums) delete[] nums;
    } BOOST_LOCAL_EXIT_END

    nums[0] = y; nums[1] = z;
    std::for_each(nums, nums + size, add);

    BOOST_LOCAL_BLOCK_TPL(const bind &sum, const bind& factor,
            const bind& x, const bind& y, const bind& z) {
        assert(sum == factor * (x + y + z));
    } BOOST_LOCAL_BLOCK_END

    return sum;
}

int main() {
    total(100.0, 90.5, 7.0);
    return 0;
}
//]

