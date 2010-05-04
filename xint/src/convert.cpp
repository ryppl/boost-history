
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

    Note that the xint::to<T> function is not here, because it's a template
    function and must be defined in a header file.
*/

#include "../boost/xint/integer.hpp"

#include <algorithm>
#include <sstream>

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

char nToChar(int n, bool upperCase) {
    if (n < 10) return char(n+'0');
    else if (upperCase) return char((n - 10) + 'A');
    else return char((n - 10) + 'a');
}

std::string to_string(const base_integer& n, size_t base, bool uppercase) {
    if (base<2 || base>36) throw exceptions::invalid_base();

    if (n._is_zero()) return "0";

    std::ostringstream out;
    if (base == 2 || base == 4 || base == 16) {
        // Fast no-division version, useful for debugging division and for cases
        // where maximum speed is necessary.
        const digit_t *firstDigit=n._get_digits(),
            *lastDigit=firstDigit + n._get_length() - 1;

        if (n._get_negative()) out << '-';

        int bits_per_base_b_digit = 0;
        while (base > 1) { base = base >> 1; ++bits_per_base_b_digit; }
        const digit_t mask=(doubledigit_t(1) << bits_per_base_b_digit)-1;

        // Skip any leading zeros
        const digit_t *d=lastDigit;
        int digitShift=(bits_per_digit - bits_per_base_b_digit);
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
        // NOTE: for when there's nothing more pressing to do
        // This could be made a lot more efficient for very large numbers, by
        // dividing n by base raised to whatever number of digits will fit into
        // a doubledigit_t, then just doing single-digit divides for that many
        // digits before going back to n for another chunk. That would be
        // premature optimization though, so I'm just making this note instead.
        // If someone can show me a use-case where more efficiency is needed in
        // this function, I'll add it later.

        const integer shift(base);
        integer::divide_t a(n._to_integer(false), integer::zero());
        a.quotient._set_negative(false);

        integer r;
        while (!a.quotient._is_zero()) {
            a = divide_r(a.quotient, shift);
            out << nToChar(a.remainder._get_digit(0), uppercase);
        }

        if (n._get_negative()) out << '-';

        std::string rval=out.str();
        std::reverse(rval.begin(), rval.end());
        return rval;
    }
}

void from_string(base_integer& target, const std::string& str, size_t base) {
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

    if (base<2 || base>36) throw exceptions::invalid_base();
    if (*c==0) throw exceptions::invalid_digit("No valid digits");

    const integer shift(base);

    // NOTE: for when there's nothing more pressing to do
    // This function could use the same efficiency improvements that to_string
    // uses, but there's even less need for them here. Show me a use-case where
    // they're needed and I'll add them; until then, this will suffice.

    integer r;
    while (*c) {
        unsigned int digit;
        if (*c>='0' && *c<='9') digit=*c-'0';
        else if (*c>='A' && *c<='Z') digit=*c-'A'+10;
        else if (*c>='a' && *c<='z') digit=*c-'a'+10;
        else
            throw exceptions::invalid_digit("encountered non-alphanumeric "
            "character in string");

        if (digit >= base) throw exceptions::invalid_digit("encountered digit "
            "greater than base allows");

        r = (r * shift) + digit;
        ++c;
    }
    r._set_negative(negate);

    target._attach(r, true);
}

void to_binary(xint::binary_t& target, const base_integer& n, size_t bits) {
    if (bits > size_t(std::numeric_limits<unsigned char>::digits)) throw
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system");
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    bitqueue_t bitqueue;
    const digit_t *s = n._get_digits(), *se = s + n._get_length();
    while (s != se) bitqueue.push(*s++, bits_per_digit);

    while (!bitqueue.empty()) target.push_back(static_cast<unsigned char>
		(bitqueue.pop(bits)));
    while (!target.empty() && target.back()==0) target.pop_back();
}

