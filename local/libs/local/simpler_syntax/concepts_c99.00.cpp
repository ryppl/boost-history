
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

// Simple parenthesized syntax example used by the documentation.

//[ add_num_factor_sum_cpp
#include <boost/local/function.hpp>
#include <boost/concept_check.hpp>
#include <iostream>

template<typename T>
class Addable {
    T x;
    T y;
    void return_type(T); // Used to check addition returns type `T`.
public:
    BOOST_CONCEPT_USAGE(Addable) {
        return_type(x + y); // Check addition `T operator+(T x, T y)`.
    }
};

struct pod {};

int main() {
    pod sum;
    int factor = 10;

    void BOOST_LOCAL_FUNCTION_PARAMS(double num, const bind factor, bind& sum) {
        BOOST_CONCEPT_ASSERT((Addable<boost::remove_reference<sum_type>::type>));

        sum = sum + factor * num;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    add(100.0);

    return 0;
}
//]

