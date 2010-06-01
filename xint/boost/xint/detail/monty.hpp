
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
    \brief Montgomery Reduction functions.

    This file declares the functions that use the Montgomery Reduction. They are
    used internally to speed up the xint::powmod function for odd modulii.
*/

#ifndef BOOST_INCLUDED_XINT_MONTY_HPP
#define BOOST_INCLUDED_XINT_MONTY_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

//! \name Montgomery Reduction functions
//!@{
digit_t inverse0(const data_t n);
data_t montgomeryR(const data_t n);
void toMontgomeryForm(data_t& target, const data_t n, const data_t m);
void fromMontgomeryForm(data_t& target, const data_t n, const data_t m);
data_t montgomeryMultiplyMod(const data_t x, const data_t y, const data_t m,
    digit_t nPrime0);
void montgomeryPowerMod(data_t& target, const data_t x, const data_t e, const
    data_t m);
//!@}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_MONTY_HPP
