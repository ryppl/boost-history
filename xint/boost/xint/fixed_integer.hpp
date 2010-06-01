
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
    \brief Declares the fixed-size integer type.
*/

#ifndef BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP
#define BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP

#include "detail/internals.hpp"
#include "random.hpp"

namespace boost {
namespace xint {

#ifdef _WIN32
	// The file's constructors make heavy use of the 'this' pointer, in a safe
	// manner. MSVC isn't smart enough to know that it's safe.
	#pragma warning(push)
	#pragma warning(disable: 4355)
#endif

//! @cond detail
namespace detail {
/*! This is the fixed-size version of digitmanager_t.

    I tried adding a memory cache to this, until I realized that it would need
    to be thread-local or it would break thread safety. Although I could use
    Boost.Thread to get thread-local variables, adding a dependency on a
    compiled library isn't very attractive, so I'm deferring any caching to the
    allocator. That's really a better place for it anyway.
*/
template <size_t Bits, class Alloc = std::allocator<digit_t>, bool Threadsafe =
    true, bool Secure = false>
class fixed_digitmanager_t: public digitmanager_t<Alloc, Threadsafe, Secure> {
    public:
    fixed_digitmanager_t() { }
    fixed_digitmanager_t(size_t requested_length, bool readonly = false):
        digitmanager_t<Alloc, Threadsafe, Secure>(requested_length, readonly)
        { }
    fixed_digitmanager_t(const base_digitmanager_t& copy):
        digitmanager_t<Alloc, Threadsafe, Secure>(copy) { }

    virtual base_digitmanager_t *new_of_type(base_digitmanager_t *copy = 0)
        const { if (copy != 0) return new fixed_digitmanager_t<Bits, Alloc,
        Threadsafe, Secure>(*copy); else return new fixed_digitmanager_t<Bits,
        Alloc, Threadsafe, Secure>; }
    virtual size_t fixed_length() const { return fixed; }
    virtual void endmod_hook(digit_t *p) { *(p + last_digit_index) &=
        last_digit_mask; }

