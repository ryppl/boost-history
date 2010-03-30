
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the Greatest Common Denominator function.
*/

#include "../xint.hpp"

namespace xint {

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
    num1._throw_if_nan();
    num2._throw_if_nan();

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
    if (m < integer::one()) {
        if (exceptions_allowed()) throw invalid_modulus();
        else return integer(not_a_number());
    }

    if (n.sign() < 0) {
        integer _n(n);
        _n._set_negative(false);

        integer nn=invmod(_n, m);
        if (nn.nan()) return nn;

        nn._set_negative(true);
        return nn + m;
    }

    if (n.even() && m.even()) return integer(not_a_number()); // GCD(x,y)!=1, no inverse possible.

    gcd_core core(n, m);

    if (core.u3 != integer::one()) return integer(not_a_number()); // GCD(x,y)!=1, no inverse possible.
    return core.u1;
}

} // namespace xint
