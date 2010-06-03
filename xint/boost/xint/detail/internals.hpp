
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
    \brief Contains the internal definitions required for all XInt types.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_INTERNALS_HPP
#define BOOST_INCLUDED_XINT_INTERNALS_HPP

// Leave this commented out to use it as a header-only library.
//#define BOOST_XINT_COMPILED_LIB

#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <cstddef> // for size_t
#include <cassert>
#include <boost/integer.hpp>
#include <boost/cstdint.hpp>
#include <boost/function.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

// Uncomment this to use Boost.Move, which can improve the speed when using
// extremely large numbers.
//#define BOOST_XINT_USE_MOVE

#ifdef BOOST_XINT_USE_MOVE
    #include <boost/move/move.hpp>
    #define BOOST_XINT_COPY_ASSIGN_REF(type) BOOST_COPY_ASSIGN_REF(type)
    #define BOOST_XINT_COPYABLE_AND_MOVABLE(type) BOOST_COPYABLE_AND_MOVABLE(type)
    #define BOOST_XINT_RV_REF(type) BOOST_RV_REF(type)
    #define BOOST_XINT_MOVE(value) boost::move(value)
    #define BOOST_XINT_FORWARD(type, value) boost::forward<type>(value)
#else
    template <typename T> class disable_boost_rv_ref: public T { };
    #define BOOST_XINT_COPY_ASSIGN_REF(type) const type&
    #define BOOST_XINT_COPYABLE_AND_MOVABLE(type)
    #define BOOST_XINT_RV_REF(type) disable_boost_rv_ref<type>
    #define BOOST_XINT_MOVE(value) value
    #define BOOST_XINT_FORWARD(type, value) value
#endif

namespace boost {

//! \brief All the types and functions of the library are within this namespace.
namespace xint {

//! \brief A callback function takes no parameters and returns a bool.
typedef boost::function<bool ()> callback_t;

//! \brief An empty callback_t, used as a default parameter for some functions.
const callback_t no_callback;

//! \brief The format for the binary representations of %integers.
typedef std::vector<unsigned char> binary_t;

//! \brief A value that can be used for the 'base' parameter of the
//!        string-to-integer constructor functions.
const size_t autobase = (std::numeric_limits<size_t>::max)();

//! \brief Holds the results from a division-with-remainder call.
template <class T>
class base_divide_t {
    public:
    base_divide_t() { }
    base_divide_t(const T& q, const T& r): quotient(q), remainder(r) { }
    base_divide_t(const base_divide_t<T>& copy):
        quotient(copy.quotient), remainder(copy.remainder) { }
    base_divide_t(BOOST_XINT_RV_REF(base_divide_t) move) {
        quotient._swap(move.quotient); remainder._swap(move.remainder); }

    base_divide_t& operator=(BOOST_XINT_COPY_ASSIGN_REF(base_divide_t) copy)
        { quotient = copy.quotient; remainder = copy.remainder; return *this; }
    base_divide_t& operator=(BOOST_XINT_RV_REF(base_divide_t) move) {
        using std::swap;
        swap(quotient, move.quotient);
        swap(remainder, move.remainder);
        return *this; }

    T quotient, remainder;

    private:
    BOOST_XINT_COPYABLE_AND_MOVABLE(base_divide_t)
};

//! \brief The items within this namespace are meant for internal use only.
namespace detail {
//! @cond detail

/*!
    This is the type used for calculations. It should be the largest
    built-in integral type the system can offer, for maximum processing
    efficiency.
*/
typedef boost::uintmax_t doubledigit_t;

/*!
    This is the type used to store information about the %integer. It must
    contain at least half the number of bits that a doubledigit_t can hold,
    and is most memory-efficient when it's exactly that.
*/
typedef boost::uint_t<std::numeric_limits<doubledigit_t>::digits / 2>::fast
    digit_t;

//! \name Some constants used by the library.
//!@{
const size_t        minimum_digits    = 1;
const size_t        bits_per_digit    = std::numeric_limits<digit_t>::digits;
const size_t        digits_in_uintmax =
    (std::numeric_limits<boost::uintmax_t>::digits + bits_per_digit - 1) /
    bits_per_digit;
const digit_t       digit_hibit       = (digit_t(1) << (bits_per_digit-1));
const doubledigit_t doubledigit_hibit = (doubledigit_t(1) << (bits_per_digit*2-1));
const doubledigit_t digit_overflowbit = (doubledigit_t(1) << bits_per_digit);
const digit_t       digit_mask        = digit_t(digit_overflowbit-1);
//!@}

////////////////////////////////////////////////////////////////////////////////

//! Holds the magnitude digits of an integer. Uses the "struct hack" -- look it
//! up if you're not familiar with it.
struct raw_digits_t {
    private:
    size_t copy_count;

