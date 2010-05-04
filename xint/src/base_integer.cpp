
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
    \brief Contains the definitions for the \c base_integer member functions.
*/

#include "../boost/xint/internals.hpp"
#include "../boost/xint/integer.hpp"

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

struct variable_zerodata_t: public data_t {
    public:
    variable_zerodata_t() { copies=1; fixed_mask=0; length=max_length=1;
        *magnitude=0; }
};

variable_zerodata_t* variable_zerodata() {
    static std::auto_ptr<variable_zerodata_t> z(new variable_zerodata_t);
    return z.get();
}

////////////////////////////////////////////////////////////////////////////////

base_integer::base_integer(): flags(0), data(0) { }

base_integer::base_integer(const base_integer& c, bool fixed): flags(0), data(0)
{
    if (fixed) flags |= flag_fixedlength;
    _attach(c, false);
}

base_integer::base_integer(size_t count, digit_t mask): flags(flag_fixedlength),
    data(0)
{
    _base_attach(allocate_fixed(mask, count, 1));
}

base_integer::base_integer(data_t *adopt, bool neg): flags(0), data(0) {
    if (adopt && adopt->fixed_mask != 0) flags |= flag_fixedlength;
    _base_attach(adopt);
    _set_negative(neg);
}

base_integer::~base_integer() {
    if (data && --data->copies == 0) deallocate(data);
}

digit_t base_integer::_get_digit(size_t index) const {
    return data->magnitude[index];
}

digit_t base_integer::_get_digit(size_t index, bool) const {
    if (index >= data->length) return 0;
    return data->magnitude[index];
}

digit_t* base_integer::_get_writable_digits() {
    assert(!_get_readonly());
    return data->magnitude;
}

const digit_t* base_integer::_get_digits() const {
    return data->magnitude;
}

size_t base_integer::_get_length() const {
    return data->length;
}

size_t base_integer::_get_max_length() const {
    return _is_fixed() ? _fixed_digits() : (std::numeric_limits<size_t>::max)();
}

void base_integer::_set_length(size_t digits) {
    data->length = digits;
}

size_t base_integer::_log2() const {
    size_t len = _get_length() - 1;
    return (bits_per_digit * len) + log2(_get_digit(len));
}

void base_integer::_set_unsigned(boost::uintmax_t n, bool negative) {
    // Assumes that the proper allocation & sizing was already done
    assert(!_get_readonly());
    if (data == 0) _base_attach(allocate(detail::digits_in_uintmax));
    if (digit_t(n) == n) {
        data->length = 1;
        *data->magnitude = digit_t(n);
    } else {
        digit_t *i = data->magnitude, *ie = i + data->max_length;
        while (n != 0 && i != ie) {
            *i++ = digit_t(n & digit_mask);
            n >>= bits_per_digit;
        }
        data->length = i - data->magnitude;
    }
    _set_negative(negative);
    _cleanup();
}

void base_integer::_set_signed(boost::intmax_t n) {
    if (n >= 0) {
        _set_unsigned(n);
    } else if (n == (std::numeric_limits<boost::intmax_t>::min)()) {
        // Have to treat the minimum number carefully, because -n is not
        // what you'd think it is.
        _set_unsigned(-(n+1), true);
        _decrement();
    } else {
        _set_unsigned(-n, true);
    }
}

bool base_integer::_is_unique() const {
    return (!data || data->copies <= 1);
}

bool base_integer::_is_zero() const {
    return (data->length == 1 && *data->magnitude == 0);
}

bool base_integer::_is_fixed() const {
    if (data) return (data->fixed_mask != 0);
    else return ((flags & flag_fixedlength) != 0);
}

size_t base_integer::_fixed_digits() const {
    return _is_fixed() ? data->max_length : 0;
}

size_t base_integer::_fixed_bits() const {
    size_t digits = _fixed_digits();
    if (digits != 0) return ((digits - 1) * detail::bits_per_digit) +
            detail::log2(data->fixed_mask);
    else return 0;
}

bool base_integer::_get_negative() const {
    return ((flags & flag_negative) ? true : false);
}

void base_integer::_set_negative(bool neg) {
    if (neg) flags |= flag_negative;
    else flags &= ~flag_negative;
}

void base_integer::_toggle_negative() {
    flags ^= flag_negative;
}

bool base_integer::_get_readonly() const {
    return ((flags & flag_readonly) ? true : false);
}

void base_integer::_set_readonly() {
    flags |= flag_readonly;
}

void base_integer::_realloc(size_t new_digit_count) {
    data = reallocate(data, new_digit_count);
}

void base_integer::_make_unique() {
    if (data && !_is_unique()) {
        data_t *olddata = data;

        data = (olddata->fixed_mask != 0 ?
            allocate_fixed(olddata->fixed_mask, olddata->max_length, 0) :
            allocate(olddata->length, 0));
        data->copies = 1;
        data->length = olddata->length;

        digit_t *s = olddata->magnitude, *se = s + olddata->length;
        digit_t *t = data->magnitude;
        while (s != se) *t++ = *s++;

        --olddata->copies;
    }
}

void base_integer::_mark_movable() {
    if (_is_unique()) flags |= flag_movable;
}

bool base_integer::_is_movable() const {
    return ((flags & flag_movable) ? true : false);
}

