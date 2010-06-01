
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
    \brief Contains the conversion functions.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
    #include "bitqueue.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_INLINE char nToChar(int n, bool upperCase) {
    if (n < 10) return char(n+'0');
    else if (upperCase) return char((n - 10) + 'A');
    else return char((n - 10) + 'a');
}

BOOST_XINT_INLINE std::string to_string(const data_t n, size_t base, bool
    uppercase)
{
    if (base<2 || base>36) throw exceptions::invalid_base();

    if (n.is_zero()) return "0";

    std::ostringstream out;
    if (base == 2 || base == 4 || base == 16) {
        // Fast no-division version, useful for debugging division and for cases
        // where maximum speed is necessary.
        const digit_t *firstDigit = n.digits(),
            *lastDigit = firstDigit + n.length - 1;

        if (n.negative) out << '-';

        int bits_per_base_b_digit = 0;
        while (base > 1) { base = base >> 1; ++bits_per_base_b_digit; }
        const digit_t mask = (doubledigit_t(1) << bits_per_base_b_digit) - 1;

        // Skip any leading zeros
        const digit_t *d = lastDigit;
        int digitShift = (bits_per_digit - bits_per_base_b_digit);
        while (digitShift >= 0 && ((*d >> digitShift) & mask) == 0)
            digitShift -= bits_per_base_b_digit;

        do {
            while (digitShift >= 0) {
                out << nToChar((*d >> digitShift) & mask, uppercase);
                digitShift -= bits_per_base_b_digit;
            }

            digitShift=(bits_per_digit - bits_per_base_b_digit);
        } while (d-- != firstDigit);

        return out.str();
    } else {
        // ATTN: for when there's nothing more pressing to do
        // This could be made a lot more efficient for very large numbers, by
        // dividing n by base raised to whatever number of digits will fit into
        // a doubledigit_t, then just doing single-digit divides for that many
        // digits before going back to n for another chunk. That would be
        // premature optimization though, so I'm just making this note instead.
        // If someone can show me a use-case where more efficiency is needed in
        // this function, I'll add it later.

        const data_t shift(n.new_holder(), base);
        data_t::divide_t a(divide(n.abs(), shift));
        do {
            out << nToChar(a.remainder[0], uppercase);
            a = divide(a.quotient, shift);
        } while (!a.quotient.is_zero());
        if (!a.remainder.is_zero()) out << nToChar(a.remainder[0], uppercase);

        if (n.negative) out << '-';

        std::string rval = out.str();
        std::reverse(rval.begin(), rval.end());
        return rval;
    }
}

BOOST_XINT_INLINE void data_t::from_string(const char *str, char **endptr,
    size_t base)
{
    bool negate=false;
    const char *c = str;

    while (isspace(*c)) ++c;

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

    if (base<2 || base>36) throw exceptions::invalid_base();
    if (*c==0) throw exceptions::invalid_digit("No valid digits");

    std::string tstr;
    if (negate) tstr.push_back('-');
    if (base <= 10) {
        const char p = char('0' + base);
        while (*c >= '0' && *c < p)
            tstr.push_back(*c++);
    } else {
        const char lower = char('a' + base - 10),
			upper = char('A' + base - 10);
        while ((*c >= '0' && *c <= '9')
            || (*c >= 'a' && *c < lower)
            || (*c >= 'A' && *c < upper))
                tstr.push_back(*c++);
    }
    *endptr = const_cast<char*>(c);

    from_string(tstr, base);
}

BOOST_XINT_INLINE void data_t::from_string(const char *str, size_t slen, size_t
    base)
{
    bool negate=false;
    const char *c = str;
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

    if (base<2 || base>36) throw exceptions::invalid_base();
    if (*c==0) throw exceptions::invalid_digit("No valid digits");

    size_t max_bits_per_char = 1;
    while (max_bits_per_char < 6 && base > (1u << max_bits_per_char))
        ++max_bits_per_char;

    if (slen == 0) slen = strlen(str);
    size_t len = bits_to_digits(slen);

    const data_t shift(new_holder(), base);

    // ATTN: for when there's nothing more pressing to do
    // This function could use the same efficiency improvements that to_string
    // uses, but there's even less need for them here. Show me a use-case where
    // they're needed and I'll add them; until then, this will suffice.

    data_t digit(new_holder(), 0);

    beginmod(len);
    set(0);

    while (*c) {
        if (*c>='0' && *c<='9') digit.set(*c-'0');
        else if (*c>='A' && *c<='Z') digit.set(*c-'A'+10);
        else if (*c>='a' && *c<='z') digit.set(*c-'a'+10);
        else
            throw exceptions::invalid_digit("encountered non-alphanumeric "
            "character in string");

        if (digit >= shift) throw exceptions::invalid_digit("encountered digit "
            "greater than base allows");

        *this *= shift;
        *this += digit;
        ++c;
    }

    negative = negate;
    endmod();
}

BOOST_XINT_INLINE void data_t::from_string(const std::string& str, size_t base) {
    from_string(str.c_str(), str.length(), base);
}

BOOST_XINT_INLINE xint::binary_t to_binary(const data_t n, size_t bits) {
    if (bits > size_t(std::numeric_limits<unsigned char>::digits)) throw
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system");
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    bitqueue_t bitqueue;
    const digit_t *s = n.digits(), *se = s + n.length;
    while (s != se) bitqueue.push(*s++, bits_per_digit);

    xint::binary_t target;
    while (!bitqueue.empty()) target.push_back(static_cast<unsigned char>
      (bitqueue.pop(bits)));
    while (!target.empty() && target.back()==0) target.pop_back();
    return target;
}

BOOST_XINT_INLINE void data_t::from_binary(xint::binary_t b, size_t bits) {
    if (bits > size_t(std::numeric_limits<unsigned char>::digits)) throw
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system");
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    size_t len = bits_to_digits(b.size() * bits);
    beginmod(len);

    bitqueue_t bitqueue;
    for (xint::binary_t::const_iterator s = b.begin(), se = b.end(); s != se;
        ++s) bitqueue.push(*s, bits);

    digit_t *d = digits(), *t = d, *te = t + max_length();
    while (t != te && !bitqueue.empty())
      *t++ = static_cast<digit_t>(bitqueue.pop(bits_per_digit));

    length = (t - d);

    endmod();
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
