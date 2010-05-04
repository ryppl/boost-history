
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
    \brief Greatest Common Denominator and Least Common Multple.

    Also holds the definition for invmod, since it uses the GCD algorithm.
*/

#include "../boost/xint/integer.hpp"

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

struct gcd_core {
    gcd_core(const integer& n, const integer& m): u1(integer::one()),
        u2(integer::zero()), u3(n)
    {
        integer t1=m, t2=n-integer::one(), t3=m;
        do {
            do {
                if (u3.even()) {
                    if (u1.odd() || u2.odd()) { u1+=m; u2+=n; }
                    u1 >>= 1;
                    u2 >>= 1;
                    u3 >>= 1;
                }

                if (t3.even() || u3 < t3) {
                    // Swap the u's with the t's
                    using std::swap;
                    swap(u1, t1);
                    swap(u2, t2);
                    swap(u3, t3);
                }
            } while (u3.even());

            while (u1 < t1 || u2 < t2) { u1+=m; u2+=n; }
            u1-=t1; u2-=t2; u3-=t3;
        } while (t3 > 0);

        while (u1 >= m && u2 >= n) { u1-=m; u2-=n; }
    }

    integer u1, u2, u3;
};

void gcd(base_integer& target, const base_integer& num1, const base_integer&
    num2)
{
    if (num1._is_zero() && num2._is_zero()) {
        target._set_unsigned(0);
    } else if (num1._is_zero()) {
        target._attach(num2, false);
    } else if (num2._is_zero()) {
        target._attach(num1, false);
    } else {
        temp_t n(num1), m(num2);
        n._set_negative(false);
        m._set_negative(false);

        size_t k = 0;
        if (!getbit(n, k) && !getbit(m, k)) ++k;
        if (k != 0) {
            shift_right(n, n, k);
            shift_right(m, m, k);
        }

        gcd_core core(n._to_integer(false), m._to_integer(false));

        if (core.u3._is_zero()) {
            shift_left(target, fixed_integer_any(1), k);
        } else {
            shift_left(target, core.u3, k);
        }
    }
}

void lcm(base_integer& target, const base_integer& num1, const base_integer&
    num2)
{
    if (num1._is_zero() || num2._is_zero()) {
        target._set_unsigned(0);
    } else {
        integer common;
        gcd(common, num1, num2);

        integer answer, remainder;
        multiply(answer, num1, num2);
        answer._set_negative(false);
        divide(answer, remainder, answer, common);

        target._attach(answer, true);
    }
}

void invmod(base_integer& target, const base_integer& n, const base_integer& m)
{
    // Calculates the modular inverse of n mod m, or (n^(-1)) mod m
    // Defined as b, where n*b corresponds to 1 (mod m)
    if (m._is_zero() || m._get_negative()) throw exceptions::invalid_modulus();

    if (n._is_zero()) {
        target._set_unsigned(0);
    } else if (n._get_negative()) {
        integer _n(n._to_integer(false));
        _n._set_negative(false);

        integer nn;
        invmod(nn, _n, m);
        if (nn._is_zero()) {
            target._attach(nn, true);
        } else {
            nn._set_negative(true);
            add(target, nn, m);
        }
    } else {
        integer nn(n._to_integer(false)), mm(m._to_integer(false));
        if (nn.even() && mm.even()) {
            // GCD != 1, no inverse possible.
            target._set_unsigned(0);
            return;
        }

        gcd_core core(nn, mm);

        if (core.u3 != integer::one()) {
            // GCD != 1, no inverse possible.
            target._set_unsigned(0);
            return;
        }

        target._attach(core.u1, true);
    }
}

} // namespace detail
//! @endcond detail

/*! \brief Calculate the Greatest Common Denominator of two integers.

- Complexity: O((log n)<sup>3</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The greatest common denominator of the two integers, which will always
be a positive number.
*/
integer gcd(const integer& num1, const integer& num2) {
    integer r;
    detail::gcd(r, num1, num2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the Least Common Multiple of two integers.

- Complexity: O((log n)<sup>3</sup> + n<sup>2</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The least common multiple of the two integers. If either integer is
zero, then the return value will be zero, by convention; in all other cases, the
return value will be a positive number.
*/
integer lcm(const integer& num1, const integer& num2) {
    integer r;
    detail::lcm(r, num1, num2);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the modular inverse of a number in a modulus, if there is one.

- Complexity: O((log n)<sup>3</sup>)

\param[in] n The number to calculate the inverse of.
\param[in] m The modulus to use.

\returns The modular inverse of \c n in \c m. If \c n has no modular inverse in
\c m, returns zero.

\exception exceptions::invalid_modulus if the modulus is less than one.
*/
integer invmod(const integer& n, const integer& m) {
    integer r;
    detail::invmod(r, n, m);
    return BOOST_XINT_MOVE(r);
}

} // namespace xint
} // namespace boost
