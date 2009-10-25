// Numeric Adaptor Library
//
// Copyright Bruno Lalande 2009
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NUMERIC_ADAPTOR_GMP_VALUE_TYPE_HPP
#define BOOST_NUMERIC_ADAPTOR_GMP_VALUE_TYPE_HPP


#define NUMERIC_ADAPTOR_INCLUDED


#include <cmath>
#include <ostream>
#include <string>

#include <gmp.h>


namespace boost { namespace numeric_adaptor {


struct gmp_value_type
{
    gmp_value_type()
    {
        mpf_init(m_value);
    }

    gmp_value_type(double v)
    {
        mpf_init(m_value);
        mpf_set_d(m_value, v);
    }

    gmp_value_type(char const* s)
    {
        mpf_init(m_value);
        mpf_set_str(m_value, s, 10);
    }

    gmp_value_type(std::string const& s)
    {
        mpf_init(m_value);
        mpf_set_str(m_value, s.c_str(), 10);
    }


    gmp_value_type(mpf_t& v)
    {
        mpf_init(m_value);
        mpf_set(m_value, v);
    }

    gmp_value_type(gmp_value_type const& other)
    {
        mpf_init(m_value);
        mpf_set(m_value, other.m_value);
    }

    void operator =( gmp_value_type const& other)
    {
        // Copy value if not self-assignment
        if (this != &other)
        {
            mpf_set(m_value, other.m_value);
        }
    }


    ~gmp_value_type()
    {
        mpf_clear(m_value);
    }

    operator double() const
    {
        return mpf_get_d(m_value);
    }

    friend inline gmp_value_type operator+(
        gmp_value_type const& a,
        gmp_value_type const& b)
    {
        gmp_value_type r;
        mpf_add(r.m_value, a.m_value, b.m_value);
        return r;
    }

    gmp_value_type& operator+=(gmp_value_type const& other)
    {
        mpf_add(m_value, m_value, other.m_value);
        return *this;
    }

    friend inline gmp_value_type operator-(
        gmp_value_type const& a,
        gmp_value_type const& b)
    {
        gmp_value_type r;
        mpf_sub(r.m_value, a.m_value, b.m_value);
        return r;
    }

    gmp_value_type& operator-=(gmp_value_type const& other)
    {
        mpf_sub(m_value, m_value, other.m_value);
        return *this;
    }

    friend inline gmp_value_type operator*(
        gmp_value_type const& a,
        gmp_value_type const& b)
    {
        gmp_value_type r;
        mpf_mul(r.m_value, a.m_value, b.m_value);
        return r;
    }

    gmp_value_type& operator*=(gmp_value_type const& other)
    {
        mpf_mul(m_value, m_value, other.m_value);
        return *this;
    }

    friend inline gmp_value_type operator/(
        gmp_value_type const& a,
        gmp_value_type const& b)
    {
        gmp_value_type r;
        mpf_div(r.m_value, a.m_value, b.m_value);
        return r;
    }

    gmp_value_type& operator/=(gmp_value_type const& other)
    {
        mpf_div(m_value, m_value, other.m_value);
        return *this;
    }

    friend inline gmp_value_type operator-(gmp_value_type const& v)
    {
        gmp_value_type r;
        mpf_neg(r.m_value, v.m_value);
        return r;
    }


    template <typename Char, typename Traits>
    friend inline std::basic_ostream<Char, Traits>& operator<<(
            std::basic_ostream<Char, Traits>& os, gmp_value_type const& v)
    {
        mp_exp_t exponent;

        char s[256];
        mpf_get_str(s, &exponent, 10, 0, v.m_value);

        char* p = s;
        if (*p == '-')
        {
            os << "-";
            p++;
        }

        os << "0." << p;
        if (exponent != 0)
        {
             os << "e" << exponent;
        }
        return os;
    }


    mpf_t m_value;
};


template <>
struct to_traits<gmp_value_type, std::string>
{
    static gmp_value_type apply(const std::string& source)
    {
        gmp_value_type result;
        mpf_init(result.m_value);
        mpf_set_str(result.m_value, source.c_str(), 10);
        return result;
    }
};


} // namespace numeric_adaptor


inline numeric_adaptor::gmp_value_type abs(
        numeric_adaptor::gmp_value_type const& v)
{
    numeric_adaptor::gmp_value_type r;
    mpf_abs(r.m_value, v.m_value);
    return r;
}

inline numeric_adaptor::gmp_value_type sqrt(
        numeric_adaptor::gmp_value_type const& v)
{
    numeric_adaptor::gmp_value_type r;
    mpf_sqrt(r.m_value, v.m_value);
    return r;
}

inline numeric_adaptor::gmp_value_type cos(
        numeric_adaptor::gmp_value_type const& v)
{
    return numeric_adaptor::gmp_value_type(std::cos(v));
}

inline numeric_adaptor::gmp_value_type sin(
        numeric_adaptor::gmp_value_type const& v)
{
    return numeric_adaptor::gmp_value_type(std::sin(v));
}

inline numeric_adaptor::gmp_value_type tan(
        numeric_adaptor::gmp_value_type const& v)
{
    return numeric_adaptor::gmp_value_type(std::tan(v));
}

inline numeric_adaptor::gmp_value_type atan(
        numeric_adaptor::gmp_value_type const& v)
{
    return numeric_adaptor::gmp_value_type(std::atan(v));
}

inline numeric_adaptor::gmp_value_type hypot(
    numeric_adaptor::gmp_value_type const& a,
    numeric_adaptor::gmp_value_type const& b)
{
    numeric_adaptor::gmp_value_type r;
    mpf_t aa;
    mpf_init(aa);
    mpf_mul(aa, a.m_value, a.m_value);
    mpf_t bb;
    mpf_init(bb);
    mpf_mul(bb, b.m_value, b.m_value);
    mpf_add(r.m_value, aa, bb);
    mpf_clear(aa);
    mpf_clear(bb);
    mpf_sqrt(r.m_value, r.m_value);
    return r;
}


} // namespace boost


#endif // BOOST_NUMERIC_ADAPTOR_GMP_VALUE_TYPE_HPP