    protected:
    static const size_t fixed, last_digit_index;
    static const digit_t last_digit_mask;
};

template <size_t Bits, class Alloc, bool Threadsafe, bool Secure>
const size_t fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>::fixed =
    detail::data_t::bits_to_digits(Bits);
template <size_t Bits, class Alloc, bool Threadsafe, bool Secure> const size_t
    fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>::last_digit_index =
    fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>::fixed - 1;
template <size_t Bits, class Alloc, bool Threadsafe, bool Secure> const digit_t
    fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>::last_digit_mask =
    (Bits % detail::bits_per_digit)
    ? digit_t((doubledigit_t(1) << (Bits % detail::bits_per_digit)) - 1)
    : detail::digit_mask;
//template <size_t Bits, class Alloc, bool Threadsafe, bool Secure> const digit_t
//    fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>::last_digit_mask =
//    digit_t((doubledigit_t(1) << (Bits % detail::bits_per_digit)) - 1);
} // namespace detail
//! @endcond detail

/*! \brief A fixed-size integer type.

    \tparam Bits The number of bits that the magnitude of the %fixed_integer
    type should be limited to. This can be any number greater than zero.

    \tparam Alloc The allocator to use. It must be for the \c
    boost::xint::detail::digit_t type.

    \tparam Threadsafe If \c true, the library ensures that each object has its
    own unique storage before returning it. If \c false, identical objects are
    allowed to share storage using a copy-on-write design, potentially
    increasing the speed of the library, but making it unsafe for programs where
    an %integer object might be accessed by more than one thread. The library
    allows copy-on-write for internal functions regardless of this parameter,
    which is safe because the internal functions are single-threaded.

    \tparam Secure If \c true, the library zeros out all memory before
    deallocating it, for maximum security.

%fixed_integer types are more processor-efficient than variable-sized %integers,
because they seldom need to do any allocation once they're created. However,
they can be less memory-efficient for holding smaller values. They also silently
discard the upper bits on any value that is too big for them, which can bite the
unwary.

\note The number of bits refers only to the unsigned magnitude of the number.
The sign is stored separately. This means that, for example, a \c
fixed_integer<8> has a range of -255 to +255, not -128 to +127 like an \c
int8_t.

\par
Also note that when calculating with %fixed_integer types, the number does not
"wrap around" when it exceeds the type's size like the built-in signed %integer
types do. It is truncated to the lower bits instead. For example,
<code>fixed_integer<8>(-255) - 1</code> and <code>fixed_integer<8>(255) +
1</code> will both be zero. However, the sign (on non-zero answers) is
preserved, so <code>fixed_integer<8>(-255) - 2</code> will be -1.
*/
template <size_t Bits, class Alloc = std::allocator<detail::digit_t>, bool
    Threadsafe = true, bool Secure = false>
class fixed_integer: public detail::fixed_digitmanager_t<Bits, Alloc,
    Threadsafe, Secure>, public any_integer
{
    public:
    typedef fixed_integer<Bits, Alloc, Threadsafe, Secure> type;

    //! \name Constructors & Destructors
    //!@{
    fixed_integer();
    fixed_integer(const fixed_integer<Bits, Alloc, Threadsafe, Secure>& b);
    fixed_integer(BOOST_XINT_RV_REF(type) b): any_integer(*this,1) { _swap(b); }
    explicit fixed_integer(const char *str, size_t base = 10);
    explicit fixed_integer(const char *str, char **endptr, size_t base = 10);
    explicit fixed_integer(const std::string& str, size_t base = 10);
    explicit fixed_integer(const xint::binary_t b, size_t bits = 0);
    explicit fixed_integer(const any_integer& other);
    template <typename Type> fixed_integer(const Type n,
        typename boost::enable_if<boost::is_integral<Type> >::type* = 0);

    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    //! This one is used internally.
    fixed_integer(const detail::data_t c): detail::fixed_digitmanager_t<Bits,
        Alloc, Threadsafe, Secure>(*c.holder()), any_integer(*this, c.length,
        c.negative) { data.beginendmod(); }
    #endif
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.
    */
    //@{
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator=(
        BOOST_XINT_COPY_ASSIGN_REF(type) c);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator=(
        BOOST_XINT_RV_REF(type) c) { _swap(c); return *this; }
    template <typename Type> fixed_integer<Bits, Alloc, Threadsafe, Secure>&
        operator=(const Type n) { fixed_integer<Bits, Alloc, Threadsafe,
        Secure> nn(n); _swap(nn); return *this; }

    bool operator!() const { return data.is_zero(); }
    fixed_integer<Bits, Alloc, Threadsafe, Secure> operator-() const;
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator+() { return
        *this; }
    const fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator+() const {
        return *this; }
    fixed_integer<Bits, Alloc, Threadsafe, Secure> operator~() const;

    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator+=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> b);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator-=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> b);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator*=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> b);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator/=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> b);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator%=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> b);

    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator++();
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator--();
    fixed_integer<Bits, Alloc, Threadsafe, Secure>  operator++(int);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>  operator--(int);

    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator&=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> n);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator|=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> n);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator^=(const
        fixed_integer<Bits, Alloc, Threadsafe, Secure> n);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>  operator<<(size_t shift)
        const;
    fixed_integer<Bits, Alloc, Threadsafe, Secure>  operator>>(size_t shift)
        const;
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator<<=(size_t shift);
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& operator>>=(size_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool is_odd() const;
    bool is_even() const;
    int  sign(bool signed_zero = false) const;
    size_t hex_digits() const;
    //!@}

    typedef base_divide_t<fixed_integer<Bits, Alloc, Threadsafe, Secure> >
        divide_t;

    /*! \name Static Member Functions

        These are functions that return a fixed_integer, but don't take one as a
        parameter. To use them as free functions, you would have to specify the
        return type anyway, so I made them static member functions of the type
        instead.
    */
    //!@{
    static fixed_integer<Bits, Alloc, Threadsafe, Secure> pow2(size_t exponent);
    static fixed_integer<Bits, Alloc, Threadsafe, Secure> factorial(size_t n);
    template <class Type> static fixed_integer<Bits, Alloc, Threadsafe, Secure>
        random_by_size(Type& gen, size_t size_in_bits, bool high_bit_on = false,
        bool low_bit_on = false, bool can_be_negative = false);
    template <class Type> static fixed_integer<Bits, Alloc, Threadsafe, Secure>
        random_prime(Type& gen, size_t size_in_bits, callback_t callback =
        no_callback);
    //!@}

    void _swap(fixed_integer<Bits, Alloc, Threadsafe, Secure>& s) { using
        std::swap; swap(data, s.data); }

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(type)
};

