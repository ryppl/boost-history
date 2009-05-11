// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan, Amsterdam
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_
#define NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_


template <typename Policy>
struct numeric_adaptor
{
    inline numeric_adaptor()
    {
        Policy::init(value);
    }

    // Copy constructor
    inline numeric_adaptor(const numeric_adaptor<Policy>& v)
    {
        Policy::init(value);
        Policy::copy(v.value, value);
    }

    // Constructor with a normal IEEE type
    template <typename T>
    inline numeric_adaptor(const T& v)
    {
        Policy::init(value);
        Policy::template set<T>(value, v);
    }

    virtual ~numeric_adaptor()
    {
        Policy::destruct(value);
    }

    // Assignment from other value
    inline numeric_adaptor<Policy> operator=(const numeric_adaptor<Policy>& v)
    {
        Policy::copy(v.value, this->value);
        return *this;
    }

    // Assignment from normal IEEE type
    template <typename T>
    inline numeric_adaptor<Policy> operator=(const T& v)
    {
        Policy::template set<T>(this->value, v);
        return *this;
    }

    // Cast to normal IEEE type
    template <typename T>
    inline operator T() const
    {
        return Policy::template big_numeric_cast<T>(value);
    }


    // Comparisons
    inline bool operator<(const numeric_adaptor<Policy>& other) const
    {
        return Policy::compare(value, other.value) < 0;
    }

    inline bool operator>(const numeric_adaptor<Policy>& other) const
    {
        return Policy::compare(value, other.value) > 0;
    }

    inline bool operator==(const numeric_adaptor<Policy>& other) const
    {
        return Policy::compare(value, other.value) == 0;
    }

    // Operators
    friend inline numeric_adaptor<Policy> operator+(
        const numeric_adaptor<Policy>& a,
        const numeric_adaptor<Policy>& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::add(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator*(
        const numeric_adaptor<Policy>& a,
        const numeric_adaptor<Policy>& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::multiply(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator-(
        const numeric_adaptor<Policy>& a,
        const numeric_adaptor<Policy>& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::subtract(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    friend inline numeric_adaptor<Policy> operator/(
        const numeric_adaptor<Policy>& a,
        const numeric_adaptor<Policy>& b)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::divide(r, a.value, b.value);
        return numeric_adaptor<Policy>(r, true);
    }

    // Functions
    static inline numeric_adaptor<Policy> sqrt(const numeric_adaptor<Policy>& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::sqrt(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> cos(const numeric_adaptor<Policy>& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::cos(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }

    static inline numeric_adaptor<Policy> sin(const numeric_adaptor<Policy>& v)
    {
        typename Policy::type r;
        Policy::init(r);
        Policy::sin(r, v.value);
        return numeric_adaptor<Policy>(r, true);
    }


private :
    typename Policy::type value;

    // Constructor with a type. Bool (or any other signature changing parameter)
    // is necessary for cases where type == OtherType
    inline numeric_adaptor<Policy>(const typename Policy::type& v, bool)
    {
        Policy::init(value);
        Policy::copy(v, value);
    }
};


#endif
