
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
    \brief Contains the function definitions for data_t and raw_digits_t.

    This file will be included by the library itself when needed.
*/

#if defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)

#if defined(BOOST_XINT_COMPILED_LIB)
    #include "internals.hpp"
    #include "../exceptions.hpp"
#endif // defined(BOOST_XINT_COMPILED_LIB)

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

////////////////////////////////////////////////////////////////////////////////
// raw_digits_t

BOOST_XINT_INLINE raw_digits_t::raw_digits_t(size_t max_len, bool ro):
    copy_count(0), max_length(max_len), readonly(ro) { }

BOOST_XINT_INLINE raw_digits_t::raw_digits_t(size_t max_len, bool ro, digit_t
    init): copy_count(0), max_length(max_len), readonly(ro)
{
    digits[0] = init;
}

BOOST_XINT_INLINE size_t raw_digits_t::copy(const raw_digits_t& c, size_t
    length)
{
    size_t max = (std::min)(max_length, c.max_length);
    if (length == 0 || length > max) length = max;
    copy(c.digits, length);
    return length;
}

BOOST_XINT_INLINE void raw_digits_t::copy(const digit_t *s, size_t length) {
    digit_t *t = digits, *te = t + (std::min)(length, max_length);
    while (t != te) *t++ = *s++;
}

BOOST_XINT_INLINE size_t raw_digits_t::zero(size_t length) {
    if (length == 0) length = max_length;
    else length = (std::min)(length, max_length);
    digit_t *t = digits, *te = t + length;
    while (t != te) *t++ = 0;
    return length;
}

BOOST_XINT_INLINE size_t raw_digits_t::copies() const {
    return copy_count;
}

////////////////////////////////////////////////////////////////////////////////
// base_digitmanager_t

BOOST_XINT_INLINE raw_digits_t *base_digitmanager_t::alloc(size_t
    requested_length, bool readonly) const
{
    size_t fixed_digit_length = fixed_length();
    size_t len = (fixed_digit_length ? fixed_digit_length :
        (std::max)(requested_length, minimum_digits));
    size_t count = len - minimum_digits + datasize;

    digit_t *storage = 0;
    try {
        storage = raw_allocate(count);
        if (readonly) return new(storage) raw_digits_t(len, readonly, 0);
        else return new(storage) raw_digits_t(len, readonly);
    } catch (std::bad_alloc&) {
        if (storage) raw_deallocate(storage, len); // Just in case.
        throw exceptions::overflow_error("Out of memory allocating integer");
    } catch (std::exception&) {
        // To make it exception-safe, we've got to ensure that the allocated
        // data is properly disposed of if there's an exception, before passing
        // the exception on.
        if (storage) raw_deallocate(storage, len);
        throw;
    }
}

//! Resizes the data (if possible), and ensures that it's unique in all cases.
BOOST_XINT_INLINE raw_digits_t *base_digitmanager_t::realloc(raw_digits_t
    *data, size_t requested_length) const
{
    if (fixed_length() == 0) {
        if (data && data->copies() <= 1 && !data->readonly) {
            digit_t *rawdata = raw_reallocate(reinterpret_cast<digit_t*>(data),
                requested_length);
            if (rawdata != 0) {
                if (reinterpret_cast<digit_t*>(data) == rawdata) {
                    *(const_cast<size_t*>(&data->max_length)) = requested_length;
                    return data;
                } else {
                    raw_digits_t *newdata =
                        new(rawdata) raw_digits_t(requested_length, false);
                    newdata->copy(*data);
                    return newdata;
                }
            }
        }
    } else requested_length = fixed_length();

    // Can't reallocate, move to plan B.
    raw_digits_t *r = data;
    if (data == 0 || data->copies() > 1 || data->readonly ||
        data->max_length < requested_length)
    {
        r = alloc(requested_length);
        if (data) r->copy(*data, requested_length);
    }

    return r;
}

BOOST_XINT_INLINE void base_digitmanager_t::dealloc(raw_digits_t *data) const {
    if (data == 0) return;

    size_t len = (data->max_length + datasize - minimum_digits);

    // Since the data was initialized with placement-new, we have to manually
    // call the destructor.
    data-> ~raw_digits_t();

    // Finally, dispose of it.
    raw_deallocate(reinterpret_cast<digit_t*>(data), len);
}

//! Tries to change max_length(), preserving current data, and ensures that the
//! data is unique. max_length is not guaranteed to change, so don't make any
//! assumptions about that.
BOOST_XINT_INLINE void base_digitmanager_t::resize_and_uniquify(size_t
    new_requested_max_length)
{
    // If the length can't change, and it's already unique, we don't need to do
    // anything.
    if (fixed_length() > 0 && raw_data->copies() <= 1) return;

    if (new_requested_max_length == 0)
        new_requested_max_length = raw_data->max_length;
    reset(realloc(raw_data, new_requested_max_length));
}