void base_integer::_swap(base_integer& other) {
    flag_t tempflags = flags;
    flags = other.flags;
    other.flags = tempflags;

    data_t *tempdata = data;
    data = other.data;
    other.data = tempdata;
}

void base_integer::_base_attach(data_t *new_data, flag_t setflags, size_t
    extra_allocation, bool is_movable)
{
    #ifdef BOOST_XINT_USE_COPY_ON_WRITE
        bool adopt=true;
    #else
        bool adopt=(new_data && is_movable);
    #endif

    if (data != new_data || extra_allocation != 0) {
        // If only one of them is fixed-length, the data can't be adopted.
        if (adopt && new_data && _is_fixed() != (new_data->fixed_mask != 0))
            adopt=false;

        // If either of them are fixed-length, and they're not the same fixed-
        // length, then the data can't be adopted.
        if (adopt && data && new_data && (data->fixed_mask ||
            new_data->fixed_mask) && (data->fixed_mask != new_data->fixed_mask
            || data->max_length != new_data->max_length)) adopt=false;

        // If we don't have a data item, but we're supposed to allocate extra
        // data, we can't adopt the existing data item.
        if (adopt && extra_allocation != 0 && !data) adopt=false;

        // If extra_allocation is requested, and it's a variable-length integer,
        // check the max_size. If there isn't enough to fit in the
        // extra_allocation, don't allow adoption.
        if (adopt && extra_allocation != 0 && data->fixed_mask == 0)
            if (data->length + extra_allocation > data->max_length) adopt=false;

        if (adopt) {
            if (data && --data->copies == 0) deallocate(data);
            data=new_data;
            if (data) ++data->copies;
        } else if (new_data != 0) {
            // Duplicate the data in a unique data_t item
            if (data == 0) {
                data = (_is_fixed() ?
                    allocate_fixed(new_data->fixed_mask, new_data->max_length,
                        0) :
                    allocate(new_data->length + extra_allocation, 0));
                ++data->copies;
            } else if ((data->copies > 1 || data->max_length < new_data->length
                + extra_allocation) || _get_readonly())
            {
                data_t *olddata = data;
                data = (_is_fixed() ?
                    allocate_fixed(data->fixed_mask, data->max_length, 0) :
                    allocate(new_data->length + extra_allocation, 0));
                ++data->copies;
                if (--olddata->copies == 0) deallocate(olddata);
            }
            data->length = (std::min)(new_data->length + extra_allocation,
                data->max_length);

            digit_t *s = new_data->magnitude, *se = s + new_data->length;
            digit_t *t = data->magnitude, *te = t + data->length;
            while (t != te && s != se) *t++ = *s++;
            zero(t, te);
        } else {
            if (--data->copies == 0) deallocate(data);
            data=0;
        }
    }
    flags = setflags;
}

void base_integer::_attach(const base_integer& copy, bool movable) {
    bool move = (copy._is_movable() || (movable && copy.data &&
        copy.data->copies <= 1));
    _base_attach(copy.data, copy.flags, 0, move);
    if (data) _cleanup();
}

void base_integer::_cleanup() {
    digit_t *p = data->magnitude + data->length - 1;
    if (data->fixed_mask != 0 && p == data->magnitude + data->max_length - 1)
        *p &= data->fixed_mask;
    if (*p == 0) {
        if (p != data->magnitude) {
            while (--p != data->magnitude && *p == 0); // Null loop
            if (*p == 0) _set_negative(false); // It's zero.
            data->length = p - data->magnitude + 1;
        } // It's zero, but it may legitimately be a "negative zero", leave it.
    }
}

integer base_integer::_to_integer(bool treat_as_movable) const {
    integer r;
    r._attach(*this, treat_as_movable);
    return r;
}

void base_integer::_increment(bool absolute_value) {
    _make_unique();
    if (_is_zero()) {
        data->magnitude[0] = 1;
        _set_negative(false);
    } else if (!absolute_value && _get_negative()) {
        _decrement(true);
    } else {
        _realloc(_get_length() + 1);
        digit_t *p = _get_writable_digits(), *pe = p + _get_length();

        while (p < pe) {
            if (*p == digit_mask) *p++ = 0;
            else { *p++ += 1; break; }
        }
        _cleanup();
    }
}

void base_integer::_decrement(bool absolute_value) {
    _make_unique();
    if (_is_zero()) {
        data->magnitude[0]=1;
        _set_negative(true);
    } else if (!absolute_value && _get_negative()) {
        _increment(true);
    } else {
        digit_t *p = _get_writable_digits(), *pe = p + _get_length();
        while (p != pe) {
            if (*p == 0) *p++ = digit_mask;
            else { *p++ -= 1; break; }
        }
        _cleanup();
    }
}

////////////////////////////////////////////////////////////////////////////////

void base_variable_length_integer::_attach_0() {
    _base_attach(variable_zerodata());
}

size_t log10_bits(size_t bits) {
    assert(std::numeric_limits<boost::intmax_t>::digits >= 32);
    const boost::intmax_t large_step(1000000000), small_step(10);
    const size_t count_per_large_step = 9, count_per_small_step = 1;
    integer n(pow2(bits)-1);

    size_t r = 0;
    while (n >= large_step) { n /= large_step; r += count_per_large_step; }
    while (n >= small_step) { n /= small_step; r += count_per_small_step; }
    return r;
}

} // namespace detail
//! @endcond detail

} // namespace xint
} // namespace boost
