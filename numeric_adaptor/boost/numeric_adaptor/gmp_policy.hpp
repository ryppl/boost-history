// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_GMP_POLICY_HPP_
#define NUMERIC_ADAPTOR_GMP_POLICY_HPP_


#include <string>


#include <gmp.h>



struct gmp_policy
{
    typedef mpf_t type;

    static inline void init(type& value)
    {
        mpf_init(value);
    }

    static inline void destruct(type& value)
    {
        mpf_clear(value);
    }

    template <typename OtherType>
    static inline void set(type& value, const OtherType& v)
    {
        mpf_set_d(value, v);
    }

    static inline void set(type& value, const std::string& v)
    {
        mpf_set_str(value, v.c_str(), 10);
    }


    static inline void copy(const type& source, type& dest)
    {
        mpf_set(dest, source);
    }

    static inline void add(type& r, const type& a, const type& b)
    {
        mpf_add(r, a, b);
    }

    static inline void subtract(type& r, const type& a, const type& b)
    {
        mpf_sub(r, a, b);
    }

    static inline void multiply(type& r, const type& a, const type& b)
    {
        mpf_mul(r, a, b);
    }

    static inline void divide(type& r, const type& a, const type& b)
    {
        mpf_div(r, a, b);
    }

    static inline void sqrt(type& r, const type& a)
    {
        mpf_sqrt(r, a);
    }

    static inline void cos(type& r, const type& a)
    {
        // COS is not available.
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::cos(d));
    }

    static inline void sin(type& r, const type& a)
    {
        // SIN is not available.
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::sin(d));
    }


    template <typename OtherType>
    static inline OtherType big_numeric_cast(const type& b)
    {
        return mpf_get_d(b);
    }


    static inline std::string as_string(type const& a)
    {
        mp_exp_t bexp;
        std::string out = mpf_get_str (0, &bexp, 10, 0, a);
        return out;
    }


    static inline int compare(const type& a, const type& b)
    {
        return mpf_cmp(a, b);
    }
};


#endif
