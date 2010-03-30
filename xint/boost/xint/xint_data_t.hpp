
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the declaration for the xint data structure. It should
    only be used by the library itself.

    The data for an integer is stored in a separate struct so it can be shared
    between different copies of an identical number.

    The digits are stored low-digit-first.

    Define XINT_SECURE to zero all memory before releasing it.
*/

#ifndef BOOST_INCLUDED_XINT_DATA_T_H
#define BOOST_INCLUDED_XINT_DATA_T_H

#if !defined(XINT_SECURE)
    #include <vector>
#endif

namespace xint {
namespace detail {

struct data_t {
    struct QuickDigits {
        // We want at least enough QuickDigits to hold two ints.
        static const int intbits=std::numeric_limits<unsigned int>::digits;
        static const int suggested=(2*intbits/bits_per_digit);
        static const int minimum=3;
        static const int count=(suggested < minimum ? minimum : suggested);
    };

    int mLength, mAllocated;
    digit_t *digits, mQuickDigits[QuickDigits::count];

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
    void alloc(int digits, bool copydigits=false);
    void realloc(int newdigits) { alloc(newdigits, true); }
    void skipLeadingZeros();
    void copy(const data_t *c, int extraDigits=0);
    void zero(digit_t *p, size_t count);

    void quickset(digit_t d1, digit_t d2=0, digit_t d3=0);

    void invert();
    void negate();

    // These primitives ignore the signs of the parameters, and their results
    // are calculated in place.
    void add(const data_t& addend);
    void subtract(const data_t& subtrahend);

    // These are also calculated in place, for maximum speed.
    void shift_left(int byBits);
    void shift_right(int byBits);
};

} // namespace detail
} // namespace xint

#endif // BOOST_INCLUDED_XINT_DATA_T_H
