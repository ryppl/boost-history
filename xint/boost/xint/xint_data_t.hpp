
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
    \brief Internal details of an xint::integer.

    This file contains the declaration for the xint data structure. It should
    only be used by the library itself.
*/

#ifndef BOOST_INCLUDED_XINT_DATA_T_H
#define BOOST_INCLUDED_XINT_DATA_T_H

#if !defined(XINT_SECURE)
    #include <vector>
#endif

namespace boost {
namespace xint {
namespace detail {

/*! \brief Holds the internal details of an xint::integer.

    The data for an integer is stored in a separate struct so it can be shared
    between different copies of an identical number.
*/
struct data_t {
    /*! \brief Holds information determining how many quick-digits to use.

        Smaller numbers are represented in a small array of digits that are part
        of the #data_t structure itself, so that no additional memory allocation
        is needed for them. The contents of this structure determine how many of
        them the library uses.
    */
    struct QuickDigits {
        static const size_t intbits=std::numeric_limits<unsigned int>::digits;
        static const size_t suggested=(2*intbits/bits_per_digit); //!< We want at least enough QuickDigits to hold two standard int types.
        static const size_t minimum=3; //!< The library's code is written to depend on this value, don't change it.
        static const size_t count=(suggested < minimum ? minimum : suggested);
    };

    size_t mLength, mAllocated;
    digit_t *digits; //!< \note The digits are stored lowest-digit-first.
    digit_t mQuickDigits[QuickDigits::count];

    #if !defined(XINT_SECURE)
        std::vector<digit_t> mStorage;
    #endif

    int mCopies;
    bool mIsNegative;

    public:
    data_t(digit_t initial1=0, digit_t initial2=0, digit_t initial3=0);
    data_t(data_t *c);

    #if defined(XINT_SECURE)
        ~data_t();
    #endif

    void attach() { ++mCopies; }
    bool detach() { return (--mCopies==0); }
    void alloc(size_t digits, bool copydigits=false);
    void realloc(size_t newdigits) { alloc(newdigits, true); }
    void skipLeadingZeros();
    void copy(const data_t *c, size_t extraDigits=0);
    void zero(digit_t *p, size_t count);

    void quickset(digit_t d1, digit_t d2=0, digit_t d3=0);

    void invert();
    void negate();

    // These primitives ignore the signs of the parameters, and their results
    // are calculated in place.
    void add(const data_t& addend);
    void subtract(const data_t& subtrahend);

    // These are also calculated in place, for maximum speed.
    void shift_left(size_t byBits);
    void shift_right(size_t byBits);
};

} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_DATA_T_H
