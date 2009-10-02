// Numeric Adaptor Library
//
// Copyright Bruno Lalande 2009
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NUMERIC_ADAPTOR_CLN_VALUE_TYPE_HPP
#define BOOST_NUMERIC_ADAPTOR_CLN_VALUE_TYPE_HPP

// CLN can be downloaded from http://www.ginac.de/CLN/

#define NUMERIC_ADAPTOR_INCLUDED


#include <ostream>
#include <string>

#include <cln/cln.h>
#include <cln/float.h>


namespace boost { namespace numeric_adaptor {


/*
 CLN documentation, 3.2:
    As a user of CLN, you can forget about the differences between the four
    Floating-point types and just declare all your Foating-point variables
    as being of type cl_F.

*/
struct cln_value_type
{
    cln_value_type()
        : m_value(cln::cl_float(0.0, cln::float_format(256)))
    {}

    cln_value_type(double v)
    {
        // Conversions from the C built-in type `double' are provided for the
        // classes cl_DF, cl_F, cl_R, cl_N and cl_number
        m_value = cln::cl_float(v, cln::float_format(256));
    }

    cln_value_type(char const* s)
        : m_value(cln::cl_float(0.0, cln::float_format(256)))
    {
        // Conversions from `const char *' are provided [...]
        m_value = s;
    }

    cln_value_type(std::string const& s)
        : cln_value_type(s.c_str())
    {
    }


    cln_value_type(cln::cl_F const& v)
        : m_value(v)
    {}

    operator double() const
    {
        return cln::double_approx(m_value);
    }

    friend inline cln_value_type operator+(
        cln_value_type const& a,
        cln_value_type const& b)
    {
        return cln_value_type(a.m_value + b.m_value);
    }

    cln_value_type& operator+=(cln_value_type const& other)
    {
        m_value += other.m_value;
        return *this;
    }

    friend inline cln_value_type operator-(
        cln_value_type const& a,
        cln_value_type const& b)
    {
        return cln_value_type(a.m_value - b.m_value);
    }

    cln_value_type& operator-=(cln_value_type const& other)
    {
        m_value -= other.m_value;
        return *this;
    }

    friend inline cln_value_type operator*(
        cln_value_type const& a,
        cln_value_type const& b)
    {
        return cln_value_type(a.m_value * b.m_value);
    }

    cln_value_type& operator*=(cln_value_type const& other)
    {
        m_value *= other.m_value;
        return *this;
    }

    friend inline cln_value_type operator/(
        cln_value_type const& a,
        cln_value_type const& b)
    {
        return cln_value_type(a.m_value / b.m_value);
    }

    cln_value_type& operator/=(cln_value_type const& other)
    {
        m_value /= other.m_value;
        return *this;
    }

    friend inline cln_value_type operator-(cln_value_type const& v)
    {
        return cln_value_type(-v.m_value);
    }


    template <typename Char, typename Traits>
    friend inline std::basic_ostream<Char, Traits>& operator<<(
            std::basic_ostream<Char, Traits>& os, cln_value_type const& v)
    {
        cln::cl_print_flags flags;
        flags.default_float_format = cln::float_format_dfloat;
        cln::print_float(os, flags, v.m_value);
        return os;
    }




    cln::cl_F m_value;
};


} // namespace numeric_adaptor


inline numeric_adaptor::cln_value_type abs(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::abs(v.m_value));
}

inline numeric_adaptor::cln_value_type sqrt(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::sqrt(v.m_value));
}

inline numeric_adaptor::cln_value_type cos(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::cos(v.m_value));
}

inline numeric_adaptor::cln_value_type sin(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::sin(v.m_value));
}

inline numeric_adaptor::cln_value_type tan(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::tan(v.m_value));
}

inline numeric_adaptor::cln_value_type atan(
        numeric_adaptor::cln_value_type const& v)
{
    return numeric_adaptor::cln_value_type(cln::atan(v.m_value));
}

inline numeric_adaptor::cln_value_type hypot(
    numeric_adaptor::cln_value_type const& a,
    numeric_adaptor::cln_value_type const& b)
{
    return numeric_adaptor::cln_value_type(
               cln::sqrt(a.m_value*a.m_value + b.m_value*b.m_value)
           );
}


} // namespace boost


#endif // BOOST_NUMERIC_ADAPTOR_CLN_VALUE_TYPE_HPP
