
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
    \brief Contains the raw_integer class declaration, and miscellaneous
           definitions for member functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_RAW_INTEGER_HPP
#define BOOST_INCLUDED_XINT_RAW_INTEGER_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

#define BOOST_XINT_RAWINT_TPL template <bitsize_t Bits, bool Secure, class \
    Alloc>
#define BOOST_XINT_RAWINT raw_integer_t<Bits, Secure, Alloc>

BOOST_XINT_RAWINT_TPL
class raw_integer_t {
    public:
    typedef magnitude_manager_t<Bits, Secure, Alloc> datatype;
    typedef base_divide_t<BOOST_XINT_RAWINT> divide_t;

    size_t length;
    bool negative;

    raw_integer_t(): length(1), negative(false), changed(false) { }
    raw_integer_t(const BOOST_XINT_RAWINT& copy): length(copy.length),
        negative(copy.negative), changed(false), data(copy.data) { }
    template <bitsize_t B, bool S, class A>
    raw_integer_t(const raw_integer_t<B,S,A>& copy): length(copy.length),
        negative(copy.negative), changed(true), data(copy.data) { make_unique();
        if (length > max_length()) length = max_length();
        trim(); }
    raw_integer_t(const BOOST_XINT_RAWINT& copy, bool neg, bool allow_zero =
        false): length(copy.length), negative(neg), changed(false),
        data(copy.data) { trim(allow_zero); }
    template <typename T>
    raw_integer_t(T n, size_t prealloc = 0, typename boost::enable_if<
        boost::is_integral<T> >::type* = 0): length(1), negative(false),
        changed(false) { if (std::numeric_limits<T>::is_signed) set_signed(n,
        prealloc); else set_unsigned(n, false, prealloc); }
    raw_integer_t(const char *s, size_t base = 10): length(1), negative(false),
        changed(false) { from_string(s, base); }
    raw_integer_t(const std::string& s, size_t base = 10): length(1),
        negative(false), changed(false) { from_string(s, base); }
    raw_integer_t(const xint::binary_t b, size_t bits = 0): length(1),
        negative(false), changed(false) { from_binary(b, bits); }

    BOOST_XINT_RAWINT& operator=(const BOOST_XINT_RAWINT& set) { length =
        set.length; negative = set.negative; data = set.data; return *this; }

    digit_t *digits(size_t resize, realloc::strategy strategy = realloc::copy);
    const digit_t *digits() const { return data.digits(); }

    digit_t get_digit(size_t i) const { return (i < max_length() ? digits()[i] :
        0); }
    digit_t operator[](size_t i) { return digits()[i]; }
    digit_t operator[](size_t i) const { return get_digit(i); }
    size_t max_length() const { return data.max_length(); }
    bool same_storage(const BOOST_XINT_RAWINT n) const { return
        data.same_storage(n.data); }

    int sign() const { return negative ? -1 : is_zero() ? 0 : 1; }
    int sign(bool allow_signed_zero) const { return (!allow_signed_zero &&
        is_zero() ? 0 : negative ? -1 : 1); }
    bool is_zero() const { return (length == 1 && digits()[0] == 0); }
    bool is_odd() const { return (digits()[0] & 0x01) != 0; }
    bool is_even() const { return (digits()[0] & 0x01) == 0; }
    size_t hex_digits() const { return (log2(*this) + 3) / 4; }
    raw_integer_t abs() const { return raw_integer_t(*this, false); }
    raw_integer_t negate() const { return raw_integer_t(*this, !negative); }

    void set(int n) { set_signed(n); }
    void set_signed(boost::intmax_t n, size_t prealloc = 0);
    void set_unsigned(boost::uintmax_t n, bool neg = false, size_t prealloc =
        0);

    bool is_nan() const { return data.is_nan(); }
    void make_nan() { data.make_nan(); }

    bool is_unique() const { return data.is_unique(); }
    void make_unique() { if (!is_unique()) data.resize_and_uniquify(length); }

    void trim(bool allow_negative_zero = false);

