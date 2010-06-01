
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

#ifndef BOOST_XINT_DOXYGEN_IGNORE
namespace detail {
    struct nan_t { };
}
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
    increasing the speed of the library, but making it unsafe for programs where
    an %integer object might be accessed by more than one thread. The library
    allows copy-on-write for internal functions regardless of this parameter,
    which is safe because the internal functions are single-threaded.

    \tparam Secure If \c true, the library zeros out all memory before
    deallocating it, for maximum security.
*/
template <class Alloc = std::allocator<detail::digit_t>, bool Threadsafe = true,
    bool Secure = false>
class nothrow_integer_t: public detail::digitmanager_t<Alloc, Threadsafe,
    Secure>, public any_integer
{
    public:
    typedef nothrow_integer_t<Alloc, Threadsafe, Secure> type;

    //! \name Constructors & Destructors
    //!@{
    nothrow_integer_t();
    nothrow_integer_t(const nothrow_integer_t<Alloc, Threadsafe, Secure>& b);
    nothrow_integer_t(BOOST_XINT_RV_REF(type) b): any_integer(*this, 1) {
        _swap(b); }
    explicit nothrow_integer_t(const char *str, size_t base = 10);
    explicit nothrow_integer_t(const char *str, char **endptr, size_t base =
        10);
    explicit nothrow_integer_t(const std::string& str, size_t base = 10);
    explicit nothrow_integer_t(const xint::binary_t b, size_t bits = 0);
    explicit nothrow_integer_t(const any_integer& other);
    template <typename Type> nothrow_integer_t(const Type n,
        typename boost::enable_if<boost::is_integral<Type> >::type* = 0);

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
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator=(
        BOOST_XINT_COPY_ASSIGN_REF(type) c);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator=(
        BOOST_XINT_RV_REF(type) c) { _swap(c); return *this; }
    template <typename Type> nothrow_integer_t<Alloc, Threadsafe, Secure>&
        operator=(const Type n) { nothrow_integer_t<Alloc, Threadsafe,
        Secure> nn(n); _swap(nn); return *this; }

    bool operator!() const { return data.is_zero(); }
    nothrow_integer_t<Alloc, Threadsafe, Secure> operator-() const;
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator+() { return
        *this; }
    const nothrow_integer_t<Alloc, Threadsafe, Secure>& operator+() const {
        return *this; }
    nothrow_integer_t<Alloc, Threadsafe, Secure> operator~() const;

    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator+=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> b);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator-=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> b);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator*=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> b);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator/=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> b);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator%=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> b);

    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator++();
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator--();
    nothrow_integer_t<Alloc, Threadsafe, Secure>  operator++(int);
    nothrow_integer_t<Alloc, Threadsafe, Secure>  operator--(int);

    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator&=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> n);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator|=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> n);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator^=(const
        nothrow_integer_t<Alloc, Threadsafe, Secure> n);
    nothrow_integer_t<Alloc, Threadsafe, Secure>  operator<<(size_t shift)
        const;
    nothrow_integer_t<Alloc, Threadsafe, Secure>  operator>>(size_t shift)
        const;
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator<<=(size_t shift);
    nothrow_integer_t<Alloc, Threadsafe, Secure>& operator>>=(size_t shift);
    //@}

    //! \name Miscellaneous Functions
    //!@{
    bool is_odd() const;
    bool is_even() const;
    bool is_nan() const { return _is_nan(); }
    int  sign(bool signed_zero = false) const;
    size_t hex_digits() const;
    //!@}

    typedef base_divide_t<nothrow_integer_t<Alloc, Threadsafe, Secure> >
        divide_t;

    /*! \name Static Member Functions

        These are functions that return a nothrow_integer_t, but don't take one
        as a parameter. To use them as free functions, you would have to specify
        the return type anyway, so I made them static member functions of the
        type instead.
    */
    //!@{
    static nothrow_integer_t<Alloc, Threadsafe, Secure> pow2(size_t exponent);
    static nothrow_integer_t<Alloc, Threadsafe, Secure> factorial(size_t n);
    static nothrow_integer_t<Alloc, Threadsafe, Secure> nan() { return _nan; }
    template <class Type> static nothrow_integer_t<Alloc, Threadsafe, Secure>
        random_by_size(Type& gen, size_t size_in_bits, bool high_bit_on = false,
        bool low_bit_on = false, bool can_be_negative = false);
    template <class Type> static nothrow_integer_t<Alloc, Threadsafe, Secure>
        random_prime(Type& gen, size_t size_in_bits, callback_t callback =
        no_callback);
    //!@}

    void _swap(nothrow_integer_t<Alloc, Threadsafe, Secure>& s) { using
        std::swap; swap(data, s.data); }
    static std::string _nan_text() { return "#NaN#"; }

    private:
    static nothrow_integer_t<Alloc, Threadsafe, Secure> _nan;
    BOOST_XINT_COPYABLE_AND_MOVABLE(type)
};

