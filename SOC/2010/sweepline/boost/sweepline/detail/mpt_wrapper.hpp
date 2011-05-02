// Boost sweepline/mpt_wrapper.hpp header file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SWEEPLINE_MPT_WRAPPER
#define BOOST_SWEEPLINE_MPT_WRAPPER

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

        mpt_wrapper(const mpt_wrapper& w) : m_(w.m_) {}

        template <typename mpt2, int N2>
        mpt_wrapper(const mpt_wrapper<mpt2, N2>& w) : m_(w.m_) {}

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

        template <typename mpt2, int N2>
        mpt_wrapper& operator=(const mpt_wrapper<mpt2, N2>& that) {
            m_ = that.get_mpt();
            return *this;
        }

        double get_d() const {
            return m_.get_d();
        }

        std::string get_str() const {
            return m_.get_str();
        }

        const mpt& get_mpt() const {
            return m_;
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
            if (that == 0)
                return m_.__get_mp()->_mp_size == 0;
            temp_[cur_] = that;
            return m_ == temp_[cur_].m_;
        }

        bool operator!=(int that) const {
            return !(*this == that);
        }

        bool operator<(int that) const {
            if (that == 0)
                return m_.__get_mp()->_mp_size < 0;
            temp_[cur_] = that;
            return m_ < temp_[cur_].m_;
        }

        bool operator<=(int that) const {
            if (that == 0)
                return m_.__get_mp()->_mp_size <= 0;
            temp_[cur_] = that;
            return m_ <= temp_[cur_].m_;
        }

        bool operator>(int that) const {
            if (that == 0)
                return m_.__get_mp()->_mp_size > 0;
            temp_[cur_] = that;
            return m_ > temp_[cur_].m_;
        }

        bool operator>=(int that) const {
            if (that == 0)
                return m_.__get_mp()->_mp_size >= 0;
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

        mpt_wrapper& operator/(const mpt_wrapper& that) const {
            temp_[cur_].m_ = this->m_ / that.m_;
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

        mpt_wrapper& operator/=(const mpt_wrapper& that) {
            this->m_ /= that.m_;
            return *this;
        }

        mpt_wrapper& get_sqrt() {
            temp_[cur_].m_ = sqrt(m_);
            return temp_[next_cur()];
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

    template<int N>
    mpt_wrapper<mpf_class, N>& sqrt(mpt_wrapper<mpf_class, N>& value) {
        return value.get_sqrt();
    }

} // detail
} // sweepline
} // boost

#endif