    public:
    const size_t max_length;
    const bool readonly;
    digit_t digits[minimum_digits]; // Must be last data item, see "struct hack"

    raw_digits_t(size_t max_len = 1, bool ro = false);
    raw_digits_t(size_t max_len, bool ro, digit_t init);
    size_t copy(const raw_digits_t& c, size_t length = 0);
    void copy(const digit_t *s, size_t length);
    size_t zero(size_t length = 0);
    size_t copies() const;

    friend class base_digitmanager_t;
};

const size_t datasize = (sizeof(raw_digits_t) + sizeof(digit_t) - 1) /
    sizeof(digit_t);

//! Stores and manages the raw digits.
class base_digitmanager_t {
    public:
    base_digitmanager_t(): raw_data(0) { }
    virtual ~base_digitmanager_t() { };

    raw_digits_t *get() { return raw_data; }
    const raw_digits_t *get() const { return raw_data; }
    void reset(raw_digits_t *p) { if (raw_data != p) { dec(); raw_data = p;
        inc(); } }

    raw_digits_t* operator->() { return raw_data; }
    const raw_digits_t* operator->() const { return raw_data; }
    void copy(base_digitmanager_t *c, size_t length) { raw_data->copy(
        *c->raw_data, length); }

    void resize_and_uniquify(size_t new_requested_max_length = 0);

    virtual base_digitmanager_t *new_of_type(base_digitmanager_t *copy = 0)
        const = 0;
    virtual size_t fixed_length() const { return 0; }

    /*! \name Modification Hooks

        beforemod and aftermod will be called once per modification, before the
        first beginmod does any work and after the last endmod finishes. The
        default implementations do nothing.
    */
    //@{
    virtual void beforemod() { }
    virtual void aftermod() { }
    virtual void endmod_hook(digit_t*) { }
    //@}

    protected:
    void inc() { if (raw_data) ++raw_data->copy_count; }
    void dec() { if (raw_data && --raw_data->copy_count == 0)
            dealloc(raw_data); }

    raw_digits_t *alloc(size_t requested_length, bool readonly = false)
        const;
    raw_digits_t *realloc(raw_digits_t *data, size_t new_requested_length)
        const;
    void dealloc(raw_digits_t *data) const;

    /*! \name Interface to memory allocation.

        These could be static functions, since they don't access any existing
        data, but static functions can't be virtual.
    */
    //@{
    virtual digit_t* raw_allocate(size_t requested_length) const = 0;
    virtual digit_t* raw_reallocate(digit_t *data, size_t new_requested_length)
        const { return 0; }
    virtual void raw_deallocate(digit_t *data, size_t length) const = 0;
    //@}

    virtual raw_digits_t *zerodata() const = 0;

    raw_digits_t *raw_data;

    friend void swap(base_digitmanager_t& p1, base_digitmanager_t& p2);
};

//! This is the allocator-based version of base_digitmanager_t, with a few other
//! features.
template <class Alloc = std::allocator<digit_t>, bool Threadsafe = true, bool
    Secure = false>
class digitmanager_t: public base_digitmanager_t {
    BOOST_STATIC_ASSERT(sizeof(typename Alloc::value_type) == sizeof(digit_t));

    public:
    digitmanager_t() { reset(zerodata()); }
    digitmanager_t(size_t requested_length, bool readonly = false)
        { if (requested_length > 0) reset(alloc(requested_length, readonly)); }
    digitmanager_t(const base_digitmanager_t& copy) {
        base_digitmanager_t& c(const_cast<base_digitmanager_t&>(copy));
        reset(c.get()); }
    virtual ~digitmanager_t() { dec(); }

    virtual base_digitmanager_t *new_of_type(base_digitmanager_t *copy = 0)
        const { if (copy != 0) return new digitmanager_t<Alloc, Threadsafe,
        Secure>(*copy); else return new digitmanager_t<Alloc, Threadsafe,
        Secure>; }
    virtual void aftermod() { if (Threadsafe && raw_data && raw_data->copies() >
        1) resize_and_uniquify(); }

