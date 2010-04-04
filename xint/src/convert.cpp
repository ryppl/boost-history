
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the conversion functions. Note that the to<T> function is
    not here, because it's a template function and must be defined in a header
    file.

    TODO: the to_string function could be made more efficient by using only
    doubledigit_t-sized pieces of the integer at a time, and dividing the whole
    thing by the total of the divisions done to get the digits. Same with the
    from_string function.
*/

#include "../xint.hpp"
#include "../xint_data_t.hpp"

#include <vector>
#include <algorithm>
#include <sstream>

namespace xint {

using namespace detail;

namespace {

char nToChar(int n, bool upperCase) {
    if (n < 10) return char(n+'0');
    else if (upperCase) return char((n - 10) + 'A');
    else return char((n - 10) + 'a');
}

} // namespace

std::string to_string(const integer& n, size_t base, bool upperCase) {
    if (n.nan()) return detail::nan_text;
    if (base<2 || base>36) {
        if (exceptions_allowed()) throw invalid_base();
        else return std::string();
    }

    if (n.sign()==0) return "0";

    std::ostringstream out;
    if (base==16) {
        // Special no-division version, primarily for debugging division
        const data_t *ndata=n._get_data();
        const digit_t *firstDigit=ndata->digits,
            *lastDigit=firstDigit + ndata->mLength - 1;

        const int bitsPerDigit=4;
        const digit_t mask=(doubledigit_t(1) << bitsPerDigit)-1;

        // Suppress any leading zeros
        const digit_t *d=lastDigit;
        int digitShift=(bits_per_digit - bitsPerDigit);
        while (digitShift >= 0 && ((*d >> digitShift) & mask) == 0)
            digitShift -= bitsPerDigit;

        do {
            while (digitShift >= 0) {
                out << nToChar((*d >> digitShift) & mask, upperCase);
                digitShift -= bitsPerDigit;
            }

            digitShift=(bits_per_digit - bitsPerDigit);
        } while (d-- != firstDigit);

        std::string r(n.sign() < 0 ? std::string("-")+out.str() : out.str());
        return r;
    } else {
        const integer shift=base;
        std::pair<integer, integer> a=std::make_pair(n, integer::zero());
        a.first._set_negative(false);

        integer r;
        while (a.first.sign()!=0) {
            a=divide_r(a.first, shift);
            out << nToChar(a.second._get_digit(0), upperCase);
        }

        if (n.sign() < 0) out << '-';

        std::string rval=out.str();
        std::reverse(rval.begin(), rval.end());
        return rval;
    }
}

integer from_string(const std::string& str, size_t base) {
    if (str==detail::nan_text) return integer(not_a_number());

    bool negate=false;
    const char *c=str.c_str();
    if (*c=='+') ++c;
    else if (*c=='-') { negate=true; ++c; }

    if (base==autobase) {
        if (*c=='0') {
            ++c;
            if (*c=='x' || *c=='X') {
                ++c;
                base=16;
            } else base=8;
        } else base=10;
    }

    if (base<2 || base>36) {
        if (exceptions_allowed()) throw invalid_base();
        else return integer(not_a_number());
    }

    if (*c==0) {
        if (exceptions_allowed()) throw invalid_digit("No valid digits");
        else return integer(not_a_number());
    }

    const integer shift(base);

    integer r;
    while (*c) {
        unsigned int digit;
        if (*c>='0' && *c<='9') digit=*c-'0';
        else if (*c>='A' && *c<='Z') digit=*c-'A'+10;
        else if (*c>='a' && *c<='z') digit=*c-'a'+10;
        else {
            if (exceptions_allowed()) throw invalid_digit("encountered non-alphanumeric character in string");
            else return integer(not_a_number());
        }

        if (digit >= base) {
            if (exceptions_allowed()) throw invalid_digit("encountered digit greater than base allows");
            else return integer(not_a_number());
        }

        r = (r * shift) + digit;
        ++c;
    }
    r._set_negative(negate);
    return r;
}

integer from_binary(const std::string& str) {
    const size_t bytesPerDigit=sizeof(digit_t);
    const size_t bitsPerByte=std::numeric_limits<unsigned char>::digits;

    integer r;
    detail::data_t *rdata=r._get_data();
    rdata->alloc((str.length() + bytesPerDigit - 1)/bytesPerDigit);
    digit_t *p=rdata->digits;

    unsigned char *s=(unsigned char *)str.data(), *se=s+str.length();
    while (s<se) {
        digit_t d=0;
        for (size_t i=0; i<bytesPerDigit && s<se; ++i)
            d |= (digit_t(*s++) << (i * bitsPerByte));
        *p++=d;
    }
    rdata->skipLeadingZeros();
    return r;
}

std::string to_binary(const integer& n) {
    n._throw_if_nan();

    const size_t bytesPerDigit=sizeof(digit_t);
    const size_t bitsPerByte=std::numeric_limits<unsigned char>::digits;
    std::vector<unsigned char> temp;
    temp.reserve(n._get_length() * bytesPerDigit);

    const detail::data_t *ndata=n._get_data();
    const digit_t *p=ndata->digits, *pe=p+n._get_length();
    while (p != pe) {
        digit_t d(*p++);
        for (size_t i=0; i<bytesPerDigit; ++i) {
            temp.push_back((unsigned char)(d));
            d >>= bitsPerByte;
        }
    }
    while (!temp.empty() && temp.back()==0) temp.pop_back();
    char *c=(char*)&temp[0];
    return std::string(c, c+temp.size());
}

} // namespace xint
