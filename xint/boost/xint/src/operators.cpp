
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the definitions for the basic math operators.
*/

#include "../xint.hpp"

const xint::integer& operator+(const xint::integer& a) { return a; }
xint::integer operator-(const xint::integer& a) { return xint::negate(a); }
xint::integer operator+(const xint::integer& num1, const xint::integer& num2) { return xint::add(num1, num2); }
xint::integer operator-(const xint::integer& num1, const xint::integer& num2) { return xint::subtract(num1, num2); }
xint::integer operator*(const xint::integer& num1, const xint::integer& num2) { return xint::multiply(num1, num2); }
xint::integer operator/(const xint::integer& num1, const xint::integer& num2) { return xint::divide(num1, num2); }
xint::integer operator%(const xint::integer& num1, const xint::integer& num2) { return xint::mod(num1, num2); }
xint::integer operator&(const xint::integer& n1, const xint::integer& n2) { return xint::bitwise_and(n1, n2); }
xint::integer operator|(const xint::integer& n1, const xint::integer& n2) { return xint::bitwise_or(n1, n2); }
xint::integer operator^(const xint::integer& n1, const xint::integer& n2) { return xint::bitwise_xor(n1, n2); }