    protected:
    virtual digit_t* raw_allocate(size_t requested_length) const {
        return allocator.allocate(requested_length); }
    virtual void raw_deallocate(digit_t *data, size_t length) const {
        if (Secure) memset(data, 0, length * sizeof(digit_t));
        allocator.deallocate(data, length); }
    virtual raw_digits_t *zerodata() const { static digitmanager_t z(1, true);
        return z.raw_data; }

    static Alloc allocator;
};

template <class Alloc, bool Threadsafe, bool Secure>
Alloc digitmanager_t<Alloc, Threadsafe, Secure>::allocator;

////////////////////////////////////////////////////////////////////////////////

//! Interface between the raw data and the functions that manipulate it.
struct data_t {
    typedef base_divide_t<data_t> divide_t;

    size_t length;
    bool negative;

    explicit data_t(base_digitmanager_t &holder, size_t len = 1, bool neg =
        false);
    explicit data_t(base_digitmanager_t *holder);
    data_t(const data_t& copy);
    data_t(const data_t& copy, bool negative);
    ~data_t();

    template <typename T>
    data_t(base_digitmanager_t *holder_to_copy, T n, size_t prealloc = 0,
        typename boost::enable_if<boost::is_integral<T> >::type* = 0);

    base_digitmanager_t *new_holder() const;
    const base_digitmanager_t *holder() const;

    data_t duplicate() const;
    void duplicate_data(const data_t copy);

    void reset(data_t p);
    data_t& operator=(const data_t& p);

    bool same_storage(const data_t n) const;
    bool is_unique() const;
    void make_unique();

    void set(int n);
    void set_signed(boost::intmax_t n);
    void set_unsigned(boost::uintmax_t n, bool neg = false);

    digit_t get_digit(size_t i) const;
    digit_t operator[](size_t i);
    digit_t operator[](size_t i) const { return get_digit(i); }
    digit_t *digits();
    const digit_t *digits() const;
    size_t max_length() const;
    bool is_fixed_length() const { return (dataholder->fixed_length() > 0); }

    bool is_zero() const;
    const data_t abs() const;
    const data_t negate() const;

    void extend(size_t new_length = 0, bool skip_mod = false);

    void from_string(const char *str, size_t length, size_t base);
    void from_string(const char *str, char **endptr, size_t base);
    void from_string(const std::string& str, size_t base = 10);
    void from_binary(xint::binary_t b, size_t bits = 0);

    /*! \name Modification hooks

        You must call beginmod (with an optional minimum size) before any
        operation that modifies the data (other than simply the negative flag),
        and endmod after it.
    */
    //!@{
    void beginmod(size_t requested_size = 0, bool extend = false);
    void endmod(bool allow_negative_zero = false);
    void beginendmod() { beginmod(); digits(); endmod(true); }
    //!@}

    int sign(bool signed_zero = false) const;
    bool is_odd() const;
    bool is_even() const;
    bool is_nan() const { return (dataholder->get() == 0); }
    size_t hex_digits() const;

    data_t&       operator++();    // Preincrement
    data_t&       operator--();    // Predecrement
    data_t        operator++(int); // Postincrement
    data_t        operator--(int); // Postdecrement

    bool          operator!() const { return is_zero(); }
    data_t        operator-() const;
    data_t&       operator+()       { return *this; }
    const data_t& operator+() const { return *this; }
    // Fixed-size types can also implement operator~

    data_t&       operator+=(const data_t b);
    data_t&       operator-=(const data_t b);
    data_t&       operator*=(const data_t b);
    data_t&       operator/=(const data_t b);
    data_t&       operator%=(const data_t b);

    data_t&       operator&=(const data_t n);
    data_t&       operator|=(const data_t n);
    data_t&       operator^=(const data_t n);
    data_t&       operator<<=(size_t shift);
    data_t&       operator>>=(size_t shift);

    static size_t bits_to_digits(size_t bits) { return (bits + bits_per_digit -
        1) / bits_per_digit; }

    ////////////////////////////////////////////////////////////////////////////
    private:
    digit_t *digits(bool) { return (*dataholder)->digits; }