//! This should never be used on different derived classes, unless it doesn't
//! matter that the memory from one class will be deallocated in the other.
BOOST_XINT_INLINE void swap(base_digitmanager_t& p1, base_digitmanager_t& p2) {
    using std::swap;
    swap(p1.raw_data, p2.raw_data);
}

////////////////////////////////////////////////////////////////////////////////
// data_t

BOOST_XINT_INLINE data_t::data_t(base_digitmanager_t& holder, size_t len, bool
    neg): length(len), negative(neg), dataholder(&holder),
    holder_managed_elsewhere(true), modcount(0) { }

BOOST_XINT_INLINE data_t::data_t(base_digitmanager_t* holder):
    length(1), negative(false), dataholder(holder),
    holder_managed_elsewhere(false), modcount(0) { assert(holder != 0); }

//! Makes a copy that shares storage.
BOOST_XINT_INLINE data_t::data_t(const data_t& copy): length(copy.length),
    negative(copy.negative),
    dataholder(copy.dataholder->new_of_type(copy.dataholder)),
    holder_managed_elsewhere(false), modcount(0) { }

//! Makes a copy that shares storage and sets negative to a specific value,
//! possibly different from copy's.
BOOST_XINT_INLINE data_t::data_t(const data_t& copy, bool neg):
    length(copy.length), negative(neg),
    dataholder(copy.dataholder->new_of_type(copy.dataholder)),
    holder_managed_elsewhere(false), modcount(0) { }

BOOST_XINT_INLINE data_t::~data_t() {
    if (!holder_managed_elsewhere) delete dataholder;
}

BOOST_XINT_INLINE base_digitmanager_t *data_t::new_holder() const {
    return dataholder->new_of_type();
}

BOOST_XINT_INLINE const base_digitmanager_t *data_t::holder() const {
    if (is_nan()) throw exceptions::not_a_number();
    return dataholder;
}

//! Only works when abs(n) will fit into a digit_t.
BOOST_XINT_INLINE void data_t::set(int n) {
    beginmod(1);
    make_unique();
    if (n < 0) { negative = true; n = -n; } else negative = false;
    assert(n == int(digit_t(n)));
    length = 1;
    digits()[0] = digit_t(n);
    endmod();
}

BOOST_XINT_INLINE void data_t::set_signed(boost::intmax_t n) {
    if (n >= 0) {
        set_unsigned(n);
    } else if (n == (std::numeric_limits<boost::intmax_t>::min)()) {
        // Have to treat the minimum number carefully, because -n is not
        // what you'd think it is.
        set_unsigned(-(n + 1), true);
        --*this;
    } else {
        set_unsigned(-n, true);
    }
}

BOOST_XINT_INLINE void data_t::set_unsigned(boost::uintmax_t n, bool neg) {
    beginmod(detail::digits_in_uintmax);
    if (digit_t(n) == n) {
        length = 1;
        digits()[0] = digit_t(n);
    } else {
        digit_t *i = digits(), *ie = i + max_length();
        while (n != 0 && i != ie) {
            *i++ = digit_t(n & digit_mask);
            n >>= bits_per_digit;
        }
        length = (i - digits());
    }
    negative = neg;
    endmod();
}

//! Makes a copy with its own unique storage.
BOOST_XINT_INLINE data_t data_t::duplicate() const {
    data_t r(*this);
    r.make_unique();
    return r;
}

//! Copies the data into this object's unique storage.
BOOST_XINT_INLINE void data_t::duplicate_data(const data_t copy) {
    beginmod(copy.length);
    digits(); // To trigger any changes from the beginmod call
    length = (std::min)(max_length(), copy.length);
    negative = copy.negative;
    dataholder->copy(copy.dataholder, length);
    endmod();
}

//! Discards this item's data and adopts a copy of p.
BOOST_XINT_INLINE void data_t::reset(data_t p) {
    dataholder->reset(p.dataholder->get());
    length = p.length;
    negative = p.negative;
    call_endmod_hook = true;
}

BOOST_XINT_INLINE data_t& data_t::operator=(const data_t& p) {
    reset(p);
    return *this;
}

BOOST_XINT_INLINE const data_t data_t::abs() const {
    return data_t(*this, false);
}

BOOST_XINT_INLINE const data_t data_t::negate() const {
    return data_t(*this, !negative);
}

BOOST_XINT_INLINE bool data_t::same_storage(const data_t n) const {
    return (dataholder->get() == n.dataholder->get());
}

BOOST_XINT_INLINE bool data_t::is_unique() const {
    return ((*dataholder)->copies() <= 1);
}

BOOST_XINT_INLINE void data_t::make_unique() {
    if (!is_unique()) dataholder->resize_and_uniquify(max_length());
}

//! Unlike operator[], this function is protected from out-of-bounds accesses.
BOOST_XINT_INLINE digit_t data_t::get_digit(size_t i) const {
    return (i < length ? (*dataholder)->digits[i] : 0);
}

BOOST_XINT_INLINE digit_t data_t::operator[](size_t i) {
    return (*dataholder)->digits[i];
}

