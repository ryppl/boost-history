
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

#define BOOST_LOCAL_TYPEOF(bind_name) \
    boost::remove_const<boost::remove_reference< \
            BOOST_PP_CAT(bind_name, Xtype)>::type>::type

int main() {
    double sum = 0.0;
    //pod sum;
    int factor = 10;

    BOOST_LOCAL_FUNCTION(
    (void) (add)( (double)(num) (const bind)((factor)) (bind)((&sum)) )
    ) {
        typedef boost_se_params_t_add35::boost_se_param_t_1_add35* &sum_type;
        BOOST_CONCEPT_ASSERT((Addable<
                boost::remove_reference<sum_type>::type>));

        sum = sum + factor * num;
    } BOOST_LOCAL_FUNCTION_END(add)

    add(100.0);

    return 0;
}
//]

