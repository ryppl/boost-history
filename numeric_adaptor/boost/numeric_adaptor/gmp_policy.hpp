// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_GMP_POLICY_HPP
#define NUMERIC_ADAPTOR_GMP_POLICY_HPP


#include <string>
#include <cstring>

#include <gmp.h>


namespace boost { namespace numeric_adaptor {


struct gmp_policy
{
    typedef mpf_t value_type;

    static inline void init(value_type& value)
    {
        mpf_init(value);
    }

    static inline void destruct(value_type& value)
    {
        mpf_clear(value);
    }

    template <typename OtherType>
    static inline void set(value_type& value, const OtherType& v)
    {
        mpf_set_d(value, v);
    }

    static inline void set(value_type& value, const std::string& v)
    {
        mpf_set_str(value, v.c_str(), 10);
    }


    static inline void copy(value_type const& source, value_type& dest)
    {
        mpf_set(dest, source);
    }

    // TODO should we add specific overloads for function like mpf_add_ui?

    static inline void add(value_type& r, value_type const& a, value_type const& b)
    {
        mpf_add(r, a, b);
    }

    static inline void add(value_type& a, value_type const& b)
    {
        mpf_add(a, a, b);
    }

    static inline void subtract(value_type& r, value_type const& a, value_type const& b)
    {
        mpf_sub(r, a, b);
    }

    static inline void subtract(value_type& a, value_type const& b)
    {
        mpf_sub(a, a, b);
    }

    static inline void multiply(value_type& r, value_type const& a, value_type const& b)
    {
        mpf_mul(r, a, b);
    }

    static inline void multiply(value_type& a, value_type const& b)
    {
        mpf_mul(a, a, b);
    }

    static inline void divide(value_type& r, value_type const& a, value_type const& b)
    {
        mpf_div(r, a, b);
    }

    static inline void divide(value_type& a, value_type const& b)
    {
        mpf_div(a, a, b);
    }

    static inline void neg(value_type& r, value_type const& n)
    {
        mpf_neg(r, n);
    }

    static inline void abs(value_type& r, value_type const& a)
    {
        mpf_abs(r, a);
    }

    static inline void sqrt(value_type& r, value_type const& a)
    {
        mpf_sqrt(r, a);
    }

    static inline void cos(value_type& r, value_type const& a)
    {
        // COS is not available in GMP
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::cos(d));
    }

    static inline void sin(value_type& r, value_type const& a)
    {
        // SIN is not available in GMP
        long double d = mpf_get_d(a);
        mpf_set_d(r, std::sin(d));
    }

    static inline void hypot(value_type& r, value_type const& a, value_type const& b)
    {
        mpf_mul(r, a, a);
        value_type t;
        mpf_init(t);
        mpf_mul(t, b, b);
        mpf_add(t, r, t);
        mpf_sqrt(r, t);
        mpf_clear(t);
    }


    template <typename OtherType>
    static inline OtherType big_numeric_cast(value_type const& b)
    {
        return mpf_get_d(b);
    }


    static inline std::string as_string(value_type const& a)
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


    static inline int compare(value_type const& a, value_type const& b)
    {
        return mpf_cmp(a, b);
    }
};


}} // namespace boost::numeric_adaptor


#endif