void from_binary(base_integer& target, const xint::binary_t& b, size_t bits) {
    if (bits > size_t(std::numeric_limits<unsigned char>::digits)) throw
        exceptions::invalid_argument("can't fit that many bits into an "
        "unsigned character on this system");
    if (bits == 0) bits = std::numeric_limits<unsigned char>::digits;

    bitqueue_t bitqueue;
    for (xint::binary_t::const_iterator s = b.begin(), se = b.end(); s != se;
        ++s) bitqueue.push(*s, bits);

    size_t rlen = ((b.size() * bits) + bits_per_digit - 1) / bits_per_digit;
    temp_t r(target, rlen);
    digit_t *t = r._get_writable_digits(), *te = t + r._get_length();
    while (t != te && !bitqueue.empty())
		*t++ = static_cast<digit_t>(bitqueue.pop(bits_per_digit));

    r._cleanup();
    target._attach(r, true);
}

} // namespace detail
//! @endcond detail

/*! \brief Creates a string representation of the specified %integer.

- Complexity: O(n<sup>2</sup>)

\param[in] n The %integer to convert.
\param[in] base The base, between 2 and 36 inclusive, to convert it to. Defaults
to base 10.
\param[in] uppercase Whether to make alphabetic characters (for bases greater
than ten) uppercase or not. Defaults to \c false.

\returns The string value of \c n.

\exception exceptions::invalid_base if base is less than two or greater than 36.

\remarks
This is the function that's called when you ask the library to write an %integer
to a stream, but it's more flexible because you can specify any base between 2
and 36. (Streams only allow base-8, base-10, or base-16.)
*/
std::string to_string(const integer& n, size_t base, bool uppercase) {
    return detail::to_string(n, base, uppercase);
}

/*! \copydoc to_string(const integer&, size_t, bool)

\overload
*/
std::string to_string(const fixed_integer_any& n, size_t base, bool uppercase) {
    return detail::to_string(n, base, uppercase);
}

/*! \brief Converts a string to an integer.

- Complexity: O(n<sup>2</sup>)

\param[in] str The string to convert.
\param[in] base the base that the string representation of the number is in.
This can be any number between 2 and 36 (inclusive). It can also be the constant
xint::autobase, in which case the function will follow the standard C/C++ rules
for interpreting a numeric constant: any number with a zero as the first digit
is assumed to be base-8; any number with a leading zero-x or zero-X (such as
0x1f) is base-16, and anything else is base-10.

\returns An integer with the numeric value of the string in base \c base.

\exception exceptions::overflow_error if there is not enough free memory to
create the integer.
\exception exceptions::invalid_base if the base parameter is not between 2 and 36
(inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if the string contains any digit that cannot
be part of a number in the specified base, or if there are no valid digits.

\remarks
This is the function that's called when reading an integer from a stream, or
when contructing one from a string.
*/
integer from_string(const std::string& str, size_t base) {
    integer r;
    detail::from_string(r, str, base);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Converts a portable binary representation of a number into an
           %integer.

- Complexity: O(n)

\param[in] b An \c xint::binary_t containing the bytes to convert, lowest byte
first.
\param[in] bits The number of bits packed into each character (for portability
with systems that might not have characters with the same number of bits), or
zero (the default) to assume the number of bits in the current system's
unsigned character type.

\exception exceptions::invalid_argument if \c bits is greater than the number of
bits in an unsigned character.

\returns An %integer representing the bytes.

\see xint::to_binary
*/
integer from_binary(const xint::binary_t& b, size_t bits) {
    integer r;
    detail::from_binary(r, b, bits);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Creates a binary representation of an %integer.

- Complexity: O(n)

\param[in] n The %integer to convert.
\param[in] bits The number of bits to pack into each character (for portability
with systems that might not have characters with the same number of bits), or
zero (the default) to assume the number of bits in the current system's
unsigned character type.

\returns An \c xint::binary_t containing the binary representation, lowest byte
first.

\exception exceptions::invalid_argument if \c bits is greater than the number of
bits in an unsigned character.

\note
This function only stores the absolute value of \c n; if you need the sign, you
must store it separately.

\remarks
A binary representation is sometimes used for persistent storage or
transmission, as it is more space-efficient than a string representation.

\see xint::from_binary
\see xint::fixed_from_binary
*/
xint::binary_t to_binary(const integer& n, size_t bits) {
    xint::binary_t r;
    detail::to_binary(r, n, bits);
    return r;
}

} // namespace xint
} // namespace boost
