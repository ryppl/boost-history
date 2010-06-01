
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
    \brief Contains the definitions of the addition and subtraction functions.

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

void sub_increment(data_t& n, bool absolute_value = false);
void sub_decrement(data_t& n, bool absolute_value = false);

BOOST_XINT_INLINE void sub_add(data_t& n, const data_t n2) {
    n.beginmod((std::max)(n.length, n2.length) + 1, true);

    digit_t *ndig = n.digits(), *t = ndig, *te = t + n.max_length();
    const digit_t *p = n2.digits(), *pe = p + (std::min)(n2.length,
        n.max_length());

    digit_t carry = 0;
    while (p != pe) {
        doubledigit_t tmp = doubledigit_t(*t) + *p++ + carry;
        if (tmp >= digit_overflowbit) {
            carry = 1;
            *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
        } else {
            carry = 0;
            *t++ = static_cast<digit_t>(tmp);
        }
    }

    if (carry) {
        while (t != te) {
            if (*t == digit_mask) *t++ = 0;
            else { *t++ += 1; break; }
        }
    }

    n.length = (std::max)(n.length, size_t(t - ndig));
    n.endmod();
}

BOOST_XINT_INLINE void sub_subtract(data_t& n, const data_t n2) {
    bool swap = (compare(n, n2, true) < 0);

    n.beginmod((std::max)(n.length, n2.length), true);
    digit_t *ndig = n.digits(), *t = ndig, *te = t + n.length;
    const digit_t *p = n2.digits(), *pe = p + (std::min)(n2.length, n.length);

    // Now subtract the digits, starting at the least-significant one.
    digit_t borrow = 0;
    if (swap) {
        // abs(n2) is greater than abs(n). Requires slightly different handling.
        while (p != pe) {
            doubledigit_t tmp = digit_overflowbit + *p++ - *t - borrow;
            if (tmp < digit_overflowbit) {
                borrow = 1;
                *t++ = static_cast<digit_t>(tmp);
            } else {
                borrow = 0;
                *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
            }
        }

        n.length = size_t(t - ndig);
        n.negative = !n.negative;
    } else {
        while (p != pe) {
            doubledigit_t tmp = digit_overflowbit + *t - *p++ - borrow;
            if (tmp < digit_overflowbit) {
                borrow = 1;
                *t++ = static_cast<digit_t>(tmp);
            } else {
                borrow = 0;
                *t++ = static_cast<digit_t>(tmp - digit_overflowbit);
            }
        }

        if (borrow) {
            while (t != te) {
                if (*t == 0) *t++ = digit_mask;
                else { *t++ -= 1; borrow = 0; break; }
            }
        }
        n.length = (std::max)(n.length, size_t(t - ndig));
    }

    assert(borrow == 0);
    n.endmod();
}

BOOST_XINT_INLINE void sub_increment(data_t& n, bool absolute_value) {
    if (n.is_zero()) {
        n.set(1);
    } else if (!absolute_value && n.negative) {
        sub_decrement(n, true);
    } else {
        n.beginmod(n.length + 1, true);
        digit_t *p = n.digits(), *pe = p + n.length;

        while (p < pe) {
            if (*p == digit_mask) *p++ = 0;
            else { *p++ += 1; break; }
        }
        n.endmod();
    }
}

BOOST_XINT_INLINE void sub_decrement(data_t& n, bool absolute_value) {
    if (n.is_zero()) {
        n.set(-1);
    } else if (!absolute_value && n.negative) {
        sub_increment(n, true);
    } else {
        n.beginmod(n.length);
        digit_t *p = n.digits(), *pe = p + n.length;

        while (p < pe) {
            if (*p == 0) *p++ = digit_mask;
            else { *p++ -= 1; break; }
        }
        n.endmod();
    }
}

BOOST_XINT_INLINE data_t& data_t::operator++() {
    sub_increment(*this);
    return *this;
}

BOOST_XINT_INLINE data_t& data_t::operator--() {
    sub_decrement(*this);
    return *this;
}

BOOST_XINT_INLINE data_t data_t::operator++(int) {
    data_t r(*this);
    sub_increment(*this);
    return r;
}

BOOST_XINT_INLINE data_t data_t::operator--(int) {
    data_t r(*this);
    sub_decrement(*this);
    return r;
}

BOOST_XINT_INLINE data_t data_t::operator-() const {
    data_t r(negate());
    r.beginmod();
    r.endmod(true);
    return r;
}

BOOST_XINT_INLINE data_t& data_t::operator+=(const data_t n) {
    if (!n.is_zero()) {
        beginmod();

        if (is_zero()) {
            duplicate_data(n);
        } else if (negative != n.negative) {
            sub_subtract(*this, n.negate());
        } else {
            sub_add(*this, n);
        }

        endmod();
    }
    return *this;
}

BOOST_XINT_INLINE data_t& data_t::operator-=(const data_t n) {
    if (!n.is_zero()) {
        beginmod();

        if (is_zero()) {
            duplicate_data(n.negate());
        } else if (negative != n.negative) {
            sub_add(*this, n.negate());
        } else {
            sub_subtract(*this, n);
        }

        endmod();
    }
    return *this;
}

BOOST_XINT_INLINE data_t operator+(const data_t n1, const data_t n2) {
    data_t r(n1);
    r+=n2;
    return r;
}

BOOST_XINT_INLINE data_t operator-(const data_t n1, const data_t n2) {
    data_t r(n1);
    r-=n2;
    return r;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