//! \copydoc integer_t::integer_t()
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer():
    any_integer(*this, 1)
{
    // Don't need to do anything, already preinitialized to zero.
}

//! \copydoc integer_t::integer_t(const integer_t&)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& b):
    detail::fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>(
    *b.data.holder()), any_integer(*this, (std::min)(b.data.length,
    fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed),
    b.data.negative)
{
    data.beginendmod();
}

/*! \copydoc integer_t::integer_t(const std::string&, size_t)

\note
This function does \e not stop when the fixed_integer's bits are full. It uses
the entire string, then truncates the result to the proper number of bits.
*/
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const char
    *str, size_t base): any_integer(*this, 1)
{
    data.from_string(str, strlen(str), base);
}

/*! \copydoc integer_t::integer_t(const char *, char **, size_t)

\note
This function does \e not stop when the fixed_integer's bits are full. It
continues as long as there are valid digits for the base in question, then
truncates the result to the proper number of bits.
*/
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const char
    *str, char **endptr, size_t base): any_integer(*this, 1)
{
    data.from_string(str, endptr, base);
}

/*! \copydoc integer_t::integer_t(const std::string&, size_t)

\note
This function does \e not stop when the fixed_integer's bits are full. It uses
the entire string, then truncates the result to the proper number of bits.
*/
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const
    std::string& str, size_t base): any_integer(*this, 1)
{
    data.from_string(str.c_str(), str.length(), base);
}

/*! \copydoc integer_t::integer_t(const xint::binary_t, size_t)

\note
This function does \e not stop when the fixed_integer's bits are full. It uses
the entire \c binary_t item, then truncates the result to the proper number of
bits.
*/
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const
    xint::binary_t b, size_t bits): any_integer(*this, 1)
{
    data.from_binary(b, bits);
}

//! \copydoc integer_t::integer_t(const any_integer&)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed_integer(const any_integer&
    c): detail::fixed_digitmanager_t<Bits, Alloc, Threadsafe, Secure>(
    *c._data().holder()), any_integer(*this, (std::min)(c._data().length,
    fixed_integer<Bits, Alloc, Threadsafe, Secure>::fixed), c._data().negative)
{
    data.beginendmod();
}

/*!
    Constructs a new integer object with the value of the built-in %integer type
    passed to it.

    \param[in] n The value for the new object.
    \tparam Type The type of \c n.

    You can ignore the \c enable_if parameter, it's there only to tell the
    compiler what types the function applies to.

    \overload
*/
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
template <typename Type> fixed_integer<Bits, Alloc, Threadsafe,
    Secure>::fixed_integer(const Type n, typename
    boost::enable_if<boost::is_integral<Type> >::type*): any_integer(*this, 1)
{
    if (std::numeric_limits<Type>::is_signed) data.set_signed(n);
    else data.set_unsigned(n);
}

template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator=(BOOST_XINT_COPY_ASSIGN_REF(type) c)
{
    data = c.data;
    data.beginendmod();
    return *this;
}

//! \copydoc integer_t::operator-
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator-() const
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(-data);
    return BOOST_XINT_MOVE(r);
}

