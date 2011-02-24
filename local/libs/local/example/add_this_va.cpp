
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Simple parenthesized syntax example used by the documentation.
// Simplified syntax for variadic macros only.

//[ add_this_va_cpp
#include <boost/local/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

class adder {
public:
    adder(): sum_(0.0) {}

    double sum(const std::vector<double>& nums, const int& factor = 10) {
        void BOOST_LOCAL_FUNCTION_PARAMS(double num, const bind factor,
                bind this) {
            this_->sum_ += factor * num; // Use `this_` instead of `this`.
            std::clog << "Summed: " << this_->sum_ << std::endl;
        } BOOST_LOCAL_FUNCTION_NAME(add)

        std::for_each(nums.begin(), nums.end(), add);
        return sum_;
    }

private:
    double sum_;
};

int main() {
    std::vector<double> v(3);
    v[0] = 100.0; v[1] = 90.5; v[2] = 7.0;

    adder a;
    // sum = 10 * 100.0 + 10 * 90.5 + 10 * 7.0 = 1975.0
    assert(a.sum(v) == 1975.0);

    return 0;
}
//]