    raw_integer_t&       operator++();    // Preincrement
    raw_integer_t&       operator--();    // Predecrement
    raw_integer_t        operator++(int); // Postincrement
    raw_integer_t        operator--(int); // Postdecrement

    bool                 operator!() const { return is_zero(); }
    raw_integer_t        operator-() const { return BOOST_XINT_RAWINT(*this,
                                                 !negative, true); }
    raw_integer_t&       operator+()       { return *this; }
    const raw_integer_t& operator+() const { return *this; }
    raw_integer_t        operator~() const; // For fixed-size types only!

    raw_integer_t&       operator+=(const raw_integer_t b);
    raw_integer_t&       operator-=(const raw_integer_t b);
    raw_integer_t&       operator*=(const raw_integer_t b);
    raw_integer_t&       operator/=(const raw_integer_t b);
    raw_integer_t&       operator%=(const raw_integer_t b);

    raw_integer_t&       operator&=(const raw_integer_t n);
    raw_integer_t&       operator|=(const raw_integer_t n);
    raw_integer_t&       operator^=(const raw_integer_t n);
    raw_integer_t&       operator<<=(size_t shift);
    raw_integer_t&       operator>>=(size_t shift);

    void from_string(const char *str, size_t base = 10);
    void from_string(const char *str, char **endptr, size_t base = 10);
    void from_string(const std::string& str, size_t base = 10);
    void from_binary(xint::binary_t b, size_t bits = 0);

    template <class GenType>
    static raw_integer_t random_by_size(GenType& gen, bitsize_t bits, bool
        high_bit_on = false, bool low_bit_on = false, bool can_be_negative =
        false);

    template <class GenType>
    static raw_integer_t random_prime(GenType& gen, size_t size_in_bits,
        callback_t callback = no_callback);
    
    void _swap(BOOST_XINT_RAWINT& i2);
    
    private:
    bool changed;
    datatype data;

    template <bitsize_t B, bool S, class A>
    friend class raw_integer_t;
};

BOOST_XINT_RAWINT_TPL
digit_t *BOOST_XINT_RAWINT::digits(size_t resize, realloc::strategy strategy) {
    data.resize_and_uniquify(resize, strategy);
    if (resize == 0 || resize > data.max_length()) resize = data.max_length();
    if (strategy != realloc::copy) {
        if (length < resize) {
            if (strategy == realloc::extend) {
                digit_t *d = data.digits(), *p = d + length, *pe = d + resize;
                while (p != pe) *p++ = 0;
            }
            length = resize;
        } else if (length > data.max_length()) length = data.max_length();
    } else if (length > data.max_length()) length = data.max_length();
    changed = true;
    return data.digits();
}

BOOST_XINT_RAWINT_TPL
void BOOST_XINT_RAWINT::set_signed(boost::intmax_t n, size_t prealloc) {
    if (n >= 0) {
        if (n <= digit_mask) {
            length = 1;
            negative = false;
            if (prealloc != 0) {
                digits(prealloc, realloc::zero)[0] = static_cast<digit_t>(n);
            } else {
                digits(1, realloc::ignore)[0] = static_cast<digit_t>(n);
            }
            trim();
        } else set_unsigned(n, false, prealloc);
    } else if (n == (std::numeric_limits<boost::intmax_t>::min)()) {
        // Have to treat the minimum number carefully, because -n is not
        // what you'd think it is.
        set_unsigned(-(n + 1), true, prealloc);
        --*this;
    } else set_unsigned(-n, true, prealloc);
}

BOOST_XINT_RAWINT_TPL
void BOOST_XINT_RAWINT::set_unsigned(boost::uintmax_t n, bool neg, size_t
    prealloc)
{
    if (n <= digit_mask) {
        length = 1;
        if (prealloc != 0) {
            digits(prealloc, realloc::zero)[0] = static_cast<digit_t>(n);
        } else {
            digits(1, realloc::ignore)[0] = static_cast<digit_t>(n);
        }
    } else {
        digit_t *d = digits((std::max)(digits_in_uintmax, prealloc),
            realloc::ignore), *i = d, *ie = i + max_length();
        while (n != 0 && i != ie) {
            *i++ = static_cast<digit_t>(n);
            n >>= bits_per_digit;
        }
        length = (i - d);
    }
    negative = neg;
    trim();
}

