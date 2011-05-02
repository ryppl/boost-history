// Boost sweepline/mpt_wrapper.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.    

#ifndef VORONOI_SWEEPLINE_ROBUST_FPT
#define VORONOI_SWEEPLINE_ROBUST_FPT

namespace boost {
namespace sweepline {
namespace detail {

    // Represents the result of the epsilon robust predicate.
    // If the result is undefined some further processing is usually required.
    enum kPredicateResult {
        LESS = -1,
        UNDEFINED = 0,
        MORE = 1,
    };

    // If two floating-point numbers in the same format are ordered (x < y),
    // then they are ordered the same way when their bits are reinterpreted as
    // sign-magnitude integers. Values are considered to be almost equal if
    // their integer reinterpretatoins differ in not more than maxUlps units.
    template <typename T>
    static bool almost_equal(T a, T b, unsigned int ulps);

    template<>
    bool almost_equal<float>(float a, float b, unsigned int maxUlps) {
	    unsigned int ll_a, ll_b;

        // Reinterpret double bits as 32-bit signed integer.
        memcpy(&ll_a, &a, sizeof(float));
        memcpy(&ll_b, &b, sizeof(float));

        if (ll_a < 0x80000000)
            ll_a = 0x80000000 - ll_a;
        if (ll_b < 0x80000000)
            ll_b = 0x80000000 - ll_b;

	    if (ll_a > ll_b)
            return ll_a - ll_b <= maxUlps;
        return ll_b - ll_a <= maxUlps;
    }

    template<>
    bool almost_equal<double>(double a, double b, unsigned int maxUlps) {
        unsigned long long ll_a, ll_b;

        // Reinterpret double bits as 64-bit signed integer.
        memcpy(&ll_a, &a, sizeof(double));
        memcpy(&ll_b, &b, sizeof(double));

        // Positive 0.0 is integer zero. Negative 0.0 is 0x8000000000000000.
        // Map negative zero to an integer zero representation - making it
        // identical to positive zero - the smallest negative number is
        // represented by negative one, and downwards from there.
        if (ll_a < 0x8000000000000000ULL)
            ll_a = 0x8000000000000000ULL - ll_a;
        if (ll_b < 0x8000000000000000ULL)
            ll_b = 0x8000000000000000ULL - ll_b;

        // Compare 64-bit signed integer representations of input values.
        // Difference in 1 Ulp is equivalent to a relative error of between
        // 1/4,000,000,000,000,000 and 1/8,000,000,000,000,000.
        if (ll_a > ll_b)
            return ll_a - ll_b <= maxUlps;
        return ll_b - ll_a <= maxUlps;
    }

    template <typename T>
    double get_d(const T& value) {
        return value.get_d();
    }

    template <>
    double get_d(const float& value) {
        return value;
    }

    template <>
    double get_d(const double& value) {
        return value;
    }

    template <typename _fpt>
    class robust_fpt {
    public:
	    typedef _fpt floating_point_type;
	    typedef double relative_error_type;

	    // Rounding error is at most 1 EPS.
	    static const relative_error_type ROUNDING_ERROR;

	    // Constructors.
	    robust_fpt() : fpv_(0.0), re_(0) {}
        explicit robust_fpt(int fpv) : fpv_(fpv), re_(0) {}
        explicit robust_fpt(floating_point_type fpv, bool rounded = true) : fpv_(fpv) {
            re_ = rounded ? ROUNDING_ERROR : 0;
        }
        robust_fpt(floating_point_type fpv, relative_error_type error) :
            fpv_(fpv), re_(error) {}

        floating_point_type fpv() const { return fpv_; }
        relative_error_type re() const { return re_; }
        relative_error_type ulp() const { return re_; }

        bool operator==(double that) const {
            if (that == 0)
                return this->fpv_ == that;
            return almost_equal(this->fpv_, that, 
                                static_cast<unsigned int>(this->ulp()));
        }

        bool operator!=(double that) const {
            return !(*this == that);
        }

        bool operator<(double that) const {
            if (*this == that)
                return false;
            return this->fpv_ < that;
        }

        bool operator<=(double that) const {
            if (*this == that)
                return true;
            return this->fpv_ < that;
        }

        bool operator>(double that) const {
            if (*this == that)
                return false;
            return this->fpv_ > that;
        }

        bool operator>=(double that) const {
            if (*this == that)
                return true;
            return this->fpv_ > that;
        }

	    bool operator==(const robust_fpt &that) const {
		    unsigned int ulp = static_cast<unsigned int>(ceil(this->re_ + that.re_));
		    return almost_equal(this->fpv_, that.fpv_, ulp);	
	    }

	    bool operator!=(const robust_fpt &that) const {
		    return !(*this == that);
	    }

	    bool operator<(const robust_fpt &that) const {
		    if (*this == that)
			    return false;
		    return this->fpv_ < that.fpv_;
	    }

	    bool operator>(const robust_fpt &that) const {
		    return that < *this;
	    }

	    bool operator<=(const robust_fpt &that) const {
		    return !(that < *this);
	    }

