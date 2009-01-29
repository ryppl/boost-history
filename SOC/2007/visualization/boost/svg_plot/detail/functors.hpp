/*! \file functors.hpp
  \brief Functors to convert data to doubles.
  \details SVG plot assumes all data are convertible to double before being plotted.
  The functors are used to convert both 1D and 2D (pairs of data values) to be converted.

  \author Jacob Voytko
*/

// Copyright Jacob Voytko 2007

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_DETAIL_FUNCTORS_HPP
#define BOOST_SVG_DETAIL_FUNCTORS_HPP

namespace boost {
namespace svg {
namespace detail {

class boost_default_convert
{ /*! \class boost::svg::detail::boost_default_convert
      \brief This functor allows any 1D data convertible to doubles to be plotted.
*/
public:
    typedef double result_type;

    template <class T>
    double operator()(T val) const
    {
        return (double)val;
    }
}; // class boost_default_convert

class boost_default_2d_convert
{ /*! \class boost::svg::detail::boost_default_2d_convert
      \brief This functor allows any data convertible to type std::pair<double, double> to be plotted.
*/
public:
    typedef std::pair<double, double> result_type;
    double i;

    void start(double i)
    {
        i = i;
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
}; // class boost_default_2d_convert

} // namespace detail
} // namespace svg
} // namespace boost

#endif // BOOST_SVG_DETAIL_FUNCTORS_HPP
