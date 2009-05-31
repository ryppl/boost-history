// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP
#define NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP


#include <boost/static_assert.hpp>

#include <string>

namespace boost { namespace numeric_adaptor {


template <typename Base, typename T>
struct enable_cast
{
    inline operator T()
    {
        return static_cast<Base*>(this)->template big_numeric_cast<T>();
    }
};




template <typename Policy>
struct numeric_adaptor :
    enable_cast<numeric_adaptor<Policy>, char>,
    enable_cast<numeric_adaptor<Policy>, unsigned char>,
    enable_cast<numeric_adaptor<Policy>, int>,
    enable_cast<numeric_adaptor<Policy>, unsigned int>,
    enable_cast<numeric_adaptor<Policy>, short int>,
    enable_cast<numeric_adaptor<Policy>, unsigned short int>,
    enable_cast<numeric_adaptor<Policy>, long int>,
    enable_cast<numeric_adaptor<Policy>, unsigned long int>,
    enable_cast<numeric_adaptor<Policy>, float>,
    enable_cast<numeric_adaptor<Policy>, double>,
    enable_cast<numeric_adaptor<Policy>, long double>
{
    inline numeric_adaptor()
    {
        Policy::init(value);
    }

    // Copy constructor
    inline numeric_adaptor(numeric_adaptor<Policy> const& v)
    {
        Policy::init(value);
        Policy::copy(v.value, value);
    }

    // Constructor from a string
    inline numeric_adaptor(std::string const& v)
    {
        Policy::init(value);
        Policy::set(value, v);
    }

    inline numeric_adaptor(const char* v)
    {
        Policy::init(value);
        Policy::set(value, std::string(v));
    }

    // Constructor with a normal IEEE type
    template <typename FromType>
    inline numeric_adaptor(FromType const& v)
    {
        Policy::init(value);
        Policy::template set<FromType>(value, v);
    }


    virtual ~numeric_adaptor()
    {
        Policy::destruct(value);
    }

    // Assignment from other value
    inline numeric_adaptor<Policy> operator=(numeric_adaptor<Policy> const& v)
    {
        Policy::copy(v.value, this->value);
        return *this;
    }

    // Assignment from normal IEEE type
    template <typename FromType>
    inline numeric_adaptor<Policy> operator=(FromType const& v)
    {
        Policy::template set<FromType>(this->value, v);
        return *this;
    }

    template <class ToType>
    inline ToType big_numeric_cast()
    {
        return Policy::template big_numeric_cast<ToType>(this->value);
    }

    // tuple/fusion/variant-like get template function
    inline operator std::string()
    {
        return Policy::as_string(this->value);
    }


    // Comparisons
    inline bool operator<(numeric_adaptor<Policy> const& other) const
    {
        return Policy::compare(value, other.value) < 0;
    }

    inline bool operator>(numeric_adaptor<Policy> const& other) const
    {
        return Policy::compare(value, other.value) > 0;
    }

    inline bool operator==(numeric_adaptor<Policy> const& other) const
    {
        return Policy::compare(value, other.value) == 0;
    }

    // Operators
    friend inline numeric_adaptor<Policy> operator+(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::add(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    numeric_adaptor<Policy>& operator+=(numeric_adaptor<Policy> const& other)
    {
        Policy::add(value, other.value);
        return *this;
    }

    friend inline numeric_adaptor<Policy> operator*(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::multiply(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    numeric_adaptor<Policy>& operator*=(numeric_adaptor<Policy> const& other)
    {
        Policy::multiply(value, other.value);
        return *this;
    }

    friend inline numeric_adaptor<Policy> operator-(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::subtract(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    numeric_adaptor<Policy>& operator-=(numeric_adaptor<Policy> const& other)
    {
        Policy::subtract(value, other.value);
        return *this;
    }

    friend inline numeric_adaptor<Policy> operator/(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::divide(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    numeric_adaptor<Policy>& operator/=(numeric_adaptor<Policy> const& other)
    {
        Policy::divide(value, other.value);
        return *this;
    }

    friend inline numeric_adaptor<Policy> operator-(numeric_adaptor<Policy> const& n)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::neg(r, n.value);
        return numeric_adaptor<Policy>(r, true);
    }

    // Functions
    static inline numeric_adaptor<Policy> abs(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::abs(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> sqrt(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::sqrt(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> cos(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::cos(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> sin(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::sin(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> tan(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::tan(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> atan(numeric_adaptor<Policy> const& v)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::atan(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }


    static inline numeric_adaptor<Policy> hypot(numeric_adaptor<Policy> const& a,
                numeric_adaptor<Policy> const& b)
    {
        typename Policy::value_type r;
        Policy::init(r);
        Policy::hypot(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }


private :
    typename Policy::value_type value;

    // Construct from a policy-type. Bool (or any other signature changing parameter)
    // is necessary for cases where type == OtherType
    inline numeric_adaptor<Policy>(typename Policy::value_type const& v, bool)
    {
        Policy::init(value);
        Policy::copy(v, value);
    }
};


}} // namespace boost::numeric_adaptor


#endif