	    bool operator>=(const robust_fpt &that) const {
		    return !(*this < that);
	    }

	    bool operator-() const {
		    return robust_fpt(-fpv_, re_);
	    }

	    robust_fpt& operator=(const robust_fpt &that) {
		    this->fpv_ = that.fpv_;
		    this->relative_error_ = that.re_;
		    return *this;
	    }

	    robust_fpt& operator+=(const robust_fpt &that) {
            floating_point_type fpv = this->fpv_ + that.fpv_;
            if ((this->fpv_ >= 0 && that.fpv_ >= 0) ||
                (this->fpv_ <= 0 && that.fpv_ <= 0))
                this->re_ = (std::max)(this->re_, that.re_) + ROUNDING_ERROR;
            else {            
                floating_point_type temp = (this->fpv_ * this->re_ - that.fpv_ * that.re_) / fpv;
                this->re_ = fabs(get_d(temp)) + ROUNDING_ERROR;
            }
            this->fpv_ = fpv;
		    return *this;
	    }

	    robust_fpt& operator-=(const robust_fpt &that) {
            floating_point_type fpv = this->fpv_ - that.fpv_;
            if ((this->fpv_ >= 0 && that.fpv_ <= 0) ||
                (this->fpv_ <= 0 && that.fpv_ >= 0))
                this->re_ = (std::max)(this->re_, that.re_) + ROUNDING_ERROR;
            else {
                floating_point_type temp = (this->fpv_ * this->re_ + that.fpv_ * that.re_) / fpv;
                this->re_ = fabs(get_d(temp)) + ROUNDING_ERROR;
            }
            this->fpv_ = fpv;
		    return *this;
	    }

	    robust_fpt& operator*=(const robust_fpt &that) {
		    this->re_ += that.re_ + ROUNDING_ERROR;
		    this->fpv_ *= that.fpv_;
            return *this;
	    }

	    robust_fpt& operator/=(const robust_fpt &that) {
	        this->re_ += that.re_ + ROUNDING_ERROR;
		    this->fpv_ /= that.fpv_;
            return *this;
	    }

        robust_fpt operator+(const robust_fpt &that) const {
            floating_point_type fpv = this->fpv_ + that.fpv_;
            relative_error_type re;
            if ((this->fpv_ >= 0 && that.fpv_ >= 0) ||
                (this->fpv_ <= 0 && that.fpv_ <= 0))
                re = (std::max)(this->re_, that.re_) + ROUNDING_ERROR;
            else {
                floating_point_type temp = (this->fpv_ * this->re_ - that.fpv_ * that.re_) / fpv;
                re = fabs(get_d(temp)) + ROUNDING_ERROR;
            }
            return robust_fpt(fpv, re);
        }

        robust_fpt operator-(const robust_fpt &that) const {
            floating_point_type fpv = this->fpv_ - that.fpv_;
            relative_error_type re;
            if ((this->fpv_ >= 0 && that.fpv_ <= 0) ||
                (this->fpv_ <= 0 && that.fpv_ >= 0))
                re = (std::max)(this->re_, that.re_) + ROUNDING_ERROR;
            else {
                floating_point_type temp = (this->fpv_ * this->re_ + that.fpv_ * that.re_) / fpv;
                re = fabs(get_d(temp)) + ROUNDING_ERROR;
            }
            return robust_fpt(fpv, re);
        }

        robust_fpt operator*(const robust_fpt &that) const {
            floating_point_type fpv = this->fpv_ * that.fpv_;
            relative_error_type re = this->re_ + that.re_ + ROUNDING_ERROR;
            return robust_fpt(fpv, re);
        }

        robust_fpt operator/(const robust_fpt &that) const {
            floating_point_type fpv = this->fpv_ / that.fpv_;
            relative_error_type re = this->re_ + that.re_ + ROUNDING_ERROR;
            return robust_fpt(fpv, re);
        }

        robust_fpt get_sqrt() const {
            return robust_fpt(std::sqrt(fpv_), re_ * 0.5 + ROUNDING_ERROR);
        }

    private:
        floating_point_type fpv_;
        relative_error_type re_;
    };

    template <typename T>
    const typename robust_fpt<T>::relative_error_type robust_fpt<T>::ROUNDING_ERROR = 1;

    // Class used to make computations with epsilon relative error.
    // ERC consists of two values: value1 and value2, both positive.
    // The resulting expression is equal to the value1 - value2.
    // The main idea is to represent any expression that consists of
    // addition, substraction, multiplication and division operations
    // to avoid using substraction. Substraction of a positive value
    // is equivalent to the addition to value2 and substraction of
    // a negative value is equivalent to the addition to value1.
    // Cons: ERC gives error relative not to the resulting value,
    //       but relative to some expression instead. Example:
    //       center_x = 100, ERC's value1 = 10^20, value2 = 10^20,
    //       center_x = 1000, ERC's value3 = 10^21, value4 = 10^21,
    //       such two centers are considered equal(
    //       value1 + value4 = value2 + value3), while they are not.
    // Pros: ERC is much faster then approaches based on the use
    //       of high-precision libraries. However this will give correct
    //       answer for the previous example.
    // Solution: Use ERCs in case of defined comparison results and use
    //           high-precision libraries for undefined results.
    template <typename T>
    class epsilon_robust_comparator {
    public:
        epsilon_robust_comparator() :
          positive_sum_(0),
          negative_sum_(0) {}