/*! \brief The Bitwise Ones' Complement (i.e. bitwise NOT) operator.

This operator does not exist in the integer and nothrow_integer classes, because
as their lengths are not limited, it would result in an infinitely long number.
*/
template <size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator~() const
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    r.data.beginmod(this->fixed_length(), true);
    const detail::digit_t *s = data.digits(), *se = s + data.length;
    detail::digit_t *t = r.data.digits(), *te = t + this->fixed_length();
    while (s != se) *t++ = ~(*s++);
    while (t != te) *t++ = static_cast<detail::digit_t>(~0);
    r.data.length = this->fixed_length();
    r.data.endmod();
    return BOOST_XINT_MOVE(r);
}

//! \see operator+(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator+=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> b)
{
    data += b.data;
    return *this;
}

//! \see operator-(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator-=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> b)
{
    data -= b.data;
    return *this;
}

//! \see operator*(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator*=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> b)
{
    data *= b.data;
    return *this;
}

//! \see operator/(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator/=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> b)
{
    data /= b.data;
    return *this;
}

//! \see operator%(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator%=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> b)
{
    data %= b.data;
    return *this;
}

//! \copydoc integer_t::operator++
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator++()
{
    ++data;
    return *this;
}

//! \copydoc integer_t::operator--
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator--()
{
    --data;
    return *this;
}

//! \copydoc integer_t::operator++(int)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator++(int)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(data++);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc integer_t::operator--(int)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator--(int)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(data--);
    return BOOST_XINT_MOVE(r);
}

//! \see operator&(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator&=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> n)
{
    data &= n.data;
    return *this;
}

//! \see operator|(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator|=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> n)
{
    data |= n.data;
    return *this;
}

//! \see operator^(fixed_integer, fixed_integer)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator^=(const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> n)
{
    data ^= n.data;
    return *this;
}

//! \copydoc integer_t::operator<<(size_t) const
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator<<(size_t shift) const
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(data << shift);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc integer_t::operator>>(size_t) const
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator>>(size_t shift) const
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(data >> shift);
    return BOOST_XINT_MOVE(r);
}

//! \see fixed_integer::operator<<(size_t) const
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator<<=(size_t shift)
{
    data <<= shift;
    return *this;
}

//! \see fixed_integer::operator>>(size_t) const
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure>& fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::operator>>=(size_t shift)
{
    data >>= shift;
    return *this;
}

//! \copydoc integer_t::is_odd
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
bool fixed_integer<Bits, Alloc, Threadsafe, Secure>::is_odd() const {
    return data.is_odd();
}

//! \copydoc integer_t::is_even
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
bool fixed_integer<Bits, Alloc, Threadsafe, Secure>::is_even() const {
    return data.is_even();
}

//! \copydoc integer_t::sign
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
int fixed_integer<Bits, Alloc, Threadsafe, Secure>::sign(bool signed_zero)
    const
{
    return data.sign(signed_zero);
}

//! \copydoc integer_t::hex_digits
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
size_t fixed_integer<Bits, Alloc, Threadsafe, Secure>::hex_digits() const {
    return data.hex_digits();
}

//! \copydoc integer_t::pow2
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::pow2(size_t exponent)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    detail::pow2(r.data, exponent);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc integer_t::factorial
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::factorial(size_t n)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    detail::factorial(r.data, n);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc integer_t::random_by_size
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
template <class Type>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::random_by_size(Type& gen, size_t size_in_bits, bool
    high_bit_on, bool low_bit_on, bool can_be_negative)
{
    detail::random_generator<Type> rgen(gen);

    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    detail::random_by_size(r._data(), rgen, size_in_bits, high_bit_on,
        low_bit_on, can_be_negative);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc integer_t::random_prime
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
template <class Type>
fixed_integer<Bits, Alloc, Threadsafe, Secure> fixed_integer<Bits, Alloc,
    Threadsafe, Secure>::random_prime(Type& gen, size_t size_in_bits, callback_t
    callback)
{
    detail::random_generator<Type> rgen(gen);

    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    detail::random_prime(r._data(), rgen, size_in_bits, callback);
    return BOOST_XINT_MOVE(r);
}

////////////////////////////////////////////////////////////////////////////////
// Free function template definitions

//! \name Mathematical primitives
//!@{
//! \copydoc abs(integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> abs(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(abs(n._data()));
    return BOOST_XINT_MOVE(r);
}

