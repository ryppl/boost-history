
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[typeof_va_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <boost/local/typeof.hpp> // Typeof header.
#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>
#include <algorithm>
#include <cassert>

double total(const double& x, const double& y, const double& z) {
    double sum = double();
    int factor = 10;

    double BOOST_LOCAL_FUNCTION_PARAMS(double num, const bind factor,
            bind& sum) {
        // Typeof for variable declaration.
        BOOST_LOCAL_TYPEOF(factor) f = factor;
        return sum += f * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)
    add(x);

    size_t size = 2;
    double* nums = new double[size];
    BOOST_LOCAL_EXIT(const bind& size, bind nums) {
        // Typeof is qualified with eventual bind's `const` and `&`.
        boost::remove_const<boost::remove_reference<
                BOOST_LOCAL_TYPEOF(size)>::type>::type s;
        s = size;
        if (s && nums) delete[] nums;
    } BOOST_LOCAL_EXIT_END

    nums[0] = y; nums[1] = z;
    std::for_each(nums, nums + size, add);

    BOOST_LOCAL_BLOCK(const bind &sum, const bind& factor,
            const bind& x, const bind& y, const bind& z) {
        // Typeof for concept checking.
        BOOST_CONCEPT_ASSERT((boost::EqualityComparable<
                BOOST_LOCAL_TYPEOF(sum)>));
        assert(sum == factor * (x + y + z));
    } BOOST_LOCAL_BLOCK_END

    return sum;
}

int main() {
    total(100.0, 90.5, 7.0);
    return 0;
}
//]

#endif

