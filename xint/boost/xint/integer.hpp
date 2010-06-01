
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
    \brief Declares the arbitrary-length integer type.
*/

#ifndef BOOST_INCLUDED_XINT_INTEGER_HPP
#define BOOST_INCLUDED_XINT_INTEGER_HPP

#include "detail/internals.hpp"
#include "random.hpp"

#ifdef _WIN32
	// The file's constructors make heavy use of the 'this' pointer, in a safe
	// manner. MSVC isn't smart enough to know that it's safe.
	#pragma warning(push)
	#pragma warning(disable: 4355)
#endif

namespace boost {
namespace xint {

/*! \brief The integer_t class template.

    This class implements the standard aribitrary-length %integer type.

    There are only a few member functions, primarily operators. Most of the
    functionality is implemented using standalone functions.

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
*/
template <class Alloc = std::allocator<detail::digit_t>, bool Threadsafe = true,
    bool Secure = false>
class integer_t: private detail::digitmanager_t<Alloc, Threadsafe, Secure>,
    public any_integer
{
    public:
    typedef integer_t<Alloc, Threadsafe, Secure> type;

    //! \name Constructors & Destructors
    //!@{
    integer_t();
    integer_t(const integer_t<Alloc, Threadsafe, Secure>& b);
    integer_t(BOOST_XINT_RV_REF(type) b): any_integer(*this, 1) { _swap(b); }
    explicit integer_t(const char *str, size_t base = 10);
    explicit integer_t(const char *str, char **endptr, size_t base = 10);
    explicit integer_t(const std::string& str, size_t base = 10);
    explicit integer_t(const xint::binary_t b, size_t bits = 0);
    explicit integer_t(const any_integer& other);
    template <typename Type> integer_t(const Type n,
        typename boost::enable_if<boost::is_integral<Type> >::type* = 0);

    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    //! This one is used internally.
    integer_t(const detail::data_t c): detail::digitmanager_t<Alloc, Threadsafe,
        Secure>(*c.holder()), any_integer(*this, c.length, c.negative) {
        data.beginendmod(); }
    #endif
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.

        Note that there is no \c operator~. Since the integer type doesn't have
        a fixed maximum size, there is no logical way to implement it.
    */
    //@{
    integer_t<Alloc, Threadsafe, Secure>& operator=(BOOST_XINT_COPY_ASSIGN_REF(
        type) c);
    integer_t<Alloc, Threadsafe, Secure>& operator=(BOOST_XINT_RV_REF(type) c) {
        _swap(c); return *this; }
    template <typename Type> integer_t<Alloc, Threadsafe, Secure>& operator=(
        const Type n) { integer_t<Alloc, Threadsafe, Secure> nn(n); _swap(nn);
        return *this; }

    bool operator!() const { return data.is_zero(); }
    integer_t<Alloc, Threadsafe, Secure> operator-() const;
    integer_t<Alloc, Threadsafe, Secure>& operator+() { return *this; }
    const integer_t<Alloc, Threadsafe, Secure>& operator+() const { return
        *this; }

    integer_t<Alloc, Threadsafe, Secure>& operator+=(const integer_t<Alloc,
        Threadsafe, Secure> b);
    integer_t<Alloc, Threadsafe, Secure>& operator-=(const integer_t<Alloc,
        Threadsafe, Secure> b);
    integer_t<Alloc, Threadsafe, Secure>& operator*=(const integer_t<Alloc,
        Threadsafe, Secure> b);
    integer_t<Alloc, Threadsafe, Secure>& operator/=(const integer_t<Alloc,
        Threadsafe, Secure> b);
    integer_t<Alloc, Threadsafe, Secure>& operator%=(const integer_t<Alloc,
        Threadsafe, Secure> b);

    integer_t<Alloc, Threadsafe, Secure>& operator++();
    integer_t<Alloc, Threadsafe, Secure>& operator--();
    integer_t<Alloc, Threadsafe, Secure>  operator++(int);
    integer_t<Alloc, Threadsafe, Secure>  operator--(int);

    integer_t<Alloc, Threadsafe, Secure>& operator&=(const integer_t<Alloc,
        Threadsafe, Secure> n);
    integer_t<Alloc, Threadsafe, Secure>& operator|=(const integer_t<Alloc,
        Threadsafe, Secure> n);
    integer_t<Alloc, Threadsafe, Secure>& operator^=(const integer_t<Alloc,
        Threadsafe, Secure> n);
    integer_t<Alloc, Threadsafe, Secure>  operator<<(size_t shift) const;
    integer_t<Alloc, Threadsafe, Secure>  operator>>(size_t shift) const;
    integer_t<Alloc, Threadsafe, Secure>& operator<<=(size_t shift);
    integer_t<Alloc, Threadsafe, Secure>& operator>>=(size_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool is_odd() const;
    bool is_even() const;
    int  sign(bool signed_zero = false) const;
    size_t hex_digits() const;
    //!@}

    typedef base_divide_t<integer_t<Alloc, Threadsafe, Secure> > divide_t;

    /*! \name Static Member Functions

        These are functions that return an integer_t, but don't take one as a
        parameter. To use them as free functions, you would have to specify the
        return type anyway, so I made them static member functions of the type
        instead.
    */
    //!@{
    static integer_t<Alloc, Threadsafe, Secure> pow2(size_t exponent);
    static integer_t<Alloc, Threadsafe, Secure> factorial(size_t n);
    template <class Type> static integer_t<Alloc, Threadsafe, Secure>
        random_by_size(Type& gen, size_t size_in_bits, bool high_bit_on = false,
        bool low_bit_on = false, bool can_be_negative = false);
    template <class Type> static integer_t<Alloc, Threadsafe, Secure>
        random_prime(Type& gen, size_t size_in_bits, callback_t callback =
        no_callback);
    //!@}

    void _swap(integer_t<Alloc, Threadsafe, Secure>& s) { using std::swap;
        swap(data, s.data); }

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(type)
};

////////////////////////////////////////////////////////////////////////////////
// Member function template definitions

//! \brief Creates a new integer with an initial value of zero.
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(): any_integer(*this, 1) {
    // Don't need to do anything, already preinitialized to zero.
}

/*! \brief Creates a copy of an existing integer.

\param[in] b An existing integer.

\overload
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const integer_t<Alloc,
    Threadsafe, Secure>& b): detail::digitmanager_t<Alloc, Threadsafe, Secure>(
    *b.data.holder()), any_integer(*this, b.data.length, b.data.negative)
{
    data.beginendmod();
}

//! \copydoc integer_t(const std::string&, size_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const char *str, size_t base):
    any_integer(*this, 1)
{
    data.from_string(str, strlen(str), base);
}

/*! \brief Parse an integer from a string representation.

- Complexity: O(n<sup>2</sup>)

\param[in] str A string representation of a number.
\param[out] endptr When the function returns, this will contain a pointer to the
first character after the parsed integer.
\param[in] base The base that the string representation of the number is in.
This can be any number between 2 and 36 (inclusive). It can also be the constant
xint::autobase, in which case the function will follow the standard C/C++ rules
for interpreting a numeric constant: any number with a zero as the first digit
is assumed to be base-8; any number with a leading zero-x or zero-X (such as
0x1f) is base-16, and anything else is base-10.

\exception exceptions::invalid_base if the base parameter is not between 2 and
36 (inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if there are no valid digits before the
first invalid character.

\remarks
This function is based on the \c strtol function from the standard library. Like
\c strtol, it first discards any leading whitespace characters, then takes as
many characters as are valid for the given \c base and interprets them as an
integer. Then a pointer to the first character following the integer is stored
in the object pointed by \c endptr.

\overload
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const char *str, char **endptr,
    size_t base): any_integer(*this, 1)
{
    data.from_string(str, endptr, base);
}

/*! \brief Create an integer from a string representation.

- Complexity: O(n<sup>2</sup>)

\param[in] str A string representation of a number.
\param[in] base The base that the string representation of the number is in.
This can be any number between 2 and 36 (inclusive). It can also be the constant
xint::autobase, in which case the function will follow the standard C/C++ rules
for interpreting a numeric constant: any number with a zero as the first digit
is assumed to be base-8; any number with a leading zero-x or zero-X (such as
0x1f) is base-16, and anything else is base-10.

\exception exceptions::invalid_base if the base parameter is not between 2 and
36 (inclusive) or the constant xint::autobase.
\exception exceptions::invalid_digit if the string contains any digit that
cannot be part of a number in the specified base, or if there are no valid
digits.

\remarks
This is the most common way to initialize literal values that are too large to
fit into a native integral type.

\overload
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const std::string& str, size_t
    base): any_integer(*this, 1)
{
    data.from_string(str.c_str(), str.length(), base);
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

\overload
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const xint::binary_t b, size_t
    bits): any_integer(*this, 1)
{
    data.from_binary(b, bits);
}

/*! Constructs a new integer object from an existing integer object.

    Converts any existing integer_t, fixed_integer, or nothrow_integer object to
    this type.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>::integer_t(const any_integer& c):
    detail::digitmanager_t<Alloc, Threadsafe, Secure>(*c._data().holder()),
    any_integer(*this, c._data().length, c._data().negative)
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
template<class Alloc, bool Threadsafe, bool Secure>
template <typename Type> integer_t<Alloc, Threadsafe, Secure>::integer_t(const
    Type n, typename boost::enable_if<boost::is_integral<Type> >::type*):
    any_integer(*this, 1)
{
    if (std::numeric_limits<Type>::is_signed) data.set_signed(n);
    else data.set_unsigned(n);
}

template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator=(BOOST_XINT_COPY_ASSIGN_REF(type) c)
{
    data = c.data;
    data.beginendmod();
    return *this;
}

/*! \brief Return the additive inverse of an integer.

- Complexity: O(1)

\returns \c *this with the sign reversed.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::operator-() const
{
    integer_t<Alloc, Threadsafe, Secure> r(-data);
    return BOOST_XINT_MOVE(r);
}

//! \see operator+(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator+=(const integer_t<Alloc, Threadsafe, Secure> b)
{
    data += b.data;
    return *this;
}

//! \see operator-(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator-=(const integer_t<Alloc, Threadsafe, Secure> b)
{
    data -= b.data;
    return *this;
}

//! \see operator*(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator*=(const integer_t<Alloc, Threadsafe, Secure> b)
{
    data *= b.data;
    return *this;
}

//! \see operator/(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator/=(const integer_t<Alloc, Threadsafe, Secure> b)
{
    data /= b.data;
    return *this;
}

//! \see operator%(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator%=(const integer_t<Alloc, Threadsafe, Secure> b)
{
    data %= b.data;
    return *this;
}

/*! Preincrement operator

- Complexity: amortized O(1)
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator++()
{
    ++data;
    return *this;
}

/*! Predecrement operator

- Complexity: amortized O(1)
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator--()
{
    --data;
    return *this;
}

/*! Postincrement operator

- Complexity: O(n)

This is not recommended. It must make a copy before incrementing the \c *this
object, making it noticeably less efficient than the preincrement operator.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::operator++(int)
{
    integer_t<Alloc, Threadsafe, Secure> r(data++);
    return BOOST_XINT_MOVE(r);
}

/*! Postdecrement operator

- Complexity: O(n)

This is not recommended. It must make a copy before decrementing the \c *this
object, making it noticeably less efficient than the predecrement operator.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::operator--(int)
{
    integer_t<Alloc, Threadsafe, Secure> r(data--);
    return BOOST_XINT_MOVE(r);
}

//! \see operator&(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator&=(const integer_t<Alloc, Threadsafe, Secure> n)
{
    data &= n.data;
    return *this;
}

//! \see operator|(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator|=(const integer_t<Alloc, Threadsafe, Secure> n)
{
    data |= n.data;
    return *this;
}

//! \see operator^(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator^=(const integer_t<Alloc, Threadsafe, Secure> n)
{
    data ^= n.data;
    return *this;
}

/*! \brief Left-shift an integer by a specified number of bits.

- Complexity: O(n)

\param[in] shift The number of bits to left-shift the number.

\returns The bit-shifted integer.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::operator<<(size_t shift) const
{
    integer_t<Alloc, Threadsafe, Secure> r(data << shift);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Right-shift an integer by a specified number of bits.

- Complexity: O(n)

\param[in] shift The number of bits to right-shift the number.

\returns The bit-shifted integer.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::operator>>(size_t shift) const
{
    integer_t<Alloc, Threadsafe, Secure> r(data >> shift);
    return BOOST_XINT_MOVE(r);
}

//! \see operator<<(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator<<=(size_t shift)
{
    data <<= shift;
    return *this;
}

//! \see operator>>(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure>& integer_t<Alloc, Threadsafe,
    Secure>::operator>>=(size_t shift)
{
    data >>= shift;
    return *this;
}

/*! \brief Tests the lowest bit of \c *this to determine oddness.

- Complexity: O(1)

\returns \c true if \c *this is odd, otherwise \c false.
*/
template<class Alloc, bool Threadsafe, bool Secure>
bool integer_t<Alloc, Threadsafe, Secure>::is_odd() const {
    return data.is_odd();
}

/*! \brief Tests the lowest bit of \c *this to determine evenness.

- Complexity: O(1)

\returns \c true if \c *this is even, otherwise \c false.
*/
template<class Alloc, bool Threadsafe, bool Secure>
bool integer_t<Alloc, Threadsafe, Secure>::is_even() const {
    return data.is_even();
}

/*! \brief Tests the sign of \c *this.

- Complexity: O(1)

\param[in] signed_zero If \c false (the default value), returns zero if \c *this
is zero. If \c true, returns 1 or -1 on a zero \c *this as well. Primarily used
to identify a \ref zero "negative zero".

\returns -1 if \c *this is negative, 0 if it's zero, or 1 if it's greater than
zero.
*/
template<class Alloc, bool Threadsafe, bool Secure>
int integer_t<Alloc, Threadsafe, Secure>::sign(bool signed_zero) const {
    return data.sign(signed_zero);
}

/*! \brief Tells you roughly how large \c *this is.

- Complexity: O(1)

\returns The number of hexadecimal digits that would be required to encode \c
*this.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t integer_t<Alloc, Threadsafe, Secure>::hex_digits() const {
    return data.hex_digits();
}

/*! \brief Calculate the value of 2<sup>e</sup>

- Complexity: O(1)

\param[in] exponent The exponent to operate on.

\returns 2 to the power of \c exponent.

\remarks
This is a convenience function, to help with self-documenting code. It is also
more efficient than using bit-shifting or the \c pow function to get the same
result.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::pow2(size_t exponent)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    detail::pow2(r.data, exponent);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the value of \c n!

- Complexity: O(n<sup>3</sup>)

\param[in] n The value to operate on.

\returns \c n factorial, defined as <code>1*2*3*...*n</code>

\warning
Factorials get ridiculously huge, even with fairly small values of \c n. This
function, when used with a large parameter, is the easiest way to exhaust the
system's memory.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::factorial(size_t n)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    detail::factorial(r.data, n);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Generates a random integer with specific attributes.

- Complexity: O(n)

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] size_in_bits The maximum number of bits that you want the returned
number to have.
\param[in] high_bit_on If \c true, the returned number will have exactly the
requested size. If \c false, the upper bits may be zero, resulting in a number
that is slightly smaller than requested.
\param[in] low_bit_on If \c true, the returned number will always be odd. If
\c false, it has an equal chance of being odd or even.
\param[in] can_be_negative If \c true, the returned value has an equal chance
of being positive or negative. If \c false, it will always be positive.

\returns A random integer with the requested attributes.

\see \ref random
*/
template<class Alloc, bool Threadsafe, bool Secure>
template <class Type>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::random_by_size(Type& gen, size_t size_in_bits, bool high_bit_on,
    bool low_bit_on, bool can_be_negative)
{
    detail::random_generator<Type> rgen(gen);

    integer_t<Alloc, Threadsafe, Secure> r;
    detail::random_by_size(r._data(), rgen, size_in_bits, high_bit_on,
        low_bit_on, can_be_negative);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Generate a randomly-generated prime number of a particular bit-size.

- Complexity: Varies

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] size_in_bits The number of bits that you want the returned value to
have.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns A randomly-generated prime integer with the specified number of bits,
or zero if the provided callback function cancelled the operation.

\exception exceptions::invalid_argument if \c size_in_bits is less than two.

\note This function uses xint::is_prime. See the description of it for details
of its limitations.

\par
See the \link random Random Number Functions page\endlink for details on how to
get cryptographically-secure random numbers.

\see \ref primes
*/
template<class Alloc, bool Threadsafe, bool Secure>
template <class Type>
integer_t<Alloc, Threadsafe, Secure> integer_t<Alloc, Threadsafe,
    Secure>::random_prime(Type& gen, size_t size_in_bits, callback_t callback)
{
    detail::random_generator<Type> rgen(gen);

    integer_t<Alloc, Threadsafe, Secure> r;
    detail::random_prime(r._data(), rgen, size_in_bits, callback);
    return BOOST_XINT_MOVE(r);
}

////////////////////////////////////////////////////////////////////////////////
// Free function template definitions
/*! \name Mathematical primitives

    There are only a couple of functions here because almost all mathematical
    primitives are implemented as operators.
*/
//!@{

/*! \brief Returns the absolute value of an integer.

- Complexity: O(1)

\param[in] n The integer to operate on.

\returns If \c n is zero or positive, returns \c n. Otherwise returns \c -n.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> abs(const integer_t<Alloc, Threadsafe,
    Secure> n)
{
    integer_t<Alloc, Threadsafe, Secure> r(abs(n._data()));
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate how many \c dividends would fit into \c divisor, with the
           remainder.

- Complexity: O(n<sup>2</sup>)

\param[in] dividend, divisor The integers to operate on.

\returns A \c divide_t class containing the quotient and remainder of \c
dividend divided by \c divisor.

\exception exceptions::divide_by_zero if \c d2 is zero.

\see operator/(integer_t, integer_t)
\see operator%(integer_t, integer_t)
\see integer_t::operator/=
\see integer_t::operator%=
*/
template<class Alloc, bool Threadsafe, bool Secure>
typename integer_t<Alloc, Threadsafe, Secure>::divide_t divide(const
    integer_t<Alloc, Threadsafe, Secure> dividend, const integer_t<Alloc,
    Threadsafe, Secure> divisor)
{
    typename integer_t<Alloc, Threadsafe, Secure>::divide_t r;
    divide(r.quotient._data(), r.remainder._data(), dividend._data(),
        divisor._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

/*! \name Powers and roots

    \see integer_t::pow2
    \see integer_t::factorial
*/
//!@{

/*! \brief Calculate the value of n<sup>2</sup>

- Complexity: O(n(n+1)/2)

\param[in] n The integer to operate on.

\returns \c n times \c n.

This function uses a more-efficient algorithm than standard multiplication.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> square(const integer_t<Alloc, Threadsafe,
    Secure> n)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    square(r._data(), n._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the value of n<sup>e</sup>

- Complexity: O(n<sub>n</sub><sup>2</sup>n<sub>e</sub>)

\param[in] n, e The integers to operate on.

\returns \c n to the power of \c e.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> pow(const integer_t<Alloc, Threadsafe,
    Secure> n, const integer_t<Alloc, Threadsafe, Secure> e)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    pow(r._data(), n._data(), e._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the square root of \c an integer.

- Complexity: O(n<sup>2</sup>)

\param[in] n The value to operate on.

\returns The largest integer that, when squared, is less than or equal to \c n.
If \c n is not a perfect square, then this value will not be the exact square
root.

\exception exceptions::cannot_represent if \c n is negative.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> sqrt(const integer_t<Alloc, Threadsafe,
    Secure> n)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    sqrt(r._data(), n._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

/*! \name Conversion functions

    Note that these only convert an %integer type to something else. See the
    %integer constructors for conversions the other way.
*/
//!@{

/*! \brief Converts an xint::integer type to a built-in %integer type.

\param[in] n The %integer to convert.
\tparam T The type to convert it to.

\returns The numeric value of \c n, converted to the specified type.

\exception exceptions::too_big if \c n would not fit into the specified type.

This function provides the most efficient means of converting to a built-in
integral type.
*/
template <typename Type, class Alloc, bool Threadsafe, bool Secure>
Type to(const integer_t<Alloc, Threadsafe, Secure> n) {
    return to<Type>(n._data());
}

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
template<class Alloc, bool Threadsafe, bool Secure>
std::string to_string(const integer_t<Alloc, Threadsafe, Secure> n, size_t base
    = 10, bool uppercase = false)
{
    return to_string(n._data(), base, uppercase);
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
A binary representation is sometimes desirable for persistent storage or
transmission, as it is more space-efficient than a string representation.

\see integer_t::integer_t(xint::binary_t, size_t bits)
*/
template<class Alloc, bool Threadsafe, bool Secure>
xint::binary_t to_binary(const integer_t<Alloc, Threadsafe, Secure> n, size_t
    bits = 0)
{
    return to_binary(n._data(), bits);
}
//!@}

//! \name Bit-manipulation functions
//!@{

/*! \brief Queries the value of a particular bit in an integer.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] bit The zero-based index of the bit you're asking about.

\returns \c true if the specified bit is set (has a value of one), \c false if
it is clear.
*/
template<class Alloc, bool Threadsafe, bool Secure>
bool getbit(const integer_t<Alloc, Threadsafe, Secure> n, size_t bit) {
    return getbit(n._data(), bit);
}

/*! \brief Sets a specific bit in an integer to one.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
void setbit(integer_t<Alloc, Threadsafe, Secure>& n, size_t bit) {
    setbit(n._data(), bit);
}

/*! \brief Sets a specific bit in an integer to zero.

- Complexity: O(1)

\param[in,out] n The integer to operate on.
\param[in] bit The zero-based index of the bit to change.

\returns Nothing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
void clearbit(integer_t<Alloc, Threadsafe, Secure>& n, size_t bit) {
    clearbit(n._data(), bit);
}

/*! \brief Obtains the index of the lowest bit in an integer with a value of
           one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] return_if_zero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the lowest one-bit in the integer, or \c
return_if_zero if the integer contains no set bits.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t lowestbit(const integer_t<Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
{
    return lowestbit(n._data(), return_if_zero);
}

/*! \brief Obtains the index of the highest bit in an integer with a value of
           one.

- Complexity: O(1)

\param[in] n The integer to query.
\param[in] return_if_zero The value to return if \c n is zero (as there is no
correct answer in that case).

\returns The zero-based index of the highest one-bit in the integer, or \c
return_if_zero if the integer contains no set bits.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t highestbit(const integer_t<Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
{
    return highestbit(n._data(), return_if_zero);
}
//!@}

/*! \name Modular math functions

    A common reason to use a large-integer library is to implement public-key
    encryption, and the algorithms for such encryption often use modular math.

    \see operator%(integer_t, integer_t)
    \see integer_t::operator%=
*/
//!@{

/*! \brief Get the modular product of two integers.

- Complexity: O(2n<sup>2</sup>)

\param[in] n, by The integers to multiply.
\param[in] modulus The modulus.

\returns The result of \c n * \c by % \c modulus.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> mulmod(const integer_t<Alloc, Threadsafe,
    Secure> n, const integer_t<Alloc, Threadsafe, Secure> by, const
    integer_t<Alloc, Threadsafe, Secure> modulus)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    mulmod(r._data(), n._data(), by._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the modular product of squaring an integer.

- Complexity: O(2n<sup>2</sup>)

\param[in] n The integer to square.
\param[in] modulus The modulus.

\returns The result of \c square(n) % \c modulus.

\remarks
This is purely a convenience function, to make it easier to write
self-documenting code. It does not provide any additional efficiency over
writing out the calculation.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> sqrmod(const integer_t<Alloc, Threadsafe,
    Secure> n, const integer_t<Alloc, Threadsafe, Secure> modulus)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    sqrmod(r._data(), n._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the result of \c n<sup>e</sup> % \c m, keeping the intermediate
           results (relatively) small.

- Complexity: O(n<sub>n</sub><sup>2</sup>n<sub>e</sub>)

\param[in] n The integer to exponentiate.
\param[in] exponent The exponent.
\param[in] modulus The modulus.
\param[in] no_monty Used for testing, leave false for normal operation.

\returns The result of \c pow(n, \c exponent) % \c modulus.

\exception exceptions::invalid_modulus if the modulus is less than one.

\remarks
Because this function keeps the intermediate results small, it is far faster
and uses far less memory than a naive implementation.

\par
In addition, this function will use the Montgomery Reduction internally, if the
modulus is an odd number (and if \c no_monty isn't set), which is almost always
faster than the non-Montgomery method.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> powmod(const integer_t<Alloc, Threadsafe,
    Secure> n, const integer_t<Alloc, Threadsafe, Secure> exponent, const
    integer_t<Alloc, Threadsafe, Secure> modulus, bool no_monty = false)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    powmod(r._data(), n._data(), exponent._data(), modulus._data(), no_monty);
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the modular inverse of a number in a modulus, if there is one.

- Complexity: O((log n)<sup>3</sup>)

\param[in] n The number to calculate the inverse of.
\param[in] modulus The modulus to use.

\returns The modular inverse of \c n in \c modulus. If \c n has no modular
inverse in \c modulus, returns zero.

\exception exceptions::invalid_modulus if the modulus is less than one.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> invmod(const integer_t<Alloc, Threadsafe,
    Secure> n, const integer_t<Alloc, Threadsafe, Secure> modulus)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    invmod(r._data(), n._data(), modulus._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

/*! \name Random and prime number functions

    \see integer_t::random_by_size
    \see integer_t::random_prime
*/
//!@{

/*! \brief Tests an integer for primality.

- Complexity: Varies

\param[in] n The number to test.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns 1 if \c n seems to be prime, 0 if it isn't, or -1 if the provided
callback function cancelled the operation.

\exception exceptions::invalid_argument if \c n is less than 2.

\remarks
This function uses the Rabin-Miller probabilistic primality test. There is an
infinitesimally small chance that it will think that a composite number is
actually prime, but that is so small that it can be ignored for most practical
purposes. If even that chance is too much, you can run this function over the
number several times, it will use different randomly-selected "witness" numbers
each time.

\see \ref primes
*/
template<class Alloc, bool Threadsafe, bool Secure>
int is_prime(const integer_t<Alloc, Threadsafe, Secure> n, callback_t callback =
    no_callback)
{
    return is_prime(n._data(), callback);
}
//!@}

/*! \name Comparison Operators

    \see compare(integer_t, integer_t, bool)
*/
//!@{
template<class Alloc, bool Threadsafe, bool Secure> bool operator<(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator<(n1._data(), n2._data()); }
template<class Alloc, bool Threadsafe, bool Secure> bool operator>(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator>(n1._data(), n2._data()); }
template<class Alloc, bool Threadsafe, bool Secure> bool operator<=(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator<=(n1._data(), n2._data()); }
template<class Alloc, bool Threadsafe, bool Secure> bool operator>=(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator>=(n1._data(), n2._data()); }
template<class Alloc, bool Threadsafe, bool Secure> bool operator==(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator==(n1._data(), n2._data()); }
template<class Alloc, bool Threadsafe, bool Secure> bool operator!=(const
    integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe,
    Secure> n2) { return operator!=(n1._data(), n2._data()); }
//!@}

/*! \name Mathematical and Bitwise Operators

    These act exactly the same as for the built-in %integer types.
*/
//!@{

/*! \brief Calculate the sum of two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to add.

\returns The sum of the parameters.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator+(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() + n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the difference between two integers.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns The difference between the parameters.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator-(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() - n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the product of two integers.

- Complexity: O(n<sup>2</sup>)

\param[in] n1, n2 The integers to operate on.

\returns The product of the parameters.

\remarks
Automatically uses the more-efficient squaring algorithm if it can trivially
detect that the two parameters are copies of the same number.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator*(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() * n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate how many \c dividends would fit into \c divisor.

- Complexity: O(n<sup>2</sup>)

\param[in] dividend, divisor The integers to operate on.

\returns The integer value of \c dividend divided by \c divisor.

\exception exceptions::divide_by_zero if \c divisor is zero.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator/(const integer_t<Alloc,
    Threadsafe, Secure> dividend, const integer_t<Alloc, Threadsafe, Secure>
    divisor)
{
    integer_t<Alloc, Threadsafe, Secure> r(dividend._data() / divisor._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Get the remainder after dividing a number by another.

- Complexity: O(n<sup>2</sup>)

\param[in] n1 The dividend.
\param[in] n2 The divisor.

\returns The remainder after dividing \c n1 by \c n2.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator%(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() % n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Combine two integers using a bitwise \c AND operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in both parameters turned
on.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator&(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() & n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Combine two integers using a bitwise \c OR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter
turned on.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator|(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() | n2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Combine two integers using a bitwise \c XOR operation.

- Complexity: O(n)

\param[in] n1, n2 The integers to operate on.

\returns A positive integer with all bits that are set in either parameter, but
not both, turned on.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> operator^(const integer_t<Alloc,
    Threadsafe, Secure> n1, const integer_t<Alloc, Threadsafe, Secure> n2)
{
    integer_t<Alloc, Threadsafe, Secure> r(n1._data() ^ n2._data());
    return BOOST_XINT_MOVE(r);
}
//!@}

//! \name Miscellaneous functions
//!@{

/*! \brief Calculate the Greatest Common Denominator of two integers.

- Complexity: O((log n)<sup>3</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The greatest common denominator of the two integers, which will always
be a positive number.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> gcd(const integer_t<Alloc, Threadsafe,
    Secure> num1, const integer_t<Alloc, Threadsafe, Secure> num2)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    gcd(r._data(), num1._data(), num2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Calculate the Least Common Multiple of two integers.

- Complexity: O((log n)<sup>3</sup> + n<sup>2</sup>)

\param[in] num1, num2 The integers to operate on.

\returns The least common multiple of the two integers. If either integer is
zero, then the return value will be zero, by convention; in all other cases, the
return value will be a positive number.
*/
template<class Alloc, bool Threadsafe, bool Secure>
integer_t<Alloc, Threadsafe, Secure> lcm(const integer_t<Alloc, Threadsafe,
    Secure> num1, const integer_t<Alloc, Threadsafe, Secure> num2)
{
    integer_t<Alloc, Threadsafe, Secure> r;
    lcm(r._data(), num1._data(), num2._data());
    return BOOST_XINT_MOVE(r);
}

/*! \brief Compare two integer objects.

- Complexity: varies between O(n) if the numbers are equal (the worst case),
down to O(1) if they have different signs or wildly different magnitudes.

\param[in] n1, n2 The integers to compare.
\param[in] ignoresign If \c true, the absolute values of n1 and n2 are compared,
instead of their signed values. Used internally.

\returns A negative number if \c n1 < \c n2; zero if \c n1 == \c n2, or a
positive number if \c n1 > \c n2.
*/
template<class Alloc, bool Threadsafe, bool Secure>
int compare(const integer_t<Alloc, Threadsafe, Secure> n1, const integer_t<
    Alloc, Threadsafe, Secure> n2, bool ignoresign = false)
{
    return compare(n1._data(), n2._data(), ignoresign);
}

/*! \brief Get the log<sub>2</sub> value of an integer.

- Complexity: O(1)

\param[in] n The %integer to operate on.

\returns The %integer log<sub>2</sub> value of the integer.

\remarks
pow2(log2(n)-1) will give you an integer with the highest set bit of \c n,
assuming that \c n is non-zero.

\par
Similar to the #highestbit function.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t log2(const integer_t<Alloc, Threadsafe, Secure> n) {
    return log2(n._data());
}
//!@}

//! Allows for comparisons between integer_t types and other integral values.
#define BOOST_XINT_ANY_COMPARE(rtype, op) \
    template <class Alloc, bool Threadsafe, bool Secure, typename N> \
    rtype op(const integer_t<Alloc, Threadsafe, Secure> n1, const N n2) { \
        return op(n1, integer_t<Alloc, Threadsafe, Secure>(n2)); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    rtype op(const N n1, const integer_t<Alloc, Threadsafe, Secure> n2) { \
        return op(integer_t<Alloc, Threadsafe, Secure>(n1), n2); \
    }

BOOST_XINT_ANY_COMPARE(bool, operator<)
BOOST_XINT_ANY_COMPARE(bool, operator>)
BOOST_XINT_ANY_COMPARE(bool, operator<=)
BOOST_XINT_ANY_COMPARE(bool, operator>=)
BOOST_XINT_ANY_COMPARE(bool, operator==)
BOOST_XINT_ANY_COMPARE(bool, operator!=)
BOOST_XINT_ANY_COMPARE(int, compare)

//! Allows for operations between integer_t types and other integral values.
#define BOOST_XINT_ANY_MATH(op) \
    template <class Alloc, bool Threadsafe, bool Secure, typename N> \
    integer_t<Alloc, Threadsafe, Secure> op(const integer_t<Alloc, Threadsafe, \
        Secure> n1, const N n2) \
    { \
        integer_t<Alloc, Threadsafe, Secure> r(op(n1, integer_t<Alloc, \
            Threadsafe, Secure>(n2))); \
        return BOOST_XINT_MOVE(r); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    integer_t<Alloc, Threadsafe, Secure> op(const N n1, const integer_t<Alloc, \
        Threadsafe, Secure> n2) \
    { \
        integer_t<Alloc, Threadsafe, Secure> r(op(integer_t<Alloc, Threadsafe, \
            Secure>(n1), n2)); \
        return BOOST_XINT_MOVE(r); \
    }

BOOST_XINT_ANY_MATH(pow)
BOOST_XINT_ANY_MATH(mulmod)
BOOST_XINT_ANY_MATH(sqrmod)
BOOST_XINT_ANY_MATH(powmod)
BOOST_XINT_ANY_MATH(invmod)
BOOST_XINT_ANY_MATH(operator+)
BOOST_XINT_ANY_MATH(operator-)
BOOST_XINT_ANY_MATH(operator*)
BOOST_XINT_ANY_MATH(operator/)
BOOST_XINT_ANY_MATH(operator%)
BOOST_XINT_ANY_MATH(operator&)
BOOST_XINT_ANY_MATH(operator|)
BOOST_XINT_ANY_MATH(operator^)
BOOST_XINT_ANY_MATH(gcd)
BOOST_XINT_ANY_MATH(lcm)

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits, class Alloc, bool Threadsafe, bool
    Secure> inline std::basic_ostream<charT,traits>& operator<<(
    std::basic_ostream<charT, traits>& out, const integer_t<Alloc, Threadsafe,
    Secure> n)
{
    return operator<<(out, n._data());
}

template <typename charT, typename traits, class Alloc, bool Threadsafe, bool
    Secure> inline std::basic_istream<charT,traits>& operator>>(
    std::basic_istream<charT, traits>& in, integer_t<Alloc, Threadsafe, Secure>&
    n)
{
    return operator>>(in, n._data());
}
//!@}

template<class Alloc, bool Threadsafe, bool Secure>
inline void swap(integer_t<Alloc, Threadsafe, Secure>& left, integer_t<Alloc,
    Threadsafe, Secure>& right)
{
    left._swap(right);
}

/*! \brief The default arbitrary-length integer type.

    This uses the default parameters for the integer_t template.
*/
typedef integer_t<> integer;

} // namespace xint
} // namespace boost

namespace std {
template<>
class numeric_limits<boost::xint::integer> {
    public:
    static const bool is_specialized = true;

    static boost::xint::integer min() throw() { return 0; } // Not applicable
    static boost::xint::integer max() throw() { return 0; } // Not applicable

    static const int digits = 0; // Not applicable
    static const int digits10 = 0; // Not applicable
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::integer epsilon() throw() { return 0; }
    static boost::xint::integer round_error() throw() { return 0; }

    static const int min_exponent = 0; // Not applicable
    static const int min_exponent10 = 0; // Not applicable
    static const int max_exponent = 0; // Not applicable
    static const int max_exponent10 = 0; // Not applicable

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // Not applicable
    static const bool has_denorm_loss = false; // Not applicable

    static boost::xint::integer infinity() throw() { return 0; } // Not applicable
    static boost::xint::integer quiet_NaN() throw() { return 0; } // Not applicable
    static boost::xint::integer signaling_NaN() throw() { return 0; } // Not applicable
    static boost::xint::integer denorm_min() throw() { return 0; } // Not applicable

    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;

    static const bool traps = true; // I assume this means exceptions?
    static const bool tinyness_before = false; // Not applicable
    static const float_round_style round_style = round_indeterminate; // Not applicable
};
} // namespace std

#ifdef _WIN32
	// Return the warning setting to its original value.
	#pragma warning(pop)
#endif

#endif // BOOST_INCLUDED_XINT_INTEGER_HPP