BOOST_XINT_RAWINT_TPL
void BOOST_XINT_RAWINT::trim(bool allow_negative_zero) {
    assert(length <= max_length());

    bool zero = false;
    if (changed) {
        data.trim();
        if (length > 1) {
            digit_t *d = data.digits(), *i = d + length - 1;
            if (*i == 0) {
                do { --i; } while (i > d && *i == 0);
                length = size_t(i - d) + 1;
                if (i == d && *i == 0) zero = true;
            }
        } else if (data.digits()[0] == 0) zero = true;
        changed = false;
    } else if (length == 1 && data.digits()[0] == 0) zero = true;
    if (zero && !allow_negative_zero) negative = false;
}

BOOST_XINT_RAWINT_TPL
void BOOST_XINT_RAWINT::_swap(BOOST_XINT_RAWINT& i2) {
    using std::swap;
    swap(length, i2.length);
    swap(negative, i2.negative);
    swap(changed, i2.changed);
    swap(data, i2.data);
}

////////////////////////////////////////////////////////////////////////////////
// Free functions

BOOST_XINT_RAWINT_TPL
void swap(BOOST_XINT_RAWINT& i1, BOOST_XINT_RAWINT& i2) {
    i1._swap(i2);
}

template <typename charT, typename traits, bitsize_t Bits, bool Secure, class \
    Alloc>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const BOOST_XINT_RAWINT n)
{
    int base=((out.flags() & std::ios::hex) ? 16
        : (out.flags() & std::ios::oct) ? 8
        : 10);
    bool upperCase=(out.flags() & std::ios::uppercase ? true : false);

    int nsign = (n.is_zero() ? 0 : n.negative ? -1 : 1);
    if ((out.flags() & std::ios::showpos) && nsign >= 0) out << "+";

    if (out.flags() & std::ios::showbase) {
        if (nsign < 0) out << "-";

        if (base == 16 && upperCase) out << "0X";
        else if (base == 16) out << "0x";
        else if (base == 8) out << "0";

        out << to_string(n.abs(), base, upperCase);
    } else {
        out << to_string(n, base, upperCase);
    }
    return out;
}

template <typename charT, typename traits, bitsize_t Bits, bool Secure, class \
    Alloc>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, BOOST_XINT_RAWINT& n)
{
    int hex=(in.flags() & std::ios::hex) ? 1 : 0;
    int dec=(in.flags() & std::ios::dec) ? 1 : 0;
    int oct=(in.flags() & std::ios::oct) ? 1 : 0;
    int count=hex+dec+oct;

    size_t base=autobase;
    if (count == 1) {
        if (hex) base = 16;
        else if (oct) base = 8;
        else base = 10;
    } else if (count > 1) base = 10;
    // else auto-base

    std::string s;
    if (in.peek()=='+') {
        in.ignore();
    } else if (in.peek()=='-') {
        in.ignore();
        s.push_back('-');
    }

    if (base==autobase) {
        if (in.peek()=='0') {
            in.ignore();
            int c=in.peek();
            if (c=='x' || c=='X') {
                in.ignore();
                base=16;
            } else base=8;
        } else base=10;
    }

    while (in) {
        int c=in.peek();
        if ((base==8 && (c>='0' && c<='7')) ||
            (base==10 && (c>='0' && c<='9')) ||
            (base==16 && ((c>='0' && c<='9') || (c>='a' && c<='f') ||
                (c>='A' && c<='F'))))
        {
            in.ignore();
            s.push_back(char(c));
        } else break;
    }

    try {
        n.from_string(s, base);
    } catch (std::exception&) {
        // Catch invalid strings
        in.setstate(std::ios::failbit);
    }

    return in;
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_RAW_INTEGER_HPP
