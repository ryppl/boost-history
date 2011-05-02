// Boost sweepline/sqr_expr_evaluator.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_SQRT_EXPR_EVALUATOR
#define BOOST_SWEEPLINE_SQRT_EXPR_EVALUATOR

namespace boost {
namespace sweepline {
namespace detail {

    template <int N>
    struct sqr_expr_evaluator {
        template <typename mpt, typename mpf>
        static mpf eval(mpt *A, mpt *B);
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]).
    template <>
    struct sqr_expr_evaluator<1> {
        template <typename mpt, typename mpf>
        static mpf eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpf a, b, ret_val;
            a = A[0];
            b = B[0];
            ret_val = a * sqrt(b);
            return ret_val;
        }
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) with
    // 7 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<2> {
        template <typename mpt, typename mpf>
        static mpf eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpf lhs, rhs, numerator;
            lhs = sqr_expr_evaluator<1>::eval<mpt, mpf>(A, B);
            rhs = sqr_expr_evaluator<1>::eval<mpt, mpf>(A + 1, B + 1);
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            numerator = A[0] * A[0] * B[0] - A[1] * A[1] * B[1];
            return numerator / (lhs - rhs);
        }
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2])
    // with 16 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<3> {
        template <typename mpt, typename mpf>
        static mpf eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpt cA[2], cB[2];
#ifndef THREAD_SAFETY
            static
#endif
            mpf lhs, rhs, numer;
            lhs = sqr_expr_evaluator<2>::eval<mpt, mpf>(A, B);
            rhs = sqr_expr_evaluator<1>::eval<mpt, mpf>(A + 2, B + 2);
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            cA[0] = A[0] * A[0] * B[0] + A[1] * A[1] * B[1];
            cA[0] -= A[2] * A[2] * B[2];
            cB[0] = 1;
            cA[1] = A[0] * A[1] * 2;
            cB[1] = B[0] * B[1];
            numer = sqr_expr_evaluator<2>::eval<mpt, mpf>(cA, cB);
            return numer / (lhs - rhs);
        }
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2]) + A[3] * sqrt(B[3])
    // with 25 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<4> {
        template <typename mpt, typename mpf>
        static mpf eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpt cA[3], cB[3];
#ifndef THREAD_SAFETY
            static
#endif
            mpf lhs, rhs, numer;
            lhs = sqr_expr_evaluator<2>::eval<mpt, mpf>(A, B);
            rhs = sqr_expr_evaluator<2>::eval<mpt, mpf>(A + 2, B + 2);
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            cA[0] = A[0] * A[0] * B[0] + A[1] * A[1] * B[1];
            cA[0] -= A[2] * A[2] * B[2] + A[3] * A[3] * B[3];
            cB[0] = 1;
            cA[1] = A[0] * A[1] * 2;
            cB[1] = B[0] * B[1];
            cA[2] = A[2] * A[3] * -2;
            cB[2] = B[2] * B[3];
            numer = sqr_expr_evaluator<3>::eval<mpt, mpf>(cA, cB);
            return numer / (lhs - rhs);
        }
    };

} // detail
} // sweepline
} // boost

#endif