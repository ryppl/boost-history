// Boost sweepline/mpz_arithmetic.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_MPZ_ARITHMETIC
#define BOOST_SWEEPLINE_MPZ_ARITHMETIC

#include <cmath>

#pragma warning(disable:4800)
#include <gmpxx.h>

namespace boost {
namespace sweepline {
namespace detail {

	// Allows to compute expression without allocation of additional memory
	// for temporary mpz variables. Expression should contain less then SZ
	// temporary values.
	class mpz_wrapper {
	public:
		mpz_wrapper() {}

		explicit mpz_wrapper(int input) : m_(input) {}
		
		explicit mpz_wrapper(double input) : m_(input) {}
		
		mpz_wrapper(const mpz_class& input) : m_(input) {}

		mpz_wrapper(const mpz_wrapper& w) : m_(w.m_) {
			cur_ = 0;
		}

		mpz_wrapper& operator=(int that) {
			m_ = that;
			return *this;
		}

		mpz_wrapper& operator=(double that) {
			m_ = that;
			return *this;
		}
		
		mpz_wrapper& operator=(const mpz_wrapper& that) {
			m_ = that.m_;
			return *this;
		}

		double get_d() const {
			return m_.get_d();
		}

		std::string get_str() const {
			return m_.get_str();
		}

		bool operator==(const mpz_wrapper& that) const {
			return m_ == that.m_;
		}

		bool operator!=(const mpz_wrapper& that) const {
			return m_ != that.m_;
		}

		bool operator<(const mpz_wrapper& that) const {
			return m_ < that.m_;
		}

		bool operator<=(const mpz_wrapper& that) const {
			return m_ <= that.m_;
		}

		bool operator>(const mpz_wrapper& that) const {
			return m_ > that.m_;
		}

		bool operator>=(const mpz_wrapper& that) const {
			return m_ >= that.m_;
		}

		mpz_wrapper& operator-() const {
			temp_[cur_].m_ = -this->m_;
			return temp_[next_cur()];
		}

		mpz_wrapper& operator+(const mpz_wrapper& that) const {
			temp_[cur_].m_ = this->m_ + that.m_;
			return temp_[next_cur()];
		}

		mpz_wrapper& operator-(const mpz_wrapper& that) const {
			temp_[cur_].m_ = this->m_ - that.m_;
			return temp_[next_cur()];
		}

		mpz_wrapper& operator*(const mpz_wrapper& that) const {
			temp_[cur_].m_ = this->m_ * that.m_;
			return temp_[next_cur()];
		}

		mpz_wrapper& operator*(double that) const {
			temp_[cur_].m_ = that;
			temp_[cur_].m_ *= this->m_;
			return temp_[next_cur()];
		}

		mpz_wrapper& operator+=(const mpz_wrapper& that) {
			this->m_ += that.m_;
			return *this;
		}

		mpz_wrapper& operator-=(const mpz_wrapper& that) {
			this->m_ -= that.m_;
			return *this;
		}

		mpz_wrapper& operator*=(const mpz_wrapper& that) {
			this->m_ *= that.m_;
			return *this;
		}

		bool is_positive() const {
			return m_.__get_mp()->_mp_size > 0;
		}

		bool is_negative() const {
			return m_.__get_mp()->_mp_size < 0;
		}

	private:
		static int next_cur() {
			int ret_val = cur_;
			cur_ = ++cur_ % SZ;
			return ret_val;
		}

		mpz_class m_;
		static const int SZ = 8;
		static int cur_;
		static mpz_wrapper temp_[SZ];
	};

	int mpz_wrapper::cur_ = 0;
	mpz_wrapper mpz_wrapper::temp_[mpz_wrapper::SZ];

	static mpz_wrapper& mpz_cross(double a1, double b1, double a2, double b2) {
		static mpz_wrapper a[2], b[2];
		a[0] = a1;
		a[1] = a2;
		b[0] = b1;
		b[1] = b2;
		return a[0] * b[1] - a[1] * b[0];
	}

	static void swap(mpz_wrapper &a, mpz_wrapper &b) {
		static mpz_wrapper c;
		c = a;
		a = b;
		b = c;
	}

	static void sign_sort(mpz_wrapper *A, mpz_wrapper *B, int SZ) {
		for (int i = SZ-1; i >= 0; --i)
			for (int j = 0; j < i; ++j)
				if (!A[j].is_positive() && !A[j+1].is_negative()) {
					swap(A[j], A[j+1]);
					swap(B[j], B[j+1]);
				}
	}
	
	template <int N>
	struct sqr_expr_evaluator {
		static double eval(mpz_class *A, mpz_class *B);
	};

	// Evaluates expression:
	// A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) with
	// 7 * EPS relative error in the worst case.
	template <>
	struct sqr_expr_evaluator<2> {
		static double eval(mpz_wrapper *A, mpz_wrapper *B) {
			sign_sort(A, B, 2);
			double ret_val = fabs(A[0].get_d()) * sqrt(B[0].get_d()) +
							 fabs(A[1].get_d()) * sqrt(B[1].get_d());
			if (ret_val == 0.0)
				return 0.0;
			if (!A[1].is_negative())
				return ret_val;
			if (!A[0].is_positive())
				return -ret_val;
			return (A[0] * A[0] * B[0] - A[1] * A[1] * B[1]).get_d() / ret_val;
		}
	};

	// Evaluates expression:
	// A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2])
	// with 14 * EPS relative error in the worst case.
	template <>
	struct sqr_expr_evaluator<3> {
		static double eval(mpz_wrapper *A, mpz_wrapper *B) {
			sign_sort(A, B, 3);
			static mpz_wrapper cA[2], cB[2];
			double res = fabs(A[0].get_d()) * sqrt(B[0].get_d()) +
						 fabs(A[1].get_d()) * sqrt(B[1].get_d()) +
						 fabs(A[2].get_d()) * sqrt(B[2].get_d());
			if (res == 0.0)
				return res;
			if (!A[2].is_negative())
				return res;
			if (!A[0].is_positive())
				return -res;
			cA[0] = A[0] * A[0] * B[0];
			cB[0] = 1;
			if (!A[1].is_negative()) {
				cA[0] += A[1] * A[1] * B[1] - A[2] * A[2] * B[2];
				cA[1] = A[0] * A[1];
				cA[1] = cA[1] + cA[1];
				cB[1] = B[0] * B[1];
			} else {
				cA[0] -= A[1] * A[1] * B[1] + A[2] * A[2] * B[2];
				cA[1] = A[1] * A[2];
				cA[1] = -cA[1];
				cA[1] = cA[1] + cA[1];
				cB[1] = B[1] * B[2];
			}
			return sqr_expr_evaluator<2>::eval(cA, cB) / res;
		}
	};

	// Evaluates expression:
	// A[0] * sqrt(B[0]) + A[1] * sqrt(B[1]) + A[2] * sqrt(B[2]) + A[3] * sqrt(B[3])
	// with 23 * EPS relative error in the worst case.
	template <>
	struct sqr_expr_evaluator<4> {
		static double eval(mpz_wrapper *A, mpz_wrapper *B) {
			sign_sort(A, B, 4);
			static mpz_wrapper cA[3], cB[3];
			double res = fabs(A[0].get_d()) * sqrt(B[0].get_d()) +
						 fabs(A[1].get_d()) * sqrt(B[1].get_d()) +
						 fabs(A[2].get_d()) * sqrt(B[2].get_d()) +
						 fabs(A[3].get_d()) * sqrt(B[3].get_d());
			if (res == 0.0)
				return res;
			if (!A[3].is_negative())
				return res;
			if (!A[0].is_positive())
				return -res;
			bool three_neg = !A[1].is_positive();
			bool three_pos = !A[2].is_negative();
			if (!A[1].is_positive() || !A[2].is_negative()) {
				if ((A[1] * A[1] * B[1] <= A[2] * A[2] * B[2]) ^ A[2].is_negative()) {
					swap(A[1], A[2]);
					swap(B[1], B[2]);
				}
			}
			cA[0] = 0;
			for (int i = 0; i < 4; ++i)
				if (i < 2)
					cA[0] += A[i] * A[i] * B[i];
				else
					cA[0] -= A[i] * A[i] * B[i];
			cA[1] = A[0] * A[1];
			cA[1] += cA[1];
			cA[2] = A[2] * A[3];
			cA[2] += cA[2];
			cA[2] = -cA[2];
			cB[0] = 1;
			cB[1] = B[0] * B[1];
			cB[2] = B[2] * B[3];
			double numerator = sqr_expr_evaluator<3>::eval(cA, cB);
			if (!three_pos && !three_neg)
				return numerator / res;
			double denominator = fabs(A[0].get_d()) * sqrt(B[0].get_d()) +
								 fabs(A[3].get_d()) * sqrt(B[3].get_d());
			A[2] = -A[2];
			return numerator / (denominator + sqr_expr_evaluator<2>::eval(&A[1], &B[1]));
		}
	};

} // detail
} // sweepline
} // boost

#endif