//! \copydoc divide(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
typename fixed_integer<Bits, Alloc, Threadsafe, Secure>::divide_t divide(const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> dividend, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> divisor)
{
    typename fixed_integer<Bits, Alloc, Threadsafe, Secure>::divide_t r;
    divide(r.quotient._data(), r.remainder._data(), dividend._data(),
        divisor._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

//! \name Powers and roots
//!@{
//! \copydoc square(integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> square(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    square(r._data(), n._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc pow(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> pow(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n, const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> e)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    pow(r._data(), n._data(), e._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc sqrt(integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> sqrt(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    sqrt(r._data(), n._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

//! \name Conversion functions
//!@{
//! \copydoc to(integer_t)
template <typename Type, size_t Bits, class Alloc, bool Threadsafe, bool Secure>
Type to(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n) {
    return to<Type>(n._data());
}

//! \copydoc to_string(integer_t, size_t, bool)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
std::string to_string(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n,
    size_t base = 10, bool uppercase = false)
{
    return to_string(n._data(), base, uppercase);
}

//! \copydoc to_binary(integer_t, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
xint::binary_t to_binary(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n,
    size_t bits = 0)
{
    return to_binary(n._data(), bits);
}
//!@}

//! \name Bit-manipulation functions
//!@{
//! \copydoc getbit(const integer_t, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
bool getbit(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n, size_t bit)
{
    return getbit(n._data(), bit);
}

//! \copydoc setbit(integer_t&, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
void setbit(fixed_integer<Bits, Alloc, Threadsafe, Secure>& n, size_t bit) {
    setbit(n._data(), bit);
}

//! \copydoc clearbit(integer_t&, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
void clearbit(fixed_integer<Bits, Alloc, Threadsafe, Secure>& n, size_t bit) {
    clearbit(n._data(), bit);
}

//! \copydoc lowestbit(integer_t, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
size_t lowestbit(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
{
    return lowestbit(n._data(), return_if_zero);
}

//! \copydoc highestbit(integer_t, size_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
size_t highestbit(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
{
    return highestbit(n._data(), return_if_zero);
}
//!@}

//! \name Modular math functions
//!@{
//! \copydoc mulmod(integer_t, integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> mulmod(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n, const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> by, const fixed_integer<Bits, Alloc, Threadsafe, Secure> modulus)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    mulmod(r._data(), n._data(), by._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc sqrmod(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> sqrmod(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n, const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> modulus)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    sqrmod(r._data(), n._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc powmod(integer_t, integer_t, integer_t, bool)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> powmod(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n, const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> exponent, const fixed_integer<Bits, Alloc, Threadsafe, Secure>
    modulus, bool no_monty)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    powmod(r._data(), n._data(), exponent._data(), modulus._data(), no_monty);
    return BOOST_XINT_MOVE(r);
}

//! \copydoc invmod(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> invmod(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n, const fixed_integer<Bits, Alloc, Threadsafe,
    Secure> modulus)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    invmod(r._data(), n._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

//! \name Random and prime number functions
//!@{
//! \copydoc is_prime(integer_t, callback_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
int is_prime(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n, callback_t
    callback = no_callback)
{
    return is_prime(n._data(), callback);
}
//!@}

//! \name Comparison Operators
//!@{
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator<(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator<(n1._data(), n2._data()); }
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator>(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator>(n1._data(), n2._data()); }
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator<=(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator<=(n1._data(), n2._data()); }
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator>=(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator>=(n1._data(), n2._data()); }
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator==(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator==(n1._data(), n2._data()); }
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure> bool
    operator!=(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) { return
    operator!=(n1._data(), n2._data()); }
//!@}

//! \name Mathematical and Bitwise Operators
//!@{
//! \copydoc operator+(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator+(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() + n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator-(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator-(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() - n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator*(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator*(const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() * n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator/(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator/(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> dividend, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> divisor)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(dividend._data() /
        divisor._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator%(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator%(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() % n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator&(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator&(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() & n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator|(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator|(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() | n2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc operator^(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> operator^(const fixed_integer<
    Bits, Alloc, Threadsafe, Secure> n1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> n2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r(n1._data() ^ n2._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

//! \name Miscellaneous functions
//!@{
//! \copydoc gcd(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> gcd(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> num1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> num2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    gcd(r._data(), num1._data(), num2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc lcm(integer_t, integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
fixed_integer<Bits, Alloc, Threadsafe, Secure> lcm(const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> num1, const fixed_integer<Bits, Alloc,
    Threadsafe, Secure> num2)
{
    fixed_integer<Bits, Alloc, Threadsafe, Secure> r;
    lcm(r._data(), num1._data(), num2._data());
    return BOOST_XINT_MOVE(r);
}

//! \copydoc compare(integer_t, integer_t, bool)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
int compare(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const
    fixed_integer<Bits, Alloc, Threadsafe, Secure> n2, bool ignoresign = false)
{
    return compare(n1._data(), n2._data(), ignoresign);
}

//! \copydoc log2(integer_t)
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
size_t log2(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n) {
    return log2(n._data());
}
//!@}

//! Allows for comparisons between fixed_integer types and other integral
//! values.
#define BOOST_XINT_FIXED_ANY_COMPARE(rtype, op) \
    template <size_t Bits, class Alloc, bool Threadsafe, bool Secure, typename \
        N> \
    rtype op(const fixed_integer<Bits, Alloc, Threadsafe, Secure> n1, const N \
        n2) \
    { \
        return op(n1, fixed_integer<Bits, Alloc, Threadsafe, Secure>(n2)); \
    } \
    \
    template <typename N, size_t Bits, class Alloc, bool Threadsafe, bool \
        Secure> \
    rtype op(const N n1, const fixed_integer<Bits, Alloc, Threadsafe, Secure> \
        n2) \
    { \
        return op(fixed_integer<Bits, Alloc, Threadsafe, Secure>(n1), n2); \
    }

BOOST_XINT_FIXED_ANY_COMPARE(bool, operator<)
BOOST_XINT_FIXED_ANY_COMPARE(bool, operator>)
BOOST_XINT_FIXED_ANY_COMPARE(bool, operator<=)
BOOST_XINT_FIXED_ANY_COMPARE(bool, operator>=)
BOOST_XINT_FIXED_ANY_COMPARE(bool, operator==)
BOOST_XINT_FIXED_ANY_COMPARE(bool, operator!=)
BOOST_XINT_FIXED_ANY_COMPARE(int, compare)

//! Allows for operations between fixed_integer types and other integral values.
#define BOOST_XINT_FIXED_ANY_MATH(op) \
    template <size_t Bits, class Alloc, bool Threadsafe, bool Secure, typename \
        N> \
    fixed_integer<Bits, Alloc, Threadsafe, Secure> op(const fixed_integer<Bits,\
        Alloc, Threadsafe, Secure> n1, const N n2) \
    { \
        fixed_integer<Bits, Alloc, Threadsafe, Secure> r(op(n1, \
            fixed_integer<Bits, Alloc, Threadsafe, Secure>(n2))); \
        return BOOST_XINT_MOVE(r); \
    } \
    \
    template <typename N, size_t Bits, class Alloc, bool Threadsafe, bool \
        Secure> \
    fixed_integer<Bits, Alloc, Threadsafe, Secure> op(const N n1, const \
        fixed_integer<Bits, Alloc, Threadsafe, Secure> n2) \
    { \
        fixed_integer<Bits, Alloc, Threadsafe, Secure> r(op(fixed_integer< \
            Bits, Alloc, Threadsafe, Secure>(n1), n2)); \
        return BOOST_XINT_MOVE(r); \
    }

BOOST_XINT_FIXED_ANY_MATH(pow)
BOOST_XINT_FIXED_ANY_MATH(mulmod)
BOOST_XINT_FIXED_ANY_MATH(sqrmod)
BOOST_XINT_FIXED_ANY_MATH(powmod)
BOOST_XINT_FIXED_ANY_MATH(invmod)
BOOST_XINT_FIXED_ANY_MATH(operator+)
BOOST_XINT_FIXED_ANY_MATH(operator-)
BOOST_XINT_FIXED_ANY_MATH(operator*)
BOOST_XINT_FIXED_ANY_MATH(operator/)
BOOST_XINT_FIXED_ANY_MATH(operator%)
BOOST_XINT_FIXED_ANY_MATH(operator&)
BOOST_XINT_FIXED_ANY_MATH(operator|)
BOOST_XINT_FIXED_ANY_MATH(operator^)
BOOST_XINT_FIXED_ANY_MATH(gcd)
BOOST_XINT_FIXED_ANY_MATH(lcm)

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits, size_t Bits, class Alloc, bool
    Threadsafe, bool Secure> inline std::basic_ostream<charT,traits>&
    operator<<(std::basic_ostream<charT, traits>& out, const fixed_integer<Bits,
    Alloc, Threadsafe, Secure> n)
{
    return operator<<(out, n._data());
}

template <typename charT, typename traits, size_t Bits, class Alloc, bool
    Threadsafe, bool Secure> inline std::basic_istream<charT,traits>&
    operator>>(std::basic_istream<charT, traits>& in, fixed_integer<Bits, Alloc,
    Threadsafe, Secure>& n)
{
    return operator>>(in, n._data());
}
//!@}

template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
inline void swap(fixed_integer<Bits, Alloc, Threadsafe, Secure>& left,
    fixed_integer<Bits, Alloc, Threadsafe, Secure>& right)
{
    left._swap(right);
}

} // namespace xint
} // namespace boost

namespace std {

template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
class numeric_limits<boost::xint::fixed_integer<Bits, Alloc, Threadsafe,
    Secure> >
{
    public:
    static const bool is_specialized = true;

    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure> min()
        throw() { return -~(boost::xint::fixed_integer<Bits, Alloc, Threadsafe,
        Secure>()); }
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure> max()
        throw() { return ~(boost::xint::fixed_integer<Bits, Alloc, Threadsafe,
        Secure>()); }

    static const int digits = Bits;
    static const int digits10;
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure> epsilon()
        throw() { return 0; }
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure>
        round_error() throw() { return 0; }

    static const int min_exponent = 0; // N/A
    static const int min_exponent10 = 0; // N/A
    static const int max_exponent = 0; // N/A
    static const int max_exponent10 = 0; // N/A

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // N/A
    static const bool has_denorm_loss = false; // N/A

    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure>
        infinity() throw() { return 0; } // N/A
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure>
        quiet_NaN() throw() { return 0; } // N/A
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure>
        signaling_NaN() throw() { return 0; } // N/A
    static boost::xint::fixed_integer<Bits, Alloc, Threadsafe, Secure>
        denorm_min() throw() { return 0; } // N/A

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = true;

    static const bool traps = true; // I assume this means exceptions?
    static const bool tinyness_before = false; // N/A
    static const float_round_style round_style = round_indeterminate; // N/A
};

#ifndef BOOST_XINT_DOXYGEN_IGNORE
template<size_t Bits, class Alloc, bool Threadsafe, bool Secure>
const int numeric_limits<boost::xint::fixed_integer<Bits, Alloc, Threadsafe,
    Secure> >::digits10
    = static_cast<int>(boost::xint::detail::log10_bits(Bits));
#endif

} // namespace std

#ifdef _WIN32
	// Return the warning setting to its original value.
	#pragma warning(pop)
#endif

#endif // BOOST_INCLUDED_XINT_FIXED_INTEGER_HPP