BOOST_XINT_INLINE digit_t *data_t::digits() {
    assert(modcount > 0);

    if (mod_requested_size > max_length()) {
        dataholder->resize_and_uniquify(mod_requested_size);
    } else {
        make_unique();
    }

    if (mod_requested_extend) {
        mod_requested_extend = false;
        extend(mod_requested_size, true);
    }
    mod_requested_size = 0;

    call_endmod_hook = true;
    return digits(true);
}

BOOST_XINT_INLINE const digit_t *data_t::digits() const {
    data_t& t(*const_cast<data_t*>(this));
    return t.digits(true);
}

BOOST_XINT_INLINE size_t data_t::max_length() const {
    return (*dataholder)->max_length;
}

BOOST_XINT_INLINE bool data_t::is_zero() const {
    return (length == 1 && (*this)[0] == 0);
}

//! Extends the current object's length (to the maximum, by default), zeroing
//! the digits in between.
BOOST_XINT_INLINE void data_t::extend(size_t new_length, bool skip_mod) {
    if (new_length == 0) new_length = max_length();
    else new_length = (std::min)(new_length, max_length());

    if (length < new_length) {
        digit_t *d;
        if (!skip_mod) {
            beginmod();
            d = (*dataholder)->digits;
        } else d = digits();
        digit_t *p = d + length, *pe = d + new_length;
        while (p < pe) *p++ = 0;
        length = new_length;
        if (!skip_mod) endmod();
    }
}

BOOST_XINT_INLINE void data_t::beginmod(size_t requested, bool extend) {
    if (modcount++ == 0) {
        dataholder->beforemod();
        mod_requested_size = requested;
        mod_requested_extend = extend;
        call_endmod_hook = false;
    } else {
        mod_requested_size = (std::max)(mod_requested_size, requested);
        if (extend) mod_requested_extend = true;
    }
}

BOOST_XINT_INLINE void data_t::endmod(bool allow_negative_zero) {
    assert(length <= max_length());

    digit_t *d = digits(true), *p = d + length - 1;
    if (call_endmod_hook) {
        dataholder->endmod_hook(d);
        call_endmod_hook = false;
    }

    // Adjust length to skip any leading zeros.
    while (p > d && *p == 0) --p;
    length = (p - d + 1);

    // Zero isn't negative, unless told otherwise.
    if (!allow_negative_zero && p == d && *p == 0) negative = false;

    assert(modcount > 0);
    if (--modcount == 0) dataholder->aftermod();
}

BOOST_XINT_INLINE int data_t::sign(bool allow_signed_zero) const {
    if (!allow_signed_zero && is_zero()) return 0;
    return (negative ? -1 : 1);
}

BOOST_XINT_INLINE bool data_t::is_odd() const {
    return (((*this)[0] & 0x01) != 0);
}

BOOST_XINT_INLINE bool data_t::is_even() const {
    return (((*this)[0] & 0x01) == 0);
}

BOOST_XINT_INLINE size_t data_t::hex_digits() const {
    return (log2(*this) + 3) / 4;
}

////////////////////////////////////////////////////////////////////////////////
// Miscellaneous related free functions

BOOST_XINT_INLINE void swap(data_t& p1, data_t& p2) {
    using std::swap;
    swap(p1.length, p2.length);
    swap(p1.negative, p2.negative);
    swap(*p1.dataholder, *p2.dataholder);
}

BOOST_XINT_INLINE size_t log2_base(boost::uintmax_t n, size_t maxbits) {
    size_t lo = 0, hi = maxbits;
    while (lo < hi) {
        size_t s = lo + ((hi - lo) >> 1);
        if (n >= (boost::uintmax_t(1) << s)) lo = s + 1;
        else hi = s;
    }
    return lo;
}

//! Use this one, rather than data_t's member version, if the final result needs
//! to be made thread-safe. Use the member version if thread-safety isn't a
//! problem (i.e. for internal functions) and maximum speed is desired.
BOOST_XINT_INLINE data_t abs(const data_t n) {
    data_t r(n.abs());
    r.beginmod();
    r.endmod();
    return r;
}

BOOST_XINT_INLINE size_t log2(const data_t n) {
    size_t r = bits_per_digit * (n.length - 1);
    return r + log2(n[n.length - 1]);
}

BOOST_XINT_INLINE size_t log10_bits(size_t bits) {
    assert(std::numeric_limits<boost::intmax_t>::digits >= 32);

    digitmanager_t<> holder;
    const data_t large_step(holder.new_of_type(), 1000000000);
    const data_t small_step(holder.new_of_type(), 10);
    const size_t count_per_large_step = 9, count_per_small_step = 1;

    data_t n(new digitmanager_t<>);
    pow2(n, bits);
    --n;

    size_t r = 0;
    while (n >= large_step) { n /= large_step; r += count_per_large_step; }
    while (n >= small_step) { n /= small_step; r += count_per_small_step; }
    return r;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // defined(BOOST_XINT_COMPILED_LIB) || defined(BOOST_XINT_FROM_HEADER)