template <class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::_nan = nothrow_integer_t<Alloc, Threadsafe, Secure>(
    detail::nan_t());

////////////////////////////////////////////////////////////////////////////////
// Function template definitions

//! \copydoc integer_t::integer_t()
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t():
    any_integer(*this, 1)
{
    // Don't need to do anything, already preinitialized to zero.
}

//! \copydoc integer_t::integer_t(const integer_t&)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const
    nothrow_integer_t<Alloc, Threadsafe, Secure>& b): any_integer(*this, 1)
{
    try {
        data = b.data;
        if (!is_nan()) data.beginendmod();
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const std::string&, size_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const char *str,
    size_t base): any_integer(*this, 1)
{
    try {
        data.from_string(str, strlen(str), base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const char *, char **, size_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const char *str,
    char **endptr, size_t base): any_integer(*this, 1)
{
    try {
        data.from_string(str, endptr, base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const std::string&, size_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const
    std::string& str, size_t base): any_integer(*this, 1)
{
    try {
        data.from_string(str.c_str(), str.length(), base);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const xint::binary_t, size_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const
    xint::binary_t b, size_t bits): any_integer(*this, 1)
{
    try {
        data.from_binary(b, bits);
    } catch (std::exception&) {
        *this = nan();
    }
}

//! \copydoc integer_t::integer_t(const any_integer&)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>::nothrow_integer_t(const
    any_integer& c): any_integer(*this, 1)
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
template<class Alloc, bool Threadsafe, bool Secure>
template <typename Type> nothrow_integer_t<Alloc, Threadsafe,
    Secure>::nothrow_integer_t(const Type n, typename
    boost::enable_if<boost::is_integral<Type> >::type*): any_integer(*this, 1)
{
    try {
        if (std::numeric_limits<Type>::is_signed) data.set_signed(n);
        else data.set_unsigned(n);
    } catch (std::exception&) {
        *this = nan();
    }
}

template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator=(BOOST_XINT_COPY_ASSIGN_REF(type) c)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator-() const
{
    try {
        if (is_nan()) return *this;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(-data);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \see operator+(nothrow_integer_t, nothrow_integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator+=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> b)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator-=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> b)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator*=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> b)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator/=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> b)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator%=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> b)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator++()
{
    try {
        if (!is_nan()) ++data;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator--
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator--()
{
    try {
        if (!is_nan()) --data;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator++(int)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator++(int)
{
    try {
        if (is_nan()) return *this;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(data++);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \copydoc integer_t::operator--(int)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator--(int)
{
    try {
        if (is_nan()) return *this;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(data--);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nan();
    }
}

//! \see operator&(nothrow_integer_t, nothrow_integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator&=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator|=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator^=(const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n)
{
    try {
        if (!is_nan() && !n.is_nan()) data ^= n.data;
        else *this = nan();
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \copydoc integer_t::operator<<(size_t) const
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator<<(size_t shift) const
{
    try {
        if (is_nan()) return *this;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(data << shift);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

//! \copydoc integer_t::operator>>(size_t) const
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator>>(size_t shift) const
{
    try {
        if (is_nan()) return *this;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(data >> shift);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

//! \see nothrow_integer_t::operator<<(size_t) const
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator<<=(size_t shift)
{
    try {
        if (!is_nan()) data <<= shift;
    } catch (std::exception&) {
        *this = nan();
    }
    return *this;
}

//! \see nothrow_integer_t::operator>>(size_t) const
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure>& nothrow_integer_t<Alloc,
    Threadsafe, Secure>::operator>>=(size_t shift)
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
template<class Alloc, bool Threadsafe, bool Secure>
bool nothrow_integer_t<Alloc, Threadsafe, Secure>::is_odd() const {
    try {
        return (is_nan() ? false : data.is_odd());
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc integer_t::is_even

    Returns false instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
bool nothrow_integer_t<Alloc, Threadsafe, Secure>::is_even() const {
    try {
        return (is_nan() ? false : data.is_even());
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc integer_t::sign

    Returns zero instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
int nothrow_integer_t<Alloc, Threadsafe, Secure>::sign(bool signed_zero)
    const
{
    try {
        return (is_nan() ? 0 : data.sign(signed_zero));
    } catch (std::exception&) {
        return 0;
    }
}

/*! \copydoc integer_t::hex_digits

    Returns zero instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t nothrow_integer_t<Alloc, Threadsafe, Secure>::hex_digits() const {
    try {
        return (is_nan() ? 0 : data.hex_digits());
    } catch (std::exception&) {
        return 0;
    }
}

//! \copydoc integer_t::pow2
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::pow2(size_t exponent)
{
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        detail::pow2(r.data, exponent);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

//! \copydoc integer_t::factorial
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::factorial(size_t n)
{
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        detail::factorial(r.data, n);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

//! \copydoc integer_t::random_by_size
template<class Alloc, bool Threadsafe, bool Secure>
template <class Type>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::random_by_size(Type& gen, size_t size_in_bits, bool
    high_bit_on, bool low_bit_on, bool can_be_negative)
{
    try {
        detail::random_generator<Type> rgen(gen);

        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        detail::random_by_size(r._data(), rgen, size_in_bits, high_bit_on,
            low_bit_on, can_be_negative);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

//! \copydoc integer_t::random_prime
template<class Alloc, bool Threadsafe, bool Secure>
template <class Type>
nothrow_integer_t<Alloc, Threadsafe, Secure> nothrow_integer_t<Alloc,
    Threadsafe, Secure>::random_prime(Type& gen, size_t size_in_bits, callback_t
    callback)
{
    try {
        detail::random_generator<Type> rgen(gen);

        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        detail::random_prime(r._data(), rgen, size_in_bits, callback);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return nothrow_integer_t<Alloc, Threadsafe, Secure>::nan();
    }
}

////////////////////////////////////////////////////////////////////////////////
// Free function template definitions

//! \name Mathematical primitives
//!@{

//! \copydoc abs(integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> abs(const nothrow_integer_t<Alloc,
    Threadsafe, Secure> n)
{
    try {
        if (n.is_nan()) return n;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(abs(n._data()));
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

/*! \copydoc divide(integer_t, integer_t)

\note Returns two Not-a-Number values instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
typename nothrow_integer_t<Alloc, Threadsafe, Secure>::divide_t divide(const
    nothrow_integer_t<Alloc, Threadsafe, Secure> dividend, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> divisor)
{
    try {
        if (dividend.is_nan() || divisor.is_nan())
            return std::make_pair(dividend.nan(), dividend.nan());
        typename nothrow_integer_t<Alloc, Threadsafe, Secure>::divide_t r;
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> square(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n)
{
    try {
        if (n.is_nan()) return n;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        square(r._data(), n._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc pow(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> pow(const nothrow_integer_t<Alloc,
    Threadsafe, Secure> n, const nothrow_integer_t<Alloc, Threadsafe, Secure> e)
{
    try {
        if (n.is_nan() || e.is_nan()) return n.nan();
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        pow(r._data(), n._data(), e._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc sqrt(integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> sqrt(const nothrow_integer_t<Alloc,
    Threadsafe, Secure> n)
{
    try {
        if (n.is_nan()) return n;
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
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
Type to(const nothrow_integer_t<Alloc, Threadsafe, Secure> n) {
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
template<class Alloc, bool Threadsafe, bool Secure>
std::string to_string(const nothrow_integer_t<Alloc, Threadsafe, Secure> n,
    size_t base = 10, bool uppercase = false)
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
template<class Alloc, bool Threadsafe, bool Secure>
xint::binary_t to_binary(const nothrow_integer_t<Alloc, Threadsafe, Secure> n,
    size_t bits = 0)
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

/*! \copydoc getbit(const integer_t, size_t)

\note Returns false instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
bool getbit(const nothrow_integer_t<Alloc, Threadsafe, Secure> n, size_t bit) {
    try {
        if (n.is_nan()) return false;
        return getbit(n._data(), bit);
    } catch (std::exception&) {
        return false;
    }
}

/*! \copydoc setbit(integer_t&, size_t)

\note Does nothing instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
void setbit(nothrow_integer_t<Alloc, Threadsafe, Secure>& n, size_t bit) {
    try {
        if (!n.is_nan()) setbit(n._data(), bit);
    } catch (std::exception&) {
        // Do nothing.
    }
}

/*! \copydoc clearbit(integer_t&, size_t)

\note Does nothing instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
void clearbit(nothrow_integer_t<Alloc, Threadsafe, Secure>& n, size_t bit) {
    try {
        if (!n.is_nan()) clearbit(n._data(), bit);
    } catch (std::exception&) {
        // Do nothing.
    }
}

/*! \copydoc lowestbit(integer_t, size_t)

\note Returns \c return_if_zero instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t lowestbit(const nothrow_integer_t<Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
{
    try {
        if (n.is_nan()) return return_if_zero;
        return lowestbit(n._data(), return_if_zero);
    } catch (std::exception&) {
        return return_if_zero;
    }
}

/*! \copydoc highestbit(integer_t, size_t)

\note Returns \c return_if_zero instead of throwing.
*/
template<class Alloc, bool Threadsafe, bool Secure>
size_t highestbit(const nothrow_integer_t<Alloc, Threadsafe, Secure> n, size_t
    return_if_zero = 0)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> mulmod(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> by, const nothrow_integer_t<Alloc, Threadsafe, Secure> modulus)
{
    try {
        if (n.is_nan() || by.is_nan() || modulus.is_nan()) return n.nan();
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        mulmod(r._data(), n._data(), by._data(), modulus._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc sqrmod(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> sqrmod(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> modulus)
{
    try {
        if (n.is_nan() || modulus.is_nan()) return n.nan();
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        sqrmod(r._data(), n._data(), modulus._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc powmod(integer_t, integer_t, integer_t, bool)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> powmod(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> exponent, const nothrow_integer_t<Alloc, Threadsafe, Secure>
    modulus, bool no_monty)
{
    try {
        if (n.is_nan() || exponent.is_nan() || modulus.is_nan()) return n.nan();
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        powmod(r._data(), n._data(), exponent._data(), modulus._data(),
            no_monty);
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n.nan();
    }
}

//! \copydoc invmod(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> invmod(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> modulus)
{
    try {
        if (n.is_nan() || modulus.is_nan()) return n.nan();
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
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
template<class Alloc, bool Threadsafe, bool Secure>
int is_prime(const nothrow_integer_t<Alloc, Threadsafe, Secure> n, callback_t
    callback = no_callback)
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

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator<(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator<(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator>(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator>(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator<=(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator<=(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator>=(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator>=(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator==(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        return operator==(n1._data(), n2._data());
    } catch (std::exception&) {
        return false;
    }
}

template<class Alloc, bool Threadsafe, bool Secure> bool
    operator!=(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2)
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
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator+(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() + n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator-(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator-(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() - n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator*(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator*(const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() * n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator/(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator/(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> dividend, const nothrow_integer_t<Alloc,
    Threadsafe, Secure> divisor)
{
    if (dividend.is_nan() || divisor.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(dividend._data() /
            divisor._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return dividend.nan();
    }
}

//! \copydoc operator%(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator%(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() % n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator&(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator&(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() & n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator|(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator|(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() | n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}

//! \copydoc operator^(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> operator^(const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n1, const nothrow_integer_t<Alloc, Threadsafe,
    Secure> n2)
{
    if (n1.is_nan() || n2.is_nan()) return false;
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(n1._data() ^ n2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return n1.nan();
    }
}
//!@}

//! \name Miscellaneous functions
//!@{

//! \copydoc gcd(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> gcd(const nothrow_integer_t<Alloc,
    Threadsafe, Secure> num1, const nothrow_integer_t<Alloc, Threadsafe, Secure>
    num2)
{
    if (num1.is_nan() || num2.is_nan()) return num1.nan();
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        gcd(r._data(), num1._data(), num2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return num1.nan();
    }
}

//! \copydoc lcm(integer_t, integer_t)
template<class Alloc, bool Threadsafe, bool Secure>
nothrow_integer_t<Alloc, Threadsafe, Secure> lcm(const nothrow_integer_t<Alloc,
    Threadsafe, Secure> num1, const nothrow_integer_t<Alloc, Threadsafe, Secure>
    num2)
{
    if (num1.is_nan() || num2.is_nan()) return num1.nan();
    try {
        nothrow_integer_t<Alloc, Threadsafe, Secure> r;
        lcm(r._data(), num1._data(), num2._data());
        return BOOST_XINT_MOVE(r);
    } catch (std::exception&) {
        return num1.nan();
    }
}

/*! \copydoc compare(integer_t, integer_t, bool)

\note Returns 2 instead of throwing, so say that the items are not equal.
*/
template<class Alloc, bool Threadsafe, bool Secure>
int compare(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const
    nothrow_integer_t<Alloc, Threadsafe, Secure> n2, bool ignoresign = false)
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
template<class Alloc, bool Threadsafe, bool Secure>
size_t log2(const nothrow_integer_t<Alloc, Threadsafe, Secure> n) {
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
    rtype op(const nothrow_integer_t<Alloc, Threadsafe, Secure> n1, const N \
        n2) \
    { \
        return op(n1, nothrow_integer_t<Alloc, Threadsafe, Secure>(n2)); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    rtype op(const N n1, const nothrow_integer_t<Alloc, Threadsafe, Secure> \
        n2) \
    { \
        return op(nothrow_integer_t<Alloc, Threadsafe, Secure>(n1), n2); \
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
    nothrow_integer_t<Alloc, Threadsafe, Secure> op(const nothrow_integer_t< \
        Alloc, Threadsafe, Secure> n1, const N n2) \
    { \
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(op(n1, \
            nothrow_integer_t<Alloc, Threadsafe, Secure>(n2))); \
        return BOOST_XINT_MOVE(r); \
    } \
    \
    template <typename N, class Alloc, bool Threadsafe, bool Secure> \
    nothrow_integer_t<Alloc, Threadsafe, Secure> op(const N n1, const \
        nothrow_integer_t<Alloc, Threadsafe, Secure> n2) \
    { \
        nothrow_integer_t<Alloc, Threadsafe, Secure> r(op(nothrow_integer_t< \
            Alloc, Threadsafe, Secure>(n1), n2)); \
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
template <typename charT, typename traits, class Alloc, bool
    Threadsafe, bool Secure> inline std::basic_ostream<charT,traits>&
    operator<<(std::basic_ostream<charT, traits>& out, const nothrow_integer_t<
    Alloc, Threadsafe, Secure> n)
{
    if (n.is_nan()) return operator<<(out, n._nan_text());
    return operator<<(out, n._data());
}

template <typename charT, typename traits, class Alloc, bool
    Threadsafe, bool Secure> inline std::basic_istream<charT,traits>&
    operator>>(std::basic_istream<charT, traits>& in, nothrow_integer_t<Alloc,
    Threadsafe, Secure>& n)
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

template<class Alloc, bool Threadsafe, bool Secure>
inline void swap(nothrow_integer_t<Alloc, Threadsafe, Secure>& left,
    nothrow_integer_t<Alloc, Threadsafe, Secure>& right)
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
