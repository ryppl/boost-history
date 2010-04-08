
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

    Also holds the definition for xint::invmod, since it uses the GCD algorithm.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {
namespace core {
namespace {

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
                    std::swap(u1, t1);
                    std::swap(u2, t2);
                    std::swap(u3, t3);
                }
            } while (u3.even());

            while (u1 < t1 || u2 < t2) { u1+=m; u2+=n; }
            u1-=t1; u2-=t2; u3-=t3;
        } while (t3 > 0);

        while (u1 >= m && u2 >= n) { u1-=m; u2-=n; }
    }

    integer u1, u2, u3;
};

} // namespace

integer gcd(const integer& num1, const integer& num2) {
    int sign1=num1.sign(), sign2=num2.sign();
    if (sign1==0 && sign2==0) return integer::zero();
    else if (sign1==0) return num2;
    else if (sign2==0) return num1;

    integer n(abs(num1)), m(abs(num2));

    size_t k=0;
    while (n.even() && m.even()) { ++k; n >>= 1; m >>= 1; }

    gcd_core core(n, m);

    if (core.u3.sign() != 0) return core.u3 << k;
    return integer::one() << k;
}

integer lcm(const integer& num1, const integer& num2) {
    if (num1.sign() == 0 || num2.sign() == 0) return integer::zero();
    return abs(num1 * num2) / gcd(num1, num2);
}

integer invmod(const integer& n, const integer& m) {
    // Calculates the modular inverse of n mod m, or (n^(-1)) mod m
    // Defined as b, where n*b corresponds to 1 (mod m)
    if (m < integer::one()) throw invalid_modulus();

    int sign=n.sign();
    if (sign==0) {
        return integer::zero();
    } else if (n.sign() < 0) {
        integer _n(n);
        _n._set_negative(false);

        integer nn=invmod(_n, m);
        if (nn.sign()==0) return nn;

        nn._set_negative(true);
        return nn + m;
    }

    if (n.even() && m.even()) return integer::zero(); // GCD(x,y)!=1, no inverse possible.

    gcd_core core(n, m);

    if (core.u3 != integer::one()) return integer::zero(); // GCD(x,y)!=1, no inverse possible.
    return core.u1;
}

} // namespace core

/*! \brief Calculate the Greatest Common Denominator of two integers.

\param[in] num1, num2 The integers to operate on.

\returns The greatest common denominator of the two integers, which will always
be a positive number.
*/
integer gcd(const integer& num1, const integer& num2) {
    try {
        return integer(gcd(core::integer(num1), core::integer(num2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

/*! \brief Calculate the Least Common Multiple of two integers.

\param[in] num1, num2 The integers to operate on.

\returns The least common multiple of the two integers. If either integer is
zero, then the return value will be zero, by convention; in all other cases, the
return value will be a positive number.
*/
integer lcm(const integer& num1, const integer& num2) {
    try {
        return integer(lcm(core::integer(num1), core::integer(num2)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

/*! \brief Get the modular inverse of a number in a modulus, if there is one.

\param[in] n The number to retrieve the inverse of.
\param[in] m The modulus to use.

\returns The modular inverse of \c n in \c m. If \c n has no modular inverse in
\c m, returns zero.

\exception xint::invalid_modulus if the modulus is less than one.

\note If exceptions are blocked, it returns zero instead of throwing an
exception.
*/
integer invmod(const integer& n, const integer& m) {
    try {
        return integer(invmod(core::integer(n), core::integer(m)));
    } catch (std::exception&) {
        if (exceptions_allowed()) throw;
        else return integer::nan();
    }
}

} // namespace xint
} // namespace boost
