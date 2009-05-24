// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_
#define NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_


#include <boost/static_assert.hpp>


namespace numeric_adaptor {


template <typename Policy, typename ToType, bool IsCasted>
struct caster
{
    static inline ToType cast(typename Policy::type const& value)
    {
        return Policy::template big_numeric_cast<ToType>(value);
    }
};

// specialization for strings: casting is not possible, getting is OK
template <typename Policy>
struct caster<Policy, std::string, true>
{
    struct CAST_TO_STRING_IS_NOT_POSSIBLE {};

    // PROHIBITED!
    static inline CAST_TO_STRING_IS_NOT_POSSIBLE cast(typename Policy::type const& )
    {
    }
};

template <typename Policy>
struct caster<Policy, std::string, false>
{
    static inline std::string cast(typename Policy::type const& value)
    {
        return Policy::as_string(value);
    }
};




template <typename Policy>
struct numeric_adaptor
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

    // Cast to normal IEEE type but NOT to std::string because of compilation problems
    template <typename ToType>
    inline operator ToType() const
    {
        return caster<Policy, ToType, true>::cast(value);
    }

    // tuple/fusion/variant-like get template function
    template <typename ToType>
    inline ToType get() const
    {
        return caster<Policy, ToType, false>::cast(value);
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
        typename Policy::type r;
        Policy::init(r);
        Policy::add(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator*(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::multiply(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator-(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::subtract(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator/(
        numeric_adaptor<Policy> const& a,
        numeric_adaptor<Policy> const& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::divide(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    // Functions
    static inline numeric_adaptor<Policy> abs(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::abs(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> sqrt(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::sqrt(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> cos(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::cos(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> sin(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::sin(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> tan(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::tan(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> atan(numeric_adaptor<Policy> const& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::atan(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }


    static inline numeric_adaptor<Policy> hypot(numeric_adaptor<Policy> const& a,
                numeric_adaptor<Policy> const& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::hypot(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }


private :
    typename Policy::type value;

    // Construct from a policy-type. Bool (or any other signature changing parameter)
    // is necessary for cases where type == OtherType
    inline numeric_adaptor<Policy>(typename Policy::type const& v, bool)
    {
        Policy::init(value);
        Policy::copy(v, value);
    }
};


} // namespace numeric_adaptor


#endif