    base_digitmanager_t * const dataholder;
    const bool holder_managed_elsewhere;
    size_t modcount;
    size_t mod_requested_size;
    bool mod_requested_extend;
    bool call_endmod_hook;

    friend void swap(data_t& p1, data_t& p2);
};

// Conversion functions
template <typename T> T to(const data_t n, typename
    boost::enable_if<boost::is_integral<T> >::type* = 0);
std::string to_string(const data_t n, size_t base = 10, bool uppercase = false);
xint::binary_t to_binary(const data_t n, size_t bits = 0);

// Bit-manipulation functions
bool getbit(const data_t n, size_t bit_index);
void setbit(data_t& n, size_t bit_index);
void clearbit(data_t& n, size_t bit_index);
size_t lowestbit(const data_t n, size_t valueIfZero = 0);
size_t highestbit(const data_t n, size_t valueIfZero = 0);

// Random and prime number functions
int is_prime(const data_t n, callback_t callback = no_callback);

// Comparison Operators
bool operator<(const data_t n1, const data_t n2);
bool operator>(const data_t n1, const data_t n2);
bool operator<=(const data_t n1, const data_t n2);
bool operator>=(const data_t n1, const data_t n2);
bool operator==(const data_t n1, const data_t n2);
bool operator!=(const data_t n1, const data_t n2);

// Mathematical operators
data_t operator+(const data_t n1, const data_t n2);
data_t operator-(const data_t n1, const data_t n2);
data_t operator*(const data_t n1, const data_t n2);
data_t operator/(const data_t n1, const data_t n2);
data_t operator%(const data_t n1, const data_t n2);

// Bitwise operators
data_t operator&(const data_t n1, const data_t n2);
data_t operator|(const data_t n1, const data_t n2);
data_t operator^(const data_t n1, const data_t n2);

// Shift operators
data_t operator<<(const data_t n, size_t by);
data_t operator>>(const data_t n, size_t by);

// Miscellaneous
data_t abs(const data_t n);
size_t log2(const data_t n);
int compare(const data_t n1, const data_t n2, bool ignoresign = false);

void pow(data_t& target, const data_t n, const data_t exponent);
void pow2(data_t& target, size_t exponent);
void factorial(data_t& target, size_t n);

void sqrt(data_t& target, const data_t n);

void gcd(data_t& target, const data_t num1, const data_t num2);
void lcm(data_t& target, const data_t num1, const data_t num2);
void invmod(data_t& target, const data_t n, const data_t modulus);

void mulmod(data_t& target, const data_t n, const data_t by, const data_t
    modulus);
void sqrmod(data_t& target, const data_t n, const data_t modulus);
void powmod(data_t& target, const data_t n, const data_t exponent, const data_t
    modulus, bool noMontgomery=false);

void square(data_t& target, const data_t n);
data_t::divide_t divide(const data_t d1, const data_t d2);
void divide(data_t& qtarget, data_t& rtarget, const data_t d1, const data_t d2);

size_t log10_bits(size_t bits);

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

////////////////////////////////////////////////////////////////////////////////
// Function templates

#include "../exceptions.hpp"

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

size_t log2_base(boost::uintmax_t n, size_t maxbits);

template <typename T>
data_t::data_t(base_digitmanager_t *holder, T n, size_t prealloc, typename
    boost::enable_if<boost::is_integral<T> >::type*): dataholder(holder),
    holder_managed_elsewhere(false), modcount(0)
{
    beginmod(prealloc);
    if (std::numeric_limits<T>::is_signed) set_signed(n);
    else set_unsigned(n);
    endmod();
}

template <typename T>
size_t log2_helper(const T n,
    typename boost::enable_if<boost::is_unsigned<T> >::type* = 0)
{
    return log2_base(n, std::numeric_limits<T>::digits);
}

template <typename T>
size_t log2_helper(const T n,
    typename boost::enable_if<boost::is_signed<T> >::type* = 0)
{
    typedef typename boost::make_unsigned<T>::type uT;
    T nn = (n < 0 ? -n : n);
    return log2_base(static_cast<uT>(nn), std::numeric_limits<T>::digits);
}

template <typename T>
size_t log2(const T n,
    typename boost::enable_if<boost::is_integral<T> >::type* = 0)
{
    return log2_helper(n);
}

template <typename T>
T to(const data_t n, typename boost::enable_if<boost::is_integral<T> >::type*) {
    using std::numeric_limits;

    // We've got to use an unfixed manager, rather than n's manager, because
    // the first n might be a fixed_integer_t which would screw up later calls
    // for other types.
    static digitmanager_t<> unfixed_manager;
    static const data_t min(unfixed_manager.new_of_type(),
		(numeric_limits<T>::min)());
    static const data_t max(unfixed_manager.new_of_type(),
		(numeric_limits<T>::max)());
    if (n < min || n > max) throw exceptions::too_big("value out of range for "
        "requested conversion");

    T rval = 0, shift = T(digit_overflowbit);
    for (size_t x = 0; x < n.length; ++x) {
        if (sizeof(T) > sizeof(digit_t)) rval *= shift;
        rval += static_cast<T>(n[n.length - x - 1]);
    }
    if (n.negative) rval *= -1;
    return rval;
}

template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT,
    traits>& out, const data_t n)
{
    int base=((out.flags() & std::ios::hex) ? 16
        : (out.flags() & std::ios::oct) ? 8
        : 10);
    bool upperCase=(out.flags() & std::ios::uppercase ? true : false);

    int nsign = (n.is_zero() ? 0 : n.negative ? -1 : 1);
    if ((out.flags() & std::ios::showpos) && nsign >= 0) out << "+";

    if (out.flags() & std::ios::showbase) {
        if (nsign < 0) out << "-";

        if (base==16 && upperCase) out << "0X";
        else if (base==16) out << "0x";
        else if (base==8) out << "0";

        out << to_string(n.abs(), base, upperCase);
    } else {
        out << to_string(n, base, upperCase);
    }
    return out;
}

template <typename charT, typename traits>
inline std::basic_istream<charT,traits>& operator>>(std::basic_istream<charT,
    traits>& in, data_t& n)
{
    int hex=(in.flags() & std::ios::hex) ? 1 : 0;
    int dec=(in.flags() & std::ios::dec) ? 1 : 0;
    int oct=(in.flags() & std::ios::oct) ? 1 : 0;
    int count=hex+dec+oct;

    size_t base=autobase;
    if (count==1) {
        if (hex) base=16;
        else if (oct) base=8;
        else base=10;
    } else if (count>1) base=10;
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

namespace boost {
namespace xint {
//! \brief A base class for all %integer types.
class any_integer {
    #ifndef BOOST_XINT_DOXYGEN_IGNORE
    public:
    detail::data_t& _data() { return data; }
    const detail::data_t& _data() const { return data; }

    size_t _get_length() const { return data.length; }
    detail::digit_t _get_digit(size_t index) const { return data[index]; }
    bool _is_fixed() const { return data.is_fixed_length(); }

    protected:
    any_integer(detail::base_digitmanager_t &holder, size_t len, bool neg =
        false): data(holder, len, neg) { }
    bool _is_nan() const { return data.is_nan(); }

    detail::data_t data;
    #endif
};

typedef size_t bitsize_t;

} // namespace xint
} // namespace boost

#ifndef BOOST_XINT_COMPILED_LIB
    #include "../exceptions.hpp"
    #include "../random.hpp"
    #include "bitqueue.hpp"
    #ifdef _WIN32
        #define STRICT
        #define WIN32_LEAN_AND_MEAN
        #define NOMINMAX
        #include <windows.h>
    #endif
    #include <sstream>
    #include <fstream>
    #include <ctime>
    #include <boost/scoped_array.hpp>
    #define BOOST_XINT_INLINE inline

    #define BOOST_XINT_FROM_HEADER
    #include "data_t.cpp"
    #include "compare.cpp"
    #include "shift.cpp"
    #include "bitfunctions.cpp"
    #include "andorxor.cpp"
    #include "addsubtract.cpp"
    #include "multiply.cpp"
    #include "divide.cpp"
    #include "powers.cpp"
    #include "roots.cpp"
    #include "monty.cpp"
    #include "modular.cpp"
    #include "gcd.cpp"
    #include "convert.cpp"
    #include "prime.cpp"
    #include "random.cpp"
    #undef BOOST_XINT_FROM_HEADER
#else
    #define BOOST_XINT_INLINE
#endif // BOOST_XINT_COMPILED_LIB

#endif // BOOST_INCLUDED_XINT_INTERNALS_HPP