        epsilon_robust_comparator(const T &value) :
          positive_sum_((value>0)?value:0),
          negative_sum_((value<0)?-value:0) {}

        epsilon_robust_comparator(const T &pos, const T &neg) :
          positive_sum_(pos),
          negative_sum_(neg) {}

        T dif() const {
            return positive_sum_ - negative_sum_;
        }

        T pos() const {
            return positive_sum_;
        }

        T neg() const {
            return negative_sum_;
        }

        // Equivalent to the unary minus.
        void swap() {
            (std::swap)(positive_sum_, negative_sum_);
        }

        bool abs() {
            if (positive_sum_ < negative_sum_) {
                swap();
                return true;
            }
            return false;
        }

        epsilon_robust_comparator<T> &operator+=(const T &val) {
            if (val >= 0)
                positive_sum_ += val;
            else
                negative_sum_ -= val;
            return *this;
        }

        epsilon_robust_comparator<T> &operator+=(
            const epsilon_robust_comparator<T> &erc) {
            positive_sum_ += erc.positive_sum_;
            negative_sum_ += erc.negative_sum_;
            return *this;
        }

        epsilon_robust_comparator<T> &operator-=(const T &val) {
            if (val >= 0)
                negative_sum_ += val;
            else
                positive_sum_ -= val;
            return *this;
        }

        epsilon_robust_comparator<T> &operator-=(
            const epsilon_robust_comparator<T> &erc) {
            positive_sum_ += erc.negative_sum_;
            negative_sum_ += erc.positive_sum_;
            return *this;
        }

        epsilon_robust_comparator<T> &operator*=(const T &val) {
            positive_sum_ *= fabs(val);
            negative_sum_ *= fabs(val);
            if (val < 0) {
                swap();
            }
            return *this;
        }

        epsilon_robust_comparator<T> &operator/=(const T &val) {
            positive_sum_ /= fabs(val);
            negative_sum_ /= fabs(val);
            if (val < 0) {
                swap();
            }
            return *this;
        }

        // Compare predicate with value using ulp precision.
        kPredicateResult compare(T value, int ulp = 0) const {
            T lhs = positive_sum_ - ((value < 0) ? value : 0);
            T rhs = negative_sum_ + ((value > 0) ? value : 0);
            if (almost_equal(lhs, rhs, ulp))
                return UNDEFINED;
            return (lhs < rhs) ? LESS : MORE;
        }

        // Compare two predicats using ulp precision.
        kPredicateResult compare(const epsilon_robust_comparator &rc,
                                 int ulp = 0) const {
            T lhs = positive_sum_ + rc.neg();
            T rhs = negative_sum_ + rc.pos();
            if (almost_equal(lhs, rhs, ulp))
                return UNDEFINED;
            return (lhs < rhs) ? LESS : MORE;
        }

        // Check whether comparison has undefined result.
        bool compares_undefined(const epsilon_robust_comparator &rc,
                                int ulp) const {
            return this->compare(rc, ulp) == UNDEFINED;
        }

    private:
        T positive_sum_;
        T negative_sum_;
    };

    template<typename T>
    inline epsilon_robust_comparator<T> operator+(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        return epsilon_robust_comparator<T>(lhs.pos() + rhs.pos(),
                                            lhs.neg() + rhs.neg());
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator-(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        return epsilon_robust_comparator<T>(lhs.pos() - rhs.neg(),
                                            lhs.neg() + rhs.pos());
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const epsilon_robust_comparator<T>& lhs,
        const epsilon_robust_comparator<T>& rhs) {
        double res_pos = lhs.pos() * rhs.pos() + lhs.neg() * rhs.neg();
        double res_neg = lhs.pos() * rhs.neg() + lhs.neg() * rhs.pos();
        return epsilon_robust_comparator<T>(res_pos, res_neg);
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const epsilon_robust_comparator<T>& lhs, const T& val) {
        if (val >= 0)
            return epsilon_robust_comparator<T>(lhs.pos() * val,
                                                lhs.neg() * val);
        return epsilon_robust_comparator<T>(-lhs.neg() * val,
                                            -lhs.pos() * val);
    }

    template<typename T>
    inline epsilon_robust_comparator<T> operator*(
        const T& val, const epsilon_robust_comparator<T>& rhs) {
        if (val >= 0)
            return epsilon_robust_comparator<T>(val * rhs.pos(),
                                                val * rhs.neg());
        return epsilon_robust_comparator<T>(-val * rhs.neg(),
                                            -val * rhs.pos());
    }

} // detail
} // sweepline
} // boost

#endif