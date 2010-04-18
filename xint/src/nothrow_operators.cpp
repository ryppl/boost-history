
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
    \brief Contains the definitions for the basic math operators for the
           \c nothrow_integer type.
*/

#include "../boost/xint/xint.hpp"

namespace boost {
namespace xint {

const nothrow_integer& operator+(const nothrow_integer& a) { return a; }
nothrow_integer operator-(const nothrow_integer& a) { return negate(a); }
nothrow_integer operator+(const nothrow_integer& num1, const nothrow_integer& num2) { return add(num1, num2); }
nothrow_integer operator-(const nothrow_integer& num1, const nothrow_integer& num2) { return subtract(num1, num2); }
nothrow_integer operator*(const nothrow_integer& num1, const nothrow_integer& num2) { return multiply(num1, num2); }
nothrow_integer operator/(const nothrow_integer& num1, const nothrow_integer& num2) { return divide(num1, num2); }
nothrow_integer operator%(const nothrow_integer& num1, const nothrow_integer& num2) { return mod(num1, num2); }
nothrow_integer operator&(const nothrow_integer& n1, const nothrow_integer& n2) { return bitwise_and(n1, n2); }
nothrow_integer operator|(const nothrow_integer& n1, const nothrow_integer& n2) { return bitwise_or(n1, n2); }
nothrow_integer operator^(const nothrow_integer& n1, const nothrow_integer& n2) { return bitwise_xor(n1, n2); }

} // namespace xint
} // namespace boost
