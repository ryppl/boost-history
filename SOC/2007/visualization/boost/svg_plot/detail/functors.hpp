// functors.hpp
// Copyright (C) Jacob Voytko 2007
// Distributed under the Boost Software License, Version 1.0.
// For more information, see http://www.boost.org
// ----------------------------------------------------------------- 

#ifndef _BOOST_SVG_DETAIL_FUNCTORS_HPP
#define _BOOST_SVG_DETAIL_FUNCTORS_HPP

namespace boost {
namespace svg {
namespace detail {

// -----------------------------------------------------------------
// This functor allows any data convertible to doubles to be plotted
// -----------------------------------------------------------------
class boost_default_convert
{
public:
    typedef double result_type;

    template <class T>
    double operator()(T val) const
    {
        return (double)val;
    }
};

// -----------------------------------------------------------------
// This functor allows any data convertible to type
// std::pair<double, double> to be plot
// -----------------------------------------------------------------
class boost_default_2d_convert
{
public:
    typedef std::pair<double, double> result_type;

    double i;

    void start(double _i)
    {
        i = _i;
    }

    template <class T, class U>
    std::pair<double, double> operator()(const std::pair<T, U>& a) const
    {
        return std::pair<double, double>((double)(a.first), (double)(a.second));
    }

    template <class T>
    std::pair<double, double> operator()(T a)
    {
        return std::pair<double, double>(i++, (double)a);
    }
};

}
}
}

#endif