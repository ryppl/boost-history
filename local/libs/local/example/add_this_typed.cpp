
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_this_typed_cpp
#include <boost/local/function.hpp>
#include <boost/local/block.hpp>
#include <boost/local/exit.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>

class adder {
public:
    adder(): sum_(0.0) {}

    double sum(const std::vector<double>& nums, const int& factor = 10) {
        std::ostringstream out;

        void BOOST_LOCAL_FUNCTION_PARAMS( (double num)
                (const bind(const int&) factor) // Specify bind types
                (bind(std::ostringstream)& out) // explicitly (so no
                (bind(adder*) this) ) {         // Boost.Typeof for binds).
            this_->sum_ += factor * num;
            out << "Summed: " << this_->sum_ << std::endl;
        } BOOST_LOCAL_FUNCTION_NAME(add)
        std::for_each(nums.begin(), nums.end(), add);

        BOOST_LOCAL_EXIT( (const bind(std::ostringstream)& out) ) {
            std::cout << out.str();
        } BOOST_LOCAL_EXIT_END

        BOOST_LOCAL_BLOCK( (bind(std::ostringstream)& out)
                (const bind(adder*) this) ) {
            out << "Asserted: " << this_->sum_ << std::endl;
            assert(this_->sum_ > 0.0);
        } BOOST_LOCAL_BLOCK_END

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

