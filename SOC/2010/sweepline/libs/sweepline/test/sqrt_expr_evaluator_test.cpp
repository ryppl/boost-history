// Boost sweepline library mpz_arithmetic_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#pragma warning(disable:4800)
#include <gmpxx.h>
#include "test_type_list.hpp"

#include "boost/sweepline/detail/mpz_arithmetic.hpp"
using namespace boost::sweepline::detail;

#define BOOST_TEST_MODULE mpz_arithmetic_test
#include <boost/test/test_case_template.hpp>

template <int N>
struct test_sqr_evaluator {
    template <typename mpt>
    static bool run() {
        bool ret_val = true;
        static mpt A[N], B[N];
        double a[N], b[N];
        for (int i = 0; i < N; ++i) {
            a[i] = rand() % 1000 - 500;
            int rand_number = rand() % 1000 - 500;
            b[i] = rand_number * rand_number;
        }
        int mask = (1 << N);
        for (int i = 0; i < mask; i++) {
            double expected_val = 0.0;
            for (int j = 0; j < N; j++) {
                if (i & (1 << j)) {
                    A[j] = a[j];
                    B[j] = b[j];
                    expected_val += a[j] * sqrt(b[j]);
                } else {
                    A[j] = -a[j];
                    B[j] = b[j];
                    expected_val -= a[j] * sqrt(b[j]);
                }
            }
            double received_val = sqr_expr_evaluator<N>::eval(A, B);
            // TODO(asydorchuk): Change to almost equal check.
            ret_val &= (fabs(expected_val - received_val) <= 1E-9);
        }
        return ret_val;
    }
};

BOOST_AUTO_TEST_CASE(mpz_sqr_evaluator_test) {
    srand(static_cast<unsigned int>(time(0)));
    typedef mpt_wrapper<mpz_class, 4> mpt_wrapper_type;
    for (int i = 0; i < 1000; i++) {
        BOOST_CHECK(test_sqr_evaluator<2>::run<mpz_class>());
        BOOST_CHECK(test_sqr_evaluator<3>::run<mpz_class>());
        BOOST_CHECK(test_sqr_evaluator<4>::run<mpz_class>());
    }
    for (int i = 0; i < 1000; i++) {
        BOOST_CHECK(test_sqr_evaluator<2>::run<mpt_wrapper_type>());
        BOOST_CHECK(test_sqr_evaluator<3>::run<mpt_wrapper_type>());
        BOOST_CHECK(test_sqr_evaluator<4>::run<mpt_wrapper_type>());
    }	
}

