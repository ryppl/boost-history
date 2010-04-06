
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
    \brief Contains the definitions for xint::detail::data_t member functions.
*/

#include "../boost/xint/xint.hpp"
#include "../boost/xint/xint_data_t.hpp"

#include <cassert>

namespace boost {
namespace xint {
namespace detail {

data_t::data_t(digit_t initial1, digit_t initial2, digit_t initial3) {
    mLength=1;
    mAllocated=QuickDigits::count;
    digits=mQuickDigits;
    digits[0]=initial1;
    digits[1]=initial2;
    digits[2]=initial3;
    mCopies=0;
    mIsNegative=false;
}

//! \overload
data_t::data_t(data_t *c) {
    mLength=c->mLength;
    if (c->digits == c->mQuickDigits) {
        digits=mQuickDigits;
        mAllocated=QuickDigits::count;
    } else {
        try {
            mAllocated=mLength;
            #ifdef XINT_SECURE
                digits=new digit_t[mAllocated];
            #else
                mStorage.resize(mAllocated, 0);
                digits=&mStorage[0];
            #endif
        } catch (std::bad_alloc&) {
            throw xint::overflow_error("Out of memory allocating xint::integer");
        }
    }
    memcpy(digits, c->digits, mLength*sizeof(digit_t));

    mCopies=0;
    mIsNegative=c->mIsNegative;
}

#ifdef XINT_SECURE
data_t::~data_t() {
    zero(mQuickDigits, QuickDigits::count);
    if (digits != mQuickDigits) {
        zero(digits, mAllocated);
        delete[] digits;
    }
}
#endif

/*!
    Call this after every manipulation of a data_t object that could possibly
    leave leading zeros in the integer.
*/
void data_t::skipLeadingZeros() {
    digit_t *d=digits+mLength-1;
    while (d>digits && *d==0) --d;
    mLength=int((d-digits)+1);
    if (mLength==1 && digits[0]==0) mIsNegative=false; // Zero isn't negative
}

void data_t::quickset(digit_t d1, digit_t d2, digit_t d3) {
    assert(mCopies==1);

    mLength=3;
    digits[0]=d1;
    digits[1]=d2;
    digits[2]=d3;
    mIsNegative=false;
    skipLeadingZeros();
}

void data_t::alloc(size_t newcount, bool copydigits) {
    if (digits==mQuickDigits && newcount<=QuickDigits::count) {
        if (!copydigits) zero(digits, QuickDigits::count);
        else zero(digits+mLength, (newcount-mLength));
        mLength=newcount;
    } else if (newcount < mAllocated) {
        if (copydigits) {
            if (newcount>mLength) zero(digits+mLength, newcount-mLength);
            mLength=newcount;
        } else {
            mLength=newcount;
            zero(digits, mLength);
        }
    } else {
        #ifdef XINT_SECURE
            int e=newcount;
            newcount=1;
            while (newcount < e) newcount <<= 1;
        #endif

        if (digits==mQuickDigits && copydigits) {
            try {
                #ifdef XINT_SECURE
                    digits=new digit_t[newcount];
                #else
                    mStorage.resize(newcount);
                    digits=&mStorage[0];
                #endif
            } catch (std::bad_alloc&) {
                digits=mQuickDigits; // Might allow for recovery in some cases
                throw xint::overflow_error("Out of memory allocating xint::integer");
            }

            memcpy(digits, mQuickDigits, mLength*sizeof(digit_t));
            zero(digits+mLength, newcount-mLength);
        } else {
            #ifdef XINT_SECURE
                digit_t *newDigits=0;
                try {
                    newDigits=new digit_t[newcount];
                } catch (std::bad_alloc&) {
                    throw xint::overflow_error("Out of memory allocating xint::integer");
                }

                if (copydigits) {
                    memcpy(newDigits, digits, mLength*sizeof(digit_t));
                    zero(newDigits+mLength, newcount-mLength);
                } else zero(newDigits, newcount);

                if (digits!=mQuickDigits) {
                    zero(digits, mAllocated);
                    delete[] digits;
                }
                digits=newDigits;
            #else
                try {
                    mStorage.resize(newcount);
                } catch (std::bad_alloc&) {
                    throw xint::overflow_error("Out of memory allocating xint::integer");
                }
                digits=&mStorage[0];
                if (!copydigits) zero(digits, newcount);
            #endif
        }
        mLength=mAllocated=newcount;
    }
}

void data_t::copy(const data_t *c, size_t extraDigits) {
    alloc(c->mLength+extraDigits);

    mLength=c->mLength;
    if (c->mLength==1) *digits = *c->digits;
    else memcpy(digits, c->digits, mLength*sizeof(digit_t));

    mIsNegative=c->mIsNegative;

    // Deliberately doesn't call skipLeadingZeros().
}

void data_t::zero(digit_t *p, size_t count) {
    digit_t *pe=p+count;
    while (p<pe) *p++=0;
}

void data_t::invert() {
    assert(mCopies==1);

    mIsNegative=!mIsNegative;

    digit_t *d=digits, *e=d+mLength;
    while (d<e) {
        *d=static_cast<digit_t>(digit_overflowbit - 1 - *d);
        ++d;
    }

    d=digits;
    while (d<e) {
        doubledigit_t w=(*d)+1;
        (*d++)=static_cast<digit_t>(w);
        if (w<digit_overflowbit) break;
    }

    skipLeadingZeros();
}

void data_t::negate() {
    assert(mCopies==1);

    // If it's zero, don't change the sign
    if (mLength>1 || digits[0]!=0) mIsNegative=!mIsNegative;
}

void data_t::add(const data_t& addend) {
    assert(mCopies==1);
    assert(mLength >= addend.mLength);

    // The answer to any addition problem contains, at most, one digit more
    // than the largest addend.
    realloc(mLength+1);

    // Now add the digits, starting at the least-significant digit.
    digit_t carry=0;
    size_t x=0;
    for (; x<addend.mLength; ++x) {
        doubledigit_t t=doubledigit_t(digits[x])+addend.digits[x]+carry;
        if (t>=digit_overflowbit) { carry=1; t-=digit_overflowbit; } else carry=0;
        digits[x]=static_cast<digit_t>(t);
    }

    while (carry) {
        doubledigit_t t=doubledigit_t(digits[x])+1;
        if (t>=digit_overflowbit) { carry=1; t-=digit_overflowbit; } else carry=0;
        digits[x]=static_cast<digit_t>(t);
        ++x;
    }

    skipLeadingZeros();
}

void data_t::subtract(const data_t& subtrahend) {
    assert(mCopies==1);
    assert(mLength >= subtrahend.mLength);

    // For subtraction, the answer will always be less than or equal to the
    // size of the longest operand, so we've already got enough room.

    // Now subtract the digits, starting at the least-significant one.
    size_t x;
    int borrow=0;
    doubledigit_t t;
    for (x=0; x<subtrahend.mLength; ++x) {
        t=(digits[x]+digit_overflowbit)-subtrahend.digits[x]-borrow;
        if (t<digit_overflowbit) borrow=1; else { borrow=0; t-=digit_overflowbit; }
        digits[x]=static_cast<digit_t>(t);
    }

    for (; x<mLength && borrow; ++x) {
        t=(digits[x]+digit_overflowbit)-borrow;
        if (t<digit_overflowbit) borrow=1; else { borrow=0; t-=digit_overflowbit; }
        digits[x]=static_cast<digit_t>(t);
    }

    if (borrow) negate();
    skipLeadingZeros();
}

void data_t::shift_left(size_t byBits) {
    assert(mCopies==1);
    assert(byBits>0);

    size_t bytes=byBits / bits_per_digit, bits=byBits % bits_per_digit;
    size_t oldLength=mLength;

    realloc(mLength+bytes+1);

    if (bits != 0) {
        // Handle both bits and bytes in one pass
        digit_t *s=digits+oldLength-1, *t=s+bytes+1;

        *t-- = *s >> (bits_per_digit - bits);
        while (s > digits) {
            *t = (*s-- << bits);
            *t-- |= (*s >> (bits_per_digit - bits));
        }
        *t = (*s << bits);

        if (bytes != 0) memset(digits, 0, sizeof(digit_t) * bytes);
    } else if (bytes != 0) {
        memmove(digits+bytes, digits, sizeof(digit_t) * oldLength);
        memset(digits, 0, sizeof(digit_t) * bytes);
    }
    skipLeadingZeros();
}

void data_t::shift_right(size_t byBits) {
    assert(mCopies==1);
    assert(byBits>0);

    size_t bytes=byBits / bits_per_digit, bits=byBits % bits_per_digit,
        bits2 = bits_per_digit - bits;

    if (bytes >= mLength) {
        // Right-shift it into oblivion.
        mLength=1;
        *digits=0;
    } else if (bits != 0) {
        // Handle both bits and bytes in one pass
        digit_t *t=digits, *s=digits+bytes, *se=digits+mLength-1;
        while (s!=se) {
            *t = (*s++ >> bits);
            *t++ |= (*s << bits2);
        }
        *t = (*s >> bits);
        if (bytes != 0) {
            memset(digits+mLength-bytes, 0, sizeof(digit_t) * bytes);
            mLength-=bytes;
        }
        skipLeadingZeros();
    } else if (bytes != 0) {
        memmove(digits, digits + bytes, sizeof(digit_t) * (mLength - bytes));
        memset(digits + mLength - bytes, 0, sizeof(digit_t) * bytes);
        mLength-=bytes;
        skipLeadingZeros();
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
