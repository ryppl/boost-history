
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
    \brief Contains the functions for ANDing, ORing, and XORing.

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

BOOST_XINT_INLINE data_t& data_t::operator&=(const data_t n) {
    size_t len = (std::min)(length, n.length);
    beginmod(len);

    const digit_t *ns = n.digits(), *nse = ns + len;
    digit_t *t = digits();
    while (ns != nse) *t++ &= *ns++;
    length = len;

    endmod();
    return *this;
}

BOOST_XINT_INLINE data_t& data_t::operator|=(const data_t n) {
    size_t len = (std::max)(length, n.length);
    beginmod(len);

    if (length > n.length) {
        const digit_t *ns = n.digits(), *nse = ns + n.length;
        digit_t *t = digits();
        while (ns != nse) *t++ |= *ns++;
    } else {
        const digit_t *ns = n.digits(), *nse = ns + len;
        digit_t *t = digits(), *te = t + length;
        while (t != te) *t++ |= *ns++;
        while (ns != nse) *t++ = *ns++;
        length = len;
    }

    endmod();
    return *this;
}

BOOST_XINT_INLINE data_t& data_t::operator^=(const data_t n) {
    size_t len = (std::max)(length, n.length);
    beginmod(len);

    if (length > n.length) {
        const digit_t *ns = n.digits(), *nse = ns + n.length;
        digit_t *t = digits();
        while (ns != nse) *t++ ^= *ns++;
    } else {
        const digit_t *ns = n.digits(), *nse = ns + len;
        digit_t *t = digits(), *te = t + length;
        while (t != te) *t++ ^= *ns++;
        while (ns != nse) *t++ = *ns++;
        length = len;
    }

    endmod();
    return *this;
}

BOOST_XINT_INLINE data_t operator&(const data_t n1, const data_t n2) {
    return data_t(n1) &= n2;
}

BOOST_XINT_INLINE data_t operator|(const data_t n1, const data_t n2) {
    return data_t(n1) |= n2;
}

BOOST_XINT_INLINE data_t operator^(const data_t n1, const data_t n2) {
    return data_t(n1) ^= n2;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
