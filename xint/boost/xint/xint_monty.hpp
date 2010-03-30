
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file declares the functions that use the Montgomery Reduction. They are
    used internally to speed up the powmod function for odd modulii.
*/

#ifndef BOOST_INCLUDED_XINT_MONTY_H
#define BOOST_INCLUDED_XINT_MONTY_H

namespace xint {

detail::digit_t inverse0(const integer& n);
integer montgomeryR(const integer& n);
integer toMontgomeryForm(const integer& n, const integer& m);
integer fromMontgomeryForm(const integer& n, const integer& m);
integer montgomeryMultiplyMod(const integer& x, const integer& y, const integer&
    m, detail::digit_t nPrime0);
integer montgomeryPowerMod(const integer& x, const integer& e, const integer& m);

} // namespace xint

#endif // BOOST_INCLUDED_XINT_MONTY_H
