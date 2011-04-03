// Boost sweepline/mpz_arithmetic.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_MPZ_ARITHMETIC
#define BOOST_SWEEPLINE_MPZ_ARITHMETIC

#include <cmath>
#include <string>

namespace boost {
namespace sweepline {
namespace detail {

    // Allows to compute expression without allocation of additional memory.
    // Expression evaluation should contain less then N temporary variables
    // (e.g. (a1 * b1) + (a2 * b2) - requires two temporary variables to hold
    // a1 * b1 and a2 * b2). This class is not thread safe, use mpz_class or
    // mpq_class instead (however there'll be at least 2 times slowdown).
    template <typename mpt, int N>
    class mpt_wrapper {
    public:
        mpt_wrapper() {}

        explicit mpt_wrapper(int input) : m_(input) {}
        
        explicit mpt_wrapper(double input) : m_(input) {}
        
        mpt_wrapper(const mpt& input) : m_(input) {}

        mpt_wrapper(const mpt_wrapper& w) : m_(w.m_) {
            cur_ = 0;
        }

        mpt_wrapper& operator=(int that) {
            m_ = that;
            return *this;
        }

        mpt_wrapper& operator=(double that) {
            m_ = that;
            return *this;
        }
        
        mpt_wrapper& operator=(const mpt_wrapper& that) {
            m_ = that.m_;
            return *this;
        }

        double get_d() const {
            return m_.get_d();
        }

        std::string get_str() const {
            return m_.get_str();
        }

        bool operator==(const mpt_wrapper& that) const {
            return m_ == that.m_;
        }

        bool operator!=(const mpt_wrapper& that) const {
            return m_ != that.m_;
        }

        bool operator<(const mpt_wrapper& that) const {
            return m_ < that.m_;
        }

        bool operator<=(const mpt_wrapper& that) const {
            return m_ <= that.m_;
        }

        bool operator>(const mpt_wrapper& that) const {
            return m_ > that.m_;
        }

        bool operator>=(const mpt_wrapper& that) const {
            return m_ >= that.m_;
        }

        bool operator==(int that) const {
            temp_[cur_] = that;
            return m_ == temp_[cur_].m_;
        }

        bool operator!=(int that) const {
            temp_[cur_] = that;
            return m_ != temp_[cur_].m_;
        }

        bool operator<(int that) const {
            temp_[cur_] = that;
            return m_ < temp_[cur_].m_;
        }

        bool operator<=(int that) const {
            temp_[cur_] = that;
            return m_ <= temp_[cur_].m_;
        }

        bool operator>(int that) const {
            temp_[cur_] = that;
            return m_ > temp_[cur_].m_;
        }

        bool operator>=(int that) const {
            temp_[cur_] = that;
            return m_ >= temp_[cur_].m_;
        }

        mpt_wrapper& operator-() const {
            temp_[cur_].m_ = -this->m_;
            return temp_[next_cur()];
        }

        mpt_wrapper& operator+(const mpt_wrapper& that) const {
            temp_[cur_].m_ = this->m_ + that.m_;
            return temp_[next_cur()];
        }

        mpt_wrapper& operator-(const mpt_wrapper& that) const {
            temp_[cur_].m_ = this->m_ - that.m_;
            return temp_[next_cur()];
        }

        mpt_wrapper& operator*(const mpt_wrapper& that) const {
            temp_[cur_].m_ = this->m_ * that.m_;
            return temp_[next_cur()];
        }

        mpt_wrapper& operator*(double that) const {
            temp_[cur_].m_ = that;
            temp_[cur_].m_ *= this->m_;
            return temp_[next_cur()];
        }

        mpt_wrapper& operator+=(const mpt_wrapper& that) {
            this->m_ += that.m_;
            return *this;
        }

        mpt_wrapper& operator-=(const mpt_wrapper& that) {
            this->m_ -= that.m_;
            return *this;
        }

        mpt_wrapper& operator*=(const mpt_wrapper& that) {
            this->m_ *= that.m_;
            return *this;
        }

    private:
        static int next_cur() {
            int ret_val = cur_++;
            if (cur_ == N)
                cur_ = 0;
            return ret_val;
        }

        mpt m_;
        static int cur_;
        static mpt_wrapper temp_[N];
    };

    template <typename mpt, int N>
    int mpt_wrapper<mpt, N>::cur_ = 0;
    
    template <typename mpt, int N>
    mpt_wrapper<mpt, N> mpt_wrapper<mpt, N>::temp_[N];
    
    template <int N>
    struct sqr_expr_evaluator {
        template <typename mpt>
        static double eval(mpt *A, mpt *B);
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) with
    // 7 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<2> {
        template <typename mpt>
        static double eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpt numerator;
            double lhs = A[0].get_d() * sqrt(B[0].get_d());
            double rhs = A[1].get_d() * sqrt(B[1].get_d());
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            numerator = A[0] * A[0] * B[0] - A[1] * A[1] * B[1];
            return numerator.get_d() / (lhs - rhs);
        }
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2])
    // with 16 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<3> {
        template <typename mpt>
        static double eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpt cA[2], cB[2];
            double lhs = sqr_expr_evaluator<2>::eval<mpt>(A, B);
            double rhs = A[2].get_d() * sqrt(B[2].get_d());
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            cA[0] = A[0] * A[0] * B[0] + A[1] * A[1] * B[1];
            cA[0] -= A[2] * A[2] * B[2];
            cB[0] = 1;
            cA[1] = A[0] * A[1] * 2;
            cB[1] = B[0] * B[1];
            return sqr_expr_evaluator<2>::eval<mpt>(cA, cB) / (lhs - rhs);
        }
    };

    // Evaluates expression:
    // A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2]) + A[3] * sqrt(B[3])
    // with 25 * EPS relative error in the worst case.
    template <>
    struct sqr_expr_evaluator<4> {
        template <typename mpt>
        static double eval(mpt *A, mpt *B) {
#ifndef THREAD_SAFETY
            static
#endif
            mpt cA[3], cB[3];
            double lhs = sqr_expr_evaluator<2>::eval<mpt>(A, B);
            double rhs = sqr_expr_evaluator<2>::eval<mpt>(A + 2, B + 2);
            if ((lhs >= 0 && rhs >= 0) || (lhs <= 0 && rhs <= 0))
                return lhs + rhs;
            cA[0] = A[0] * A[0] * B[0] + A[1] * A[1] * B[1];
            cA[0] -= A[2] * A[2] * B[2] + A[3] * A[3] * B[3];
            cB[0] = 1;
            cA[1] = A[0] * A[1] * 2;
            cB[1] = B[0] * B[1];
            cA[2] = A[2] * A[3] * -2;
            cB[2] = B[2] * B[3];
            return sqr_expr_evaluator<3>::eval<mpt>(cA, cB) / (lhs - rhs);
        }
    };

} // detail
} // sweepline
} // boost

#endif
