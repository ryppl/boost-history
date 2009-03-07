/*! \file functors.hpp
  \brief Functors to convert data to doubles.
  \details SVG plot assumes all data are convertible to double or uncertain value type unc before being plotted.
    The functors are used to convert both 1D and 2D (pairs of data values) to be converted.
    Note that uncertain value class unc only holds double precision and long double data
    will therefore lose information.  This seems reasonable design decision as any real data
    to be plotted is unlikely to have more than double precision (about 16 decimal digits).

  \author Jacob Voytko and Paul A. Bristow
  \date Mar 2009
*/

// Copyright Jacob Voytko 2007
// Copyright Paul A. Bristow 2009

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_DETAIL_FUNCTORS_HPP
#define BOOST_SVG_DETAIL_FUNCTORS_HPP

#if defined (_MSC_VER)
#  pragma warning (push)
#  pragma warning (disable : 4244)
#endif

#include "..\uncertain.hpp"
//using boost::svg::unc;

namespace boost {
namespace svg {
namespace detail
{

class double_1d_convert
{ /*! \class boost::svg::detail::double_1d_convert
      \brief This functor allows any 1D data convertible to doubles to be plotted.
*/
public:
    typedef double result_type;

    template <class T>
    double operator()(T val) const
    {
        return (double)val;
    }
}; // class double_1d_convert

class pair_double_2d_convert
{ /*! \class boost::svg::detail::pair_double_2d_convert
      \brief This functor allows any 2 D data convertible to type std::pair<double, double> to be plotted.
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
}; // class pair_double_2d_convert

class pair_unc_2d_convert
{ /*! \class boost::svg::detail::pair_unc_2d_convert
      \brief This functor allows any 2D data convertible to type std::pair<unc, unc> to be plotted.
*/
public:
    typedef std::pair<unc, unc> result_type;
    unc i;
    void start(unc i)
    {
        i = i;
    }

    template <class T, class U>
    std::pair<unc, unc> operator()(const std::pair<T, U>& a) const
    {
        return std::pair<unc, unc>((unc)(a.first), (unc)(a.second));
    }

    template <class T>
    std::pair<unc, unc> operator()(T a)
    {
        return std::pair<unc, unc>(i++, (unc)a);
    }
}; // class pair_unc_2d_convert

class unc_1d_convert
{ /*! \class boost::svg::detail::unc_1d_convert
      \brief This functor allows any 1D data convertible to unc (uncertain doubles) to be plotted.
      \details Defaults provided by the unc class constructor ensure that
      uncertainty, degrees of freedom information, and type are set too.
*/
public:

    typedef unc result_type;

    template <class T>
    unc operator()(T val) const
    {
      return (unc)val;
      // warning C4244: 'argument' : conversion from 'long double' to 'double', possible loss of data.
      // because unc only holds double precision.
    }
}; // class default_1d_convert


} // namespace detail
} // namespace svg
} // namespace boost

#if defined (BOOST_MSVC)
#  pragma warning(pop)
#endif

#endif // BOOST_SVG_DETAIL_FUNCTORS_HPP
