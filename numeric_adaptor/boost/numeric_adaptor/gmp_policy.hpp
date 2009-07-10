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


struct gmp_policy: default_policy<gmp_policy, mpf_t>
{
    gmp_policy()
    {
        mpf_init(value);
    }

    ~gmp_policy()
    {
        mpf_clear(value);
    }

    gmp_policy(gmp_policy const& source)
    {
        mpf_set(value, source.value);
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


    // TODO should we add specific overloads for function like mpf_add_ui?

    static inline void add(gmp_policy& r, gmp_policy const& a, gmp_policy const& b)
    {
        mpf_add(r.value, a.value, b.value);
    }

    static inline void add(gmp_policy& a, gmp_policy const& b)
    {
        mpf_add(a.value, a.value, b.value);
    }

    static inline void subtract(gmp_policy& r, gmp_policy const& a, gmp_policy const& b)
    {
        mpf_sub(r.value, a.value, b.value);
    }

    static inline void subtract(gmp_policy& a, gmp_policy const& b)
    {
        mpf_sub(a.value, a.value, b.value);
    }

    static inline void multiply(gmp_policy& r, gmp_policy const& a, gmp_policy const& b)
    {
        mpf_mul(r.value, a.value, b.value);
    }

    static inline void multiply(gmp_policy& a, gmp_policy const& b)
    {
        mpf_mul(a.value, a.value, b.value);
    }

    static inline void divide(gmp_policy& r, gmp_policy const& a, gmp_policy const& b)
    {
        mpf_div(r.value, a.value, b.value);
    }

    static inline void divide(gmp_policy& a, gmp_policy const& b)
    {
        mpf_div(a.value, a.value, b.value);
    }

    static inline void neg(gmp_policy& r, gmp_policy const& n)
    {
        mpf_neg(r.value, n.value);
    }

    static inline void abs(gmp_policy& r, gmp_policy const& a)
    {
        mpf_abs(r.value, a.value);
    }

    static inline void sqrt(gmp_policy& r, gmp_policy const& a)
    {
        mpf_sqrt(r.value, a.value);
    }

    static inline void hypot(gmp_policy& r, gmp_policy const& a, gmp_policy const& b)
    {
        mpf_mul(r.value, a.value, a.value);
        value_type t;
        mpf_init(t);
        mpf_mul(t, b.value, b.value);
        mpf_add(t, r.value, t);
        mpf_sqrt(r.value, t);
        mpf_clear(t);
    }


    template <typename OtherType>
    static inline OtherType big_numeric_cast(value_type const& b)
    {
        return mpf_get_d(b);
    }

    template <typename OtherType>
    static inline OtherType big_numeric_cast(gmp_policy const& b)
    {
        return mpf_get_d(b.value);
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
