
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
    \brief Contains the definitions for data_t allocation functions.
*/

#include "../boost/xint/internals.hpp"

#define VALGRIND_COMPATIBLE

#include <cassert>

namespace boost {
namespace xint {

//! @cond detail
namespace detail {

static const size_t datasize = (sizeof(data_t)+sizeof(digit_t)-1) /
    sizeof(digit_t);
static const size_t minimum_digits = 1, digits_in_struct = 1;

void zero(digit_t *p, digit_t *end) {
    while (p < end) *p++ = 0;
}

void zero(digit_t *p, size_t size) {
    zero(p, p + size);
}

data_t *allocate(size_t max_digits, size_t initial_length, digit_t mask) {
    max_digits = (std::max)(max_digits, minimum_digits);
    size_t count = max_digits - digits_in_struct + datasize;

    try {
        #ifdef VALGRIND_COMPATIBLE
            void *storage = malloc(count * sizeof(digit_t));
            data_t *r = new(storage) data_t;
        #else
            digit_t *storage(new digit_t[count]);
            data_t *r = new(reinterpret_cast<void*>(storage)) data_t;
        #endif
        r->copies=0;
        r->length=(std::min)(initial_length, max_digits);
        r->max_length=max_digits;
        r->fixed_mask=mask;
        zero(r->magnitude, r->length);
        return r;
    } catch (std::bad_alloc&) {
        throw exceptions::overflow_error("Out of memory allocating integer");
    }
}

data_t *allocate_fixed(digit_t mask, size_t max_digits, size_t initial_length) {
    assert(mask != 0);
    return allocate(max_digits, initial_length, mask);
}

data_t *allocate_fixed_zero(digit_t mask, size_t max_digits) {
    data_t *r=allocate(max_digits, 1, mask);
    r->copies = 1;
    return r;
}

data_t *reallocate(data_t *olddata, size_t newsize) {
    if (olddata==0) {
        data_t *newdata=allocate(newsize);
        newdata->copies=1;
        return newdata;
    } else if (olddata->fixed_mask != 0) {
        // It's a fixed-size item, have to re-use it.
        newsize = (std::min)(newsize, olddata->max_length);
        if (olddata->length < newsize) zero(olddata->magnitude +
            olddata->length, olddata->magnitude + newsize);
        olddata->length = newsize;
        return olddata;
    } else if (olddata->copies == 1 && newsize <= olddata->max_length) {
        // We can re-use this data.
        if (olddata->length < newsize) zero(olddata->magnitude +
            olddata->length, olddata->magnitude + newsize);
        olddata->length = newsize;
        return olddata;
    } else {
        // Have to allocate new data.
        data_t *newdata=allocate(newsize, 0);
        newdata->copies = 1;
        newdata->length = newsize;

        digit_t *s = olddata->magnitude, *se = s + olddata->length,
            *t = newdata->magnitude, *te = t + newdata->length;
        while (s!=se) *t++ = *s++;
        zero(t, te);

        if (--olddata->copies == 0) deallocate(olddata);
        return newdata;
    }
}

void deallocate(data_t *data) {
    data->~data_t();
    digit_t *digits=reinterpret_cast<digit_t*>(data);

    #ifdef XINT_SECURE
    zero(digits, data->max_length + datasize - digits_in_struct);
    #endif

    #ifdef VALGRIND_COMPATIBLE
        free(digits);
    #else
        delete[] digits;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
// bitqueue_t functions

void bitqueue_t::push(doubledigit_t bits, size_t count) {
    if (count < ddbits) {
        doubledigit_t mask = (doubledigit_t(1) << count) - 1;
        bits &= mask;
    }

    if (pending.empty()) {
        pending.push(std::make_pair(bits, count));
    } else {
        indata_t &n(pending.back());
        if (n.second + count <= ddbits) {
            n.first |= bits << n.second;
            n.second += count;
        } else {
            pending.push(std::make_pair(bits, count));
        }
    }
    bits_pending += count;
}

doubledigit_t bitqueue_t::pop(size_t requestedbits) {
    doubledigit_t buffer = 0;
    size_t bits_in_buffer = 0;
    while (bits_in_buffer < requestedbits && !pending.empty()) {
        indata_t &n(pending.front());
        size_t maxbits = requestedbits - bits_in_buffer, actualbits =
            (std::min)(n.second, maxbits);
        buffer |= (n.first << bits_in_buffer);

        n.first >>= actualbits;
        n.second -= actualbits;
        bits_in_buffer += actualbits;
        bits_pending -= actualbits;

        if (n.second == 0) pending.pop();
    }
    return (buffer & ((doubledigit_t(1) << requestedbits) - 1));
}

} // namespace detail
//! @endcond detail

} // namespace xint
} // namespace boost
