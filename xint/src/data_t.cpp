
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

#include "../boost/xint/xint.hpp"

#include <cassert>

namespace boost {
namespace xint {
namespace detail {

static size_t datasize=(sizeof(data_t<>)+sizeof(digit_t)-1) / sizeof(digit_t);

data_t<> *allocate(size_t max_digits, size_t initial_length) {
    if (max_digits < 1) max_digits = 1;
    size_t count = max_digits - 1 + datasize;

    try {
        digit_t *storage(new digit_t[count]);
        data_t<> *r=reinterpret_cast<data_t<>*>(storage);
        r->copies=0;
        r->length=(std::min)(initial_length, max_digits);
        r->max_length=max_digits;

        digit_t *p = r->magnitude, *pe = p + r->length;
        while (p!=pe) *p++=0;
        return r;
    } catch (std::bad_alloc&) {
        throw exceptions::overflow_error("Out of memory allocating integer");
    }
}

data_t<> *reallocate(data_t<> *olddata, size_t newsize) {
    if (olddata==0) {
        data_t<> *newdata=allocate(newsize);
        newdata->copies=1;
        return newdata;
    } if (olddata->copies == 1 && newsize <= olddata->max_length) {
        // We can re-use this data.
        if (olddata->length < newsize) {
            digit_t *p=olddata->magnitude + olddata->length,
                *pe=olddata->magnitude + newsize;
            while (p!=pe) *p++=0;
        } else {
            digit_t *p=olddata->magnitude + olddata->length,
                *pe=olddata->magnitude + newsize;
            while (p!=pe) *(--p)=0;
        }
        olddata->length = newsize;
        return olddata;
    } else {
        // Have to allocate new data.
        data_t<> *newdata=allocate(newsize, 0);
        newdata->copies=1;
        newdata->length=newsize;
        digit_t *s=olddata->magnitude, *se=s+olddata->length,
            *t=newdata->magnitude, *te=t+newdata->length;
        while (s!=se) *t++ = *s++;
        while (t < te) *t++ = 0;

        if (--olddata->copies == 0) deallocate(olddata);
        return newdata;
    }
}

data_t<> *duplicate(data_t<> *olddata, size_t extra) {
    data_t<> *r = allocate(olddata->length+extra, 0);
    r->length = olddata->length;
    r->copies=1;

    digit_t *s = olddata->magnitude, *se = s + olddata->length,
        *t = r->magnitude, *te = t + olddata->length + extra;
    while (s!=se) *t++ = *s++;
    while (t < te) *t++ = 0;

    if (olddata && --olddata->copies == 0) deallocate(olddata);
    return r;
}

data_t<> *duplicate(data_t<> *olddata, data_t<> *data_to_copy, size_t extra) {
    if (!olddata || olddata->copies > 1 || data_to_copy->length + extra >
        olddata->max_length)
    {
        data_t<> *r = allocate(data_to_copy->length+extra, 0);
        r->length = data_to_copy->length + extra;
        digit_t *s = data_to_copy->magnitude, *se = s + data_to_copy->length,
            *t = r->magnitude, *te = t + r->length;
        while (s!=se) *t++ = *s++;
        while (t < te) *t++ = 0;
        r->copies=1;
        if (olddata && --olddata->copies == 0) deallocate(olddata);
        return r;
    } else {
        data_t<> *r = olddata;
        digit_t *s = data_to_copy->magnitude, *se = s + data_to_copy->length,
            *t = r->magnitude, *te = t + r->length + extra;
        while (s != se) *t++ = *s++;
        while (t < te) *t++ = 0;
        r->length = data_to_copy->length;
        return r;
    }
}

void deallocate(data_t<> *data) {
    digit_t *digits=reinterpret_cast<digit_t*>(data);

    #ifdef XINT_SECURE
    digit_t *p = digits, *pe = p + data->max_length + datasize;
    while (p != pe) *p++ = 0;
    #endif

    delete[] digits;
}

} // namespace detail
} // namespace xint
} // namespace boost
