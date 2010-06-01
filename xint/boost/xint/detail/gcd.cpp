
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains functions using the Greatest Common Denominator algorithm.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

struct gcd_core {
    gcd_core(const data_t n, const data_t m): u1(n.new_holder(), 1),
        u2(n.new_holder()), u3(n)
    {
        const data_t zero(n.new_holder());
        data_t t1(m), t2(n), t3(m);
        t1.beginmod();
        t2.beginmod();
        t3.beginmod();
        u1.beginmod();
        u2.beginmod();
        u3.beginmod();

        --t2;
        do {
            do {
                if (u3.is_even()) {
                    if (u1.is_odd() || u2.is_odd()) { u1 += m; u2 += n; }
                    u1 >>= 1;
                    u2 >>= 1;
                    u3 >>= 1;
                }

                if (t3.is_even() || u3 < t3) {
                    // Swap the u's with the t's
                    using std::swap;
                    swap(u1, t1);
                    swap(u2, t2);
                    swap(u3, t3);
                }
            } while (u3.is_even());

            while (u1 < t1 || u2 < t2) { u1 += m; u2 += n; }
            u1 -= t1; u2 -= t2; u3 -= t3;
        } while (t3 > zero);

        while (u1 >= m && u2 >= n) { u1 -= m; u2 -= n; }

        u3.endmod();
        u2.endmod();
        u1.endmod();
        t3.endmod();
        t2.endmod();
        t1.endmod();
    }

    data_t u1, u2, u3;
};

BOOST_XINT_INLINE void gcd(data_t& target, const data_t num1, const data_t num2)
{
    target.beginmod();
    if (num1.is_zero() && num2.is_zero()) {
        target.set(0);
    } else if (num1.is_zero()) {
        target.duplicate_data(num2);
    } else if (num2.is_zero()) {
        target.duplicate_data(num1);
    } else {
        data_t n(num1, false), m(num2, false);

        size_t k = 0;
        if (!getbit(n, k) && !getbit(m, k)) ++k;
        if (k != 0) { n >>= k; m >>= k; }

        gcd_core core(n, m);
        if (core.u3.is_zero()) {
            target.set(1);
            target <<= k;
        } else {
            target.duplicate_data(core.u3);
            target <<= k;
        }
    }
    target.endmod();
}

BOOST_XINT_INLINE void lcm(data_t& target, const data_t num1, const data_t num2)
{
    if (num1.is_zero() || num2.is_zero()) {
        target.set(0);
    } else {
        data_t common(target.new_holder());
        gcd(common, num1, num2);

        target.beginmod();
        target.duplicate_data(num1);
        target *= num2;
        target /= common;
        target.negative = false;
        target.endmod();
    }
}

//! Calculates the modular inverse of n mod m, or (n^(-1)) mod m
//! Defined as b, where n*b corresponds to 1 (mod m)
BOOST_XINT_INLINE void invmod(data_t& target, const data_t n, const data_t m) {
    if (m.is_zero() || m.negative) throw exceptions::invalid_modulus();

    if (n.is_zero()) {
        target.set(0);
    } else if (n.negative) {
        target.beginmod();
        invmod(target, n.abs(), m);
        if (!target.is_zero()) {
            target.negative = true;
            target += m;
        }
        target.endmod();
    } else {
        if (n.is_even() && m.is_even()) {
            // GCD != 1, no inverse possible.
            target.set(0);
        } else {
            gcd_core core(n, m);
            if (core.u3 != data_t(target.new_holder(), 1)) {
                // GCD != 1, no inverse possible.
                target.set(0);
            } else target.duplicate_data(core.u1);
        }
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
