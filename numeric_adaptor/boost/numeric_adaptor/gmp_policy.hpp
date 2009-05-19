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
#include <cstring>


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


    static inline void copy(type const& source, type& dest)
    {
        mpf_set(dest, source);
    }

    static inline void add(type& r, type const& a, type const& b)
    {
        mpf_add(r, a, b);
    }

    static inline void subtract(type& r, type const& a, type const& b)
    {
        mpf_sub(r, a, b);
    }

    static inline void multiply(type& r, type const& a, type const& b)
    {
        mpf_mul(r, a, b);
    }

    static inline void divide(type& r, type const& a, type const& b)
    {
        mpf_div(r, a, b);
    }

    static inline void abs(type& r, type const& a)
    {
        mpf_abs(r, a);
    }

    static inline void sqrt(type& r, type const& a)
    {
        mpf_sqrt(r, a);
    }

    static inline void cos(type& r, type const& a)
    {
        // COS is not available in GMP
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::cos(d));
    }

    static inline void sin(type& r, type const& a)
    {
        // SIN is not available in GMP
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::sin(d));
    }

    static inline void hypot(type& r, type const& a, type const& b)
    {
        mpf_mul(r, a, a);
        type t;
        mpf_init(t);
        mpf_mul(t, b, b);
        mpf_add(t, r, t);
        mpf_sqrt(r, t);
        mpf_clear(t);
    }


    template <typename OtherType>
    static inline OtherType big_numeric_cast(type const& b)
    {
        return mpf_get_d(b);
    }


    static inline std::string as_string(type const& a)
    {
        mp_exp_t exponent;
        static char s[1024];
        mpf_get_str (s, &exponent, 10, sizeof(s) - 1, a);

        std::string out;
        out.reserve(100);

        // TODO: this is probably not working well for large numbers -> add e0X

        register int i = 0;
        for (register const char* ptr = s; *ptr; ptr++, i++)
        {
            if (i == exponent)
            {
                out += ".";
            }
            out += *ptr;
        }
        return out;
    }


    static inline int compare(type const& a, type const& b)
    {
        return mpf_cmp(a, b);
    }
};


#endif
