
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
    \brief Declares the \c xint::nothrow_integer class.
*/

#ifndef BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP
#define BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP

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

#ifdef BOOST_XINT_DOXYGEN_IGNORE
    // The documentation should see a simplified version of the functions.
    #define BOOST_XINT_INTEGER_INITIAL_TPL template<...>
    #define BOOST_XINT_INTEGER_TPL template<...>
    #define BOOST_XINT_NTINTEGER_TYPE nothrow_integer_t
#else
    namespace detail { struct nan_t { }; }

    #define BOOST_XINT_INTEGER_INITIAL_TPL template <class Alloc = \
        std::allocator<detail::digit_t>, bool Threadsafe = true, bool Secure = \
        false>
    #define BOOST_XINT_INTEGER_TPL template<class Alloc, bool Threadsafe, bool \
        Secure>
    #define BOOST_XINT_NTINTEGER_TYPE nothrow_integer_t<Alloc, Threadsafe, \
        Secure>
#endif

/*! \brief The %nothrow_integer_t class template.

    Operations using this class do not throw exceptions, they return special
    values (most often \ref nan "Not-a-Number") to indicate failures.

    There are only a few member functions; most of the functionality is
    implemented using standalone functions.

    \tparam Alloc The allocator to use. It must be for the \c
    boost::xint::detail::digit_t type.

    \tparam Threadsafe If \c true, the library ensures that each object has its
    own unique storage before returning it. If \c false, identical objects are
    allowed to share storage using a copy-on-write design, potentially
    increasing the speed of the library, but making it unsafe for multithreaded
    use (with \ref threadsafe "some exceptions"). The library allows
    copy-on-write for internal functions regardless of this parameter, which is
    safe because the internal functions are single-threaded.

    \tparam Secure If \c true, the library zeros out all memory before
    deallocating it, for maximum security.
*/
BOOST_XINT_INTEGER_INITIAL_TPL
class nothrow_integer_t: public detail::digitmanager_t<Alloc, Threadsafe,
    Secure>, public any_integer
{
    public:
    typedef BOOST_XINT_NTINTEGER_TYPE type;

    //! \name Constructors & Destructors
    //!@{
    nothrow_integer_t();
    nothrow_integer_t(const BOOST_XINT_NTINTEGER_TYPE& b, bool
        force_thread_safety = false);
    nothrow_integer_t(BOOST_XINT_RV_REF(type) b): any_integer(*this, 1) {
        _swap(b); }
    explicit nothrow_integer_t(const char *str, size_t base = 10);
    explicit nothrow_integer_t(const char *str, char **endptr, size_t base =
        10);
    explicit nothrow_integer_t(const std::string& str, size_t base = 10);
    explicit nothrow_integer_t(const xint::binary_t b, bitsize_t bits = 0);
    explicit nothrow_integer_t(const any_integer& other, bool
        force_thread_safety = false);
    template <typename Type> nothrow_integer_t(const Type n, typename
        boost::enable_if<boost::is_integral<Type> >::type* = 0);

    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    //! This one is used internally.
    nothrow_integer_t(const detail::data_t c): detail::digitmanager_t<Alloc,
        Threadsafe, Secure>(*c.holder()), any_integer(*this, c.length,
        c.negative) { data.beginendmod(); }
    nothrow_integer_t(const detail::nan_t& c): detail::digitmanager_t<Alloc,
        Threadsafe, Secure>(0), any_integer(*this, 0) { }
    #endif
    //!@}

    /*! \name Mathematical and Bitwise Operators
        \brief These operate exactly like their counterparts for the built-in
               %integer types.
    */
    //@{
    BOOST_XINT_NTINTEGER_TYPE& operator=(BOOST_XINT_COPY_ASSIGN_REF(type) c);
    BOOST_XINT_NTINTEGER_TYPE& operator=(BOOST_XINT_RV_REF(type) c) { _swap(c);
        return *this; }
    template <typename Type> BOOST_XINT_NTINTEGER_TYPE& operator=(const Type n)
        { BOOST_XINT_NTINTEGER_TYPE nn(n); _swap(nn); return *this; }

    bool operator!() const { return data.is_zero(); }
    BOOST_XINT_NTINTEGER_TYPE operator-() const;
    BOOST_XINT_NTINTEGER_TYPE& operator+() { return *this; }
    const BOOST_XINT_NTINTEGER_TYPE& operator+() const { return *this; }
    BOOST_XINT_NTINTEGER_TYPE operator~() const;

    BOOST_XINT_NTINTEGER_TYPE& operator+=(const BOOST_XINT_NTINTEGER_TYPE b);
    BOOST_XINT_NTINTEGER_TYPE& operator-=(const BOOST_XINT_NTINTEGER_TYPE b);
    BOOST_XINT_NTINTEGER_TYPE& operator*=(const BOOST_XINT_NTINTEGER_TYPE b);
    BOOST_XINT_NTINTEGER_TYPE& operator/=(const BOOST_XINT_NTINTEGER_TYPE b);
    BOOST_XINT_NTINTEGER_TYPE& operator%=(const BOOST_XINT_NTINTEGER_TYPE b);

    BOOST_XINT_NTINTEGER_TYPE& operator++();
    BOOST_XINT_NTINTEGER_TYPE& operator--();
    BOOST_XINT_NTINTEGER_TYPE  operator++(int);
    BOOST_XINT_NTINTEGER_TYPE  operator--(int);

    BOOST_XINT_NTINTEGER_TYPE& operator&=(const BOOST_XINT_NTINTEGER_TYPE n);
    BOOST_XINT_NTINTEGER_TYPE& operator|=(const BOOST_XINT_NTINTEGER_TYPE n);
    BOOST_XINT_NTINTEGER_TYPE& operator^=(const BOOST_XINT_NTINTEGER_TYPE n);
    BOOST_XINT_NTINTEGER_TYPE  operator<<(bitsize_t shift) const;
    BOOST_XINT_NTINTEGER_TYPE  operator>>(bitsize_t shift) const;
    BOOST_XINT_NTINTEGER_TYPE& operator<<=(bitsize_t shift);
    BOOST_XINT_NTINTEGER_TYPE& operator>>=(bitsize_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool is_odd() const;
    bool is_even() const;
    bool is_nan() const { return _is_nan(); }
    int  sign(bool signed_zero = false) const;
    size_t hex_digits() const;
    //!@}

    typedef base_divide_t<BOOST_XINT_NTINTEGER_TYPE> divide_t;

    /*! \name Static Member Functions

        These are functions that return a nothrow_integer_t, but don't take one
        as a parameter. To use them as free functions, you would have to specify
        the return type anyway, so I made them static member functions of the
        type instead.
    */
    //!@{
    static BOOST_XINT_NTINTEGER_TYPE pow2(size_t exponent);
    static BOOST_XINT_NTINTEGER_TYPE factorial(size_t n);
    static BOOST_XINT_NTINTEGER_TYPE nan() { return _nan; }
    template <class Type> static BOOST_XINT_NTINTEGER_TYPE random_by_size(Type&
        gen, bitsize_t size_in_bits, bool high_bit_on = false, bool low_bit_on =
        false, bool can_be_negative = false);
    template <class Type> static BOOST_XINT_NTINTEGER_TYPE random_prime(Type&
        gen, bitsize_t size_in_bits, callback_t callback = no_callback);
    //!@}

    void _swap(BOOST_XINT_NTINTEGER_TYPE& s) { using std::swap; swap(data,
        s.data); }
    static std::string _nan_text() { return "#NaN#"; }

    private:
    static BOOST_XINT_NTINTEGER_TYPE _nan;
    BOOST_XINT_COPYABLE_AND_MOVABLE(type)
};

BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::_nan =
    BOOST_XINT_NTINTEGER_TYPE(detail::nan_t());

////////////////////////////////////////////////////////////////////////////////
// Function template definitions

//! \copydoc integer_t::integer_t()
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(): any_integer(*this, 1) {
    // Don't need to do anything, already preinitialized to zero.
}

//! \copydoc integer_t::integer_t(const integer_t&, bool)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const BOOST_XINT_NTINTEGER_TYPE& b,
    bool force_thread_safety): any_integer(*this, 1)
{
    try {
        data = b.data;
        if (!is_nan()) {
            data.beginendmod();
            if (force_thread_safety && Threadsafe == false) data.make_unique();
        }
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const std::string&, size_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const char *str, size_t base):
    any_integer(*this, 1)
{
    try {
        data.from_string(str, strlen(str), base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const char *, char **, size_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const char *str, char **endptr,
    size_t base): any_integer(*this, 1)
{
    try {
        data.from_string(str, endptr, base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const std::string&, size_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const std::string& str, size_t
    base): any_integer(*this, 1)
{
    try {
        data.from_string(str.c_str(), str.length(), base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const xint::binary_t, bitsize_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const xint::binary_t b, bitsize_t
    bits): any_integer(*this, 1)
{
    try {
        data.from_binary(b, bits);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const any_integer&, bool)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const any_integer& c, bool):
    any_integer(*this, 1)
{
    if (c._data().is_nan()) {
        *this = nan();
    } else {
        try {
            data.duplicate_data(c._data());
        } catch (std::exception&) {
            *this = nan();
        }
    }
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
BOOST_XINT_INTEGER_TPL
template <typename Type> BOOST_XINT_NTINTEGER_TYPE::nothrow_integer_t(const Type
    n, typename boost::enable_if<boost::is_integral<Type> >::type*):
    any_integer(*this, 1)
{
    try {
        if (std::numeric_limits<Type>::is_signed) data.set_signed(n);
        else data.set_unsigned(n);
    } catch (std::exception&) {
        *this = nan();
    }
}

BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator=(
    BOOST_XINT_COPY_ASSIGN_REF(type) c)
{
    try {
        data = c.data;
        if (!is_nan()) data.beginendmod();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator-
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::operator-() const {
    try {
        if (is_nan()) return *this;
        BOOST_XINT_NTINTEGER_TYPE r(-data);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \see operator+(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator+=(const
    BOOST_XINT_NTINTEGER_TYPE b)
{
    try {
        if (!is_nan() && !b.is_nan()) data += b.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator-(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator-=(const
    BOOST_XINT_NTINTEGER_TYPE b)
{
    try {
        if (!is_nan() && !b.is_nan()) data -= b.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator*(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator*=(const
    BOOST_XINT_NTINTEGER_TYPE b)
{
    try {
        if (!is_nan() && !b.is_nan()) data *= b.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator/(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator/=(const
    BOOST_XINT_NTINTEGER_TYPE b)
{
    try {
        if (!is_nan() && !b.is_nan()) data /= b.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator%(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator%=(const
    BOOST_XINT_NTINTEGER_TYPE b)
{
    try {
        if (!is_nan() && !b.is_nan()) data %= b.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator++
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator++() {
    try {
        if (!is_nan()) ++data;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator--
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator--() {
    try {
        if (!is_nan()) --data;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator++(int)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::operator++(int) {
    try {
        if (is_nan()) return *this;
        BOOST_XINT_NTINTEGER_TYPE r(data++);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \copydoc integer_t::operator--(int)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::operator--(int) {
    try {
        if (is_nan()) return *this;
        BOOST_XINT_NTINTEGER_TYPE r(data--);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \see operator&(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator&=(const
    BOOST_XINT_NTINTEGER_TYPE n)
{
    try {
        if (!is_nan() && !n.is_nan()) data &= n.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator|(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator|=(const
    BOOST_XINT_NTINTEGER_TYPE n)
{
    try {
        if (!is_nan() && !n.is_nan()) data |= n.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see operator^(nothrow_integer_t, nothrow_integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator^=(const
    BOOST_XINT_NTINTEGER_TYPE n)
{
    try {
        if (!is_nan() && !n.is_nan()) data ^= n.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator<<(bitsize_t) const
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::operator<<(bitsize_t shift)
    const
{
    try {
        if (is_nan()) return *this;
        BOOST_XINT_NTINTEGER_TYPE r(data << shift);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

//! \copydoc integer_t::operator>>(bitsize_t) const
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::operator>>(bitsize_t shift)
    const
{
    try {
        if (is_nan()) return *this;
        BOOST_XINT_NTINTEGER_TYPE r(data >> shift);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

//! \see nothrow_integer_t::operator<<(bitsize_t) const
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator<<=(bitsize_t
    shift)
{
    try {
        if (!is_nan()) data <<= shift;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see nothrow_integer_t::operator>>(bitsize_t) const
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE& BOOST_XINT_NTINTEGER_TYPE::operator>>=(bitsize_t
    shift)
{
    try {
        if (!is_nan()) data >>= shift;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

/*! \copydoc integer_t::is_odd

    Returns false instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
bool BOOST_XINT_NTINTEGER_TYPE::is_odd() const {
    try {
        return (is_nan() ? false : data.is_odd());
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc integer_t::is_even

    Returns false instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
bool BOOST_XINT_NTINTEGER_TYPE::is_even() const {
    try {
        return (is_nan() ? false : data.is_even());
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc integer_t::sign

    Returns zero instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
int BOOST_XINT_NTINTEGER_TYPE::sign(bool signed_zero) const {
    try {
        return (is_nan() ? 0 : data.sign(signed_zero));
    } catch (std::exception&) {
        return 0;
    }
}

/*! \copydoc integer_t::hex_digits

    Returns zero instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
size_t BOOST_XINT_NTINTEGER_TYPE::hex_digits() const {
    try {
        return (is_nan() ? 0 : data.hex_digits());
    } catch (std::exception&) {
        return 0;
    }
}

//! \copydoc integer_t::pow2
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::pow2(size_t exponent) {
    try {
        BOOST_XINT_NTINTEGER_TYPE r;
        detail::pow2(r.data, exponent);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

//! \copydoc integer_t::factorial
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::factorial(size_t n) {
    try {
        BOOST_XINT_NTINTEGER_TYPE r;
        detail::factorial(r.data, n);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

//! \copydoc integer_t::random_by_size
BOOST_XINT_INTEGER_TPL
template <class Type>
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::random_by_size(Type& gen,
    bitsize_t size_in_bits, bool high_bit_on, bool low_bit_on, bool
    can_be_negative)
{
    try {
        detail::random_generator<Type> rgen(gen);

        BOOST_XINT_NTINTEGER_TYPE r;
        detail::random_by_size(r._data(), rgen, size_in_bits, high_bit_on,
            low_bit_on, can_be_negative);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

//! \copydoc integer_t::random_prime
BOOST_XINT_INTEGER_TPL
template <class Type>
BOOST_XINT_NTINTEGER_TYPE BOOST_XINT_NTINTEGER_TYPE::random_prime(Type& gen,
    bitsize_t size_in_bits, callback_t callback)
{
    try {
        detail::random_generator<Type> rgen(gen);

        BOOST_XINT_NTINTEGER_TYPE r;
        detail::random_prime(r._data(), rgen, size_in_bits, callback);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return BOOST_XINT_NTINTEGER_TYPE::nan();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Free function template definitions

//! \name Mathematical primitives
//!@{

//! \copydoc abs(integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE abs(const BOOST_XINT_NTINTEGER_TYPE n) {
    try {
        if (n.is_nan()) return n;
        BOOST_XINT_NTINTEGER_TYPE r(abs(n._data()));
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

/*! \copydoc divide(integer_t, integer_t)

\note Returns two Not-a-Number values instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
typename BOOST_XINT_NTINTEGER_TYPE::divide_t divide(const
    BOOST_XINT_NTINTEGER_TYPE dividend, const BOOST_XINT_NTINTEGER_TYPE divisor)
{
    try {
        if (dividend.is_nan() || divisor.is_nan())
            return std::make_pair(dividend.nan(), dividend.nan());
        typename BOOST_XINT_NTINTEGER_TYPE::divide_t r;
        divide(r.quotient._data(), r.remainder._data(), dividend._data(),
            divisor._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return std::make_pair(dividend.nan(), dividend.nan());
    }
}
//!@}

//! \name Powers and roots
//!@{

//! \copydoc square(integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE square(const BOOST_XINT_NTINTEGER_TYPE n) {
    try {
        if (n.is_nan()) return n;
        BOOST_XINT_NTINTEGER_TYPE r;
        square(r._data(), n._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc pow(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE pow(const BOOST_XINT_NTINTEGER_TYPE n, const
    BOOST_XINT_NTINTEGER_TYPE e)
{
    try {
        if (n.is_nan() || e.is_nan()) return n.nan();
        BOOST_XINT_NTINTEGER_TYPE r;
        pow(r._data(), n._data(), e._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc sqrt(integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE sqrt(const BOOST_XINT_NTINTEGER_TYPE n) {
    try {
        if (n.is_nan()) return n;
        BOOST_XINT_NTINTEGER_TYPE r;
        sqrt(r._data(), n._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}
//!@}

//! \name Conversion functions
//!@{

/*! \copydoc to(integer_t)

\note Returns T(0) instead of throwing.
*/
template <typename Type, class Alloc, bool Threadsafe, bool Secure>
Type to(const BOOST_XINT_NTINTEGER_TYPE n) {
    try {
        if (n.is_nan()) return Type(0);
        return to<Type>(n._data());
    } catch (std::exception&) {
        return Type(0);
    }
}

/*! \copydoc to_string(integer_t, size_t, bool)

\note Returns an empty string instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
std::string to_string(const BOOST_XINT_NTINTEGER_TYPE n, size_t base = 10, bool
    uppercase = false)
{
    try {
        if (n.is_nan()) return n._nan_text();
        return to_string(n._data(), base, uppercase);
    } catch (std::exception&) {
        return std::string();
    }
}

/*! \copydoc to_binary(integer_t, size_t)

\note Returns an empty \c binary_t object instead of throwing. Note that a zero
value will also return an empty \c binary_t object.
*/
BOOST_XINT_INTEGER_TPL
xint::binary_t to_binary(const BOOST_XINT_NTINTEGER_TYPE n, bitsize_t bits = 0)
{
    try {
        if (n.is_nan()) return xint::binary_t();
        return to_binary(n._data(), bits);
    } catch (std::exception&) {
        return xint::binary_t();
    }
}
//!@}

//! \name Bit-manipulation functions
//!@{

/*! \copydoc getbit(const integer_t, bitsize_t)

\note Returns false instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
bool getbit(const BOOST_XINT_NTINTEGER_TYPE n, bitsize_t bit) {
    try {
        if (n.is_nan()) return false;
        return getbit(n._data(), bit);
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc setbit(integer_t&, bitsize_t)

\note Does nothing instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
void setbit(BOOST_XINT_NTINTEGER_TYPE& n, bitsize_t bit) {
    try {
        if (!n.is_nan()) setbit(n._data(), bit);
    } catch (std::exception&) {
        // Do nothing.
    }
}

/*! \copydoc clearbit(integer_t&, bitsize_t)

\note Does nothing instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
void clearbit(BOOST_XINT_NTINTEGER_TYPE& n, bitsize_t bit) {
    try {
        if (!n.is_nan()) clearbit(n._data(), bit);
    } catch (std::exception&) {
        // Do nothing.
    }
}

/*! \copydoc lowestbit(integer_t, bitsize_t)

\note Returns \c return_if_zero instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
bitsize_t lowestbit(const BOOST_XINT_NTINTEGER_TYPE n, bitsize_t return_if_zero
    = 0)
{
    try {
        if (n.is_nan()) return return_if_zero;
        return lowestbit(n._data(), return_if_zero);
    } catch (std::exception&) {
        return return_if_zero;
    }
}

/*! \copydoc highestbit(integer_t, bitsize_t)

\note Returns \c return_if_zero instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
bitsize_t highestbit(const BOOST_XINT_NTINTEGER_TYPE n, bitsize_t return_if_zero
    = 0)
{
    try {
        if (n.is_nan()) return return_if_zero;
        return highestbit(n._data(), return_if_zero);
    } catch (std::exception&) {
        return return_if_zero;
    }
}
//!@}

//! \name Modular math functions
//!@{

//! \copydoc mulmod(integer_t, integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE mulmod(const BOOST_XINT_NTINTEGER_TYPE n, const
    BOOST_XINT_NTINTEGER_TYPE by, const BOOST_XINT_NTINTEGER_TYPE modulus)
{
    try {
        if (n.is_nan() || by.is_nan() || modulus.is_nan()) return n.nan();
        BOOST_XINT_NTINTEGER_TYPE r;
        mulmod(r._data(), n._data(), by._data(), modulus._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc sqrmod(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE sqrmod(const BOOST_XINT_NTINTEGER_TYPE n, const
    BOOST_XINT_NTINTEGER_TYPE modulus)
{
    try {
        if (n.is_nan() || modulus.is_nan()) return n.nan();
        BOOST_XINT_NTINTEGER_TYPE r;
        sqrmod(r._data(), n._data(), modulus._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc powmod(integer_t, integer_t, integer_t, bool)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE powmod(const BOOST_XINT_NTINTEGER_TYPE n, const
    BOOST_XINT_NTINTEGER_TYPE exponent, const BOOST_XINT_NTINTEGER_TYPE modulus,
    bool no_monty)
{
    try {
        if (n.is_nan() || exponent.is_nan() || modulus.is_nan()) return n.nan();
        BOOST_XINT_NTINTEGER_TYPE r;
        powmod(r._data(), n._data(), exponent._data(), modulus._data(),
            no_monty);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc invmod(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE invmod(const BOOST_XINT_NTINTEGER_TYPE n, const
    BOOST_XINT_NTINTEGER_TYPE modulus)
{
    try {
        if (n.is_nan() || modulus.is_nan()) return n.nan();
        BOOST_XINT_NTINTEGER_TYPE r;
        invmod(r._data(), n._data(), modulus._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}
//!@}

//! \name Random and prime number functions
//!@{

/*! \copydoc is_prime(integer_t, callback_t)

\note Returns -2 instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
int is_prime(const BOOST_XINT_NTINTEGER_TYPE n, callback_t callback =
    no_callback)
{
    try {
        if (n.is_nan()) return -2;
        return is_prime(n._data(), callback);
    } catch (std::exception&) {
        return -2;
    }
}
//!@}

//! \name Comparison Operators
//!@{

BOOST_XINT_INTEGER_TPL bool operator<(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator<(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

BOOST_XINT_INTEGER_TPL bool operator>(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator>(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

BOOST_XINT_INTEGER_TPL bool operator<=(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator<=(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

BOOST_XINT_INTEGER_TPL bool operator>=(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator>=(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

BOOST_XINT_INTEGER_TPL bool operator==(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator==(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

BOOST_XINT_INTEGER_TPL bool operator!=(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator!=(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}
//!@}

//! \name Mathematical and Bitwise Operators
//!@{

//! \copydoc operator+(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator+(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() + n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator-(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator-(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() - n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator*(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator*(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() * n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator/(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator/(const BOOST_XINT_NTINTEGER_TYPE dividend,
    const BOOST_XINT_NTINTEGER_TYPE divisor)
{
    if (dividend.is_nan() || divisor.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(dividend._data() /
            divisor._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return dividend.nan();
    }
}

//! \copydoc operator%(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator%(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() % n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator&(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator&(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() & n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator|(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator|(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() | n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator^(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE operator^(const BOOST_XINT_NTINTEGER_TYPE n1, const
    BOOST_XINT_NTINTEGER_TYPE n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        BOOST_XINT_NTINTEGER_TYPE r(n1._data() ^ n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}
//!@}

//! \name Miscellaneous functions
//!@{

//! \copydoc gcd(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE gcd(const BOOST_XINT_NTINTEGER_TYPE num1, const
    BOOST_XINT_NTINTEGER_TYPE num2)
{
    if (num1.is_nan() || num2.is_nan()) return num1.nan();
    try {
        BOOST_XINT_NTINTEGER_TYPE r;
        gcd(r._data(), num1._data(), num2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return num1.nan();
    }
}

//! \copydoc lcm(integer_t, integer_t)
BOOST_XINT_INTEGER_TPL
BOOST_XINT_NTINTEGER_TYPE lcm(const BOOST_XINT_NTINTEGER_TYPE num1, const
    BOOST_XINT_NTINTEGER_TYPE num2)
{
    if (num1.is_nan() || num2.is_nan()) return num1.nan();
    try {
        BOOST_XINT_NTINTEGER_TYPE r;
        lcm(r._data(), num1._data(), num2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return num1.nan();
    }
}

/*! \copydoc compare(integer_t, integer_t, bool)

\note Returns 2 instead of throwing, so say that the items are not equal.
*/
BOOST_XINT_INTEGER_TPL
int compare(const BOOST_XINT_NTINTEGER_TYPE n1, const BOOST_XINT_NTINTEGER_TYPE
    n2, bool ignoresign = false)
{
    if (n1.is_nan() || n2.is_nan()) return 2;
    try {
        return compare(n1._data(), n2._data(), ignoresign);
    } catch (std::exception&) {
        return 2;
    }
}

/*! \copydoc log2(integer_t)

\note Returns zero instead of throwing.
*/
BOOST_XINT_INTEGER_TPL
size_t log2(const BOOST_XINT_NTINTEGER_TYPE n) {
    try {
        return (n.is_nan() ? 0 : log2(n._data()));
    } catch (std::exception&) {
        return 0;
    }
}
//!@}

//! Allows for comparisons between nothrow_integer_t types and other integral
//! values.
#define BOOST_XINT_NOTHROW_ANY_COMPARE(rtype, op) \
    template <class Alloc, bool Threadsafe, bool Secure, typename N> \
    rtype op(const BOOST_XINT_NTINTEGER_TYPE n1, const N n2) { \
        return op(n1, BOOST_XINT_NTINTEGER_TYPE(n2)); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    rtype op(const N n1, const BOOST_XINT_NTINTEGER_TYPE n2) { \
        return op(BOOST_XINT_NTINTEGER_TYPE(n1), n2); \
    }

BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator<)
BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator>)
BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator<=)
BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator>=)
BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator==)
BOOST_XINT_NOTHROW_ANY_COMPARE(bool, operator!=)
BOOST_XINT_NOTHROW_ANY_COMPARE(int, compare)

//! Allows for operations between nothrow_integer_t types and other integral
//! values.
#define BOOST_XINT_NOTHROW_ANY_MATH(op) \
    template <class Alloc, bool Threadsafe, bool Secure, typename N> \
    BOOST_XINT_NTINTEGER_TYPE op(const BOOST_XINT_NTINTEGER_TYPE n1, const N \
        n2) \
    { \
        BOOST_XINT_NTINTEGER_TYPE r(op(n1, BOOST_XINT_NTINTEGER_TYPE(n2))); \
        return BOOST_XINT_MOVE(r); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    BOOST_XINT_NTINTEGER_TYPE op(const N n1, const BOOST_XINT_NTINTEGER_TYPE \
        n2) \
    { \
        BOOST_XINT_NTINTEGER_TYPE r(op(BOOST_XINT_NTINTEGER_TYPE(n1), n2)); \
        return BOOST_XINT_MOVE(r); \
    }

BOOST_XINT_NOTHROW_ANY_MATH(pow)
BOOST_XINT_NOTHROW_ANY_MATH(mulmod)
BOOST_XINT_NOTHROW_ANY_MATH(sqrmod)
BOOST_XINT_NOTHROW_ANY_MATH(powmod)
BOOST_XINT_NOTHROW_ANY_MATH(invmod)
BOOST_XINT_NOTHROW_ANY_MATH(operator+)
BOOST_XINT_NOTHROW_ANY_MATH(operator-)
BOOST_XINT_NOTHROW_ANY_MATH(operator*)
BOOST_XINT_NOTHROW_ANY_MATH(operator/)
BOOST_XINT_NOTHROW_ANY_MATH(operator%)
BOOST_XINT_NOTHROW_ANY_MATH(operator&)
BOOST_XINT_NOTHROW_ANY_MATH(operator|)
BOOST_XINT_NOTHROW_ANY_MATH(operator^)
BOOST_XINT_NOTHROW_ANY_MATH(gcd)
BOOST_XINT_NOTHROW_ANY_MATH(lcm)

//! \name Stream input/output functions
//!@{
template <typename charT, typename traits, class Alloc, bool Threadsafe, bool
    Secure> inline std::basic_ostream<charT,traits>&
    operator<<(std::basic_ostream<charT, traits>& out, const
    BOOST_XINT_NTINTEGER_TYPE n)
{
    if (n.is_nan()) return operator<<(out, n._nan_text());
    return operator<<(out, n._data());
}

template <typename charT, typename traits, class Alloc, bool Threadsafe, bool
    Secure> inline std::basic_istream<charT,traits>&
    operator>>(std::basic_istream<charT, traits>& in, BOOST_XINT_NTINTEGER_TYPE&
    n)
{
    if (in.peek()=='#') {
        // Must be either #NaN# or an error
        char buffer[6];

        // These are efficient and safe, but MSVC complains about them anyway.
        //std::streamsize size=in.readsome(buffer, 5);
        //buffer[size]=0;

        // Replacing them with these.
        char *p = buffer, *pe = p + 5;
        while (p != pe) in >> *p++;
        *p = 0;

        std::string str(buffer);

        if (str == n._nan_text()) n = n.nan();
        else in.setstate(std::ios::failbit);
        return in;
    } else {
        return operator>>(in, n._data());
    }
}
//!@}

BOOST_XINT_INTEGER_TPL
inline void swap(BOOST_XINT_NTINTEGER_TYPE& left, BOOST_XINT_NTINTEGER_TYPE&
    right)
{
    left._swap(right);
}

/*! \brief The default nothrow_integer type.

    This uses the default parameters for the nothrow_integer_t template.
*/
typedef nothrow_integer_t<> nothrow_integer;

} // namespace xint
} // namespace boost

namespace std {
template<>
class numeric_limits<boost::xint::nothrow_integer> {
    public:
    static const bool is_specialized = true;

    static boost::xint::nothrow_integer min() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer max() throw() { return 0; } // Not applicable

    static const int digits = 0; // Not applicable
    static const int digits10 = 0; // Not applicable
    static const bool is_signed = true;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static boost::xint::nothrow_integer epsilon() throw() { return 0; }
    static boost::xint::nothrow_integer round_error() throw() { return 0; }

    static const int min_exponent = 0; // Not applicable
    static const int min_exponent10 = 0; // Not applicable
    static const int max_exponent = 0; // Not applicable
    static const int max_exponent10 = 0; // Not applicable

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = true;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent; // Not applicable
    static const bool has_denorm_loss = false; // Not applicable

    static boost::xint::nothrow_integer infinity() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer quiet_NaN() throw()
        { return boost::xint::nothrow_integer::nan(); }
    static boost::xint::nothrow_integer signaling_NaN() throw() { return 0; } // Not applicable
    static boost::xint::nothrow_integer denorm_min() throw() { return 0; } // Not applicable

    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;

    static const bool traps = false; // I assume this means exceptions?
    static const bool tinyness_before = false; // Not applicable
    static const float_round_style round_style = round_indeterminate; // Not applicable
};
} // namespace std

#ifdef _WIN32
    // Return the warning setting to its original value.
    #pragma warning(pop)
#endif

#endif // BOOST_INCLUDED_XINT_NOTHROW_INTEGER_HPP
