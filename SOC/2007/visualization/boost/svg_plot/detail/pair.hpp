/*! \file pair.hpp
    \brief Provides a private implementation of operator<< for std::pair that
      outputs pairs with a comma separated format, for example: 1.2, 3.4
    \details Hidden in namespace detail to avoid clashes with other implementations of std::pair operator<<.
    \date Mar 2009
    \author Paul A. Bristow
*/

// Copyright Paul A. Bristow 2006 - 2009.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SVG_PAIR
#define BOOST_SVG_PAIR

#include <boost/svg_plot/uncertain.hpp>
// using boost::svg::unc;

#include<iostream>
 // using std::ostream;

#include <utility>
  //using std::pair;
  //using std::make_pair;

namespace boost
{
namespace svg
{
namespace detail
{

  // Hidden in namespace detail to avoid clashes with other implementations of std::pair operator<<.
  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p);
  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p);
 // std::ostream& operator<< (std::ostream& os, const std::pair<boost::svg::unc, boost::svg::unc>& p);

  template<class T1, class T2>
  std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2>& p)
  { // Output a pair of values.
         os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      return os;
  } // std::ostream& operator<<

  std::ostream& operator<< (std::ostream& os, const std::pair<double, double>& p)
  { // Output a pair of double values.
      //int precision = os.precision(5); // Save & use rather than default precision(6).
      os << p.first << ", " << p.second;
      // Outputs:  1.2, 3.4
      //os.precision(precision); // Restore.
      return os;
  } // std::ostream& operator<<

 



  //std::pair<class T1, class T2>& operator=(const pair& rhs)
  //{
  //  first = rhs.first;
  //  second = rhs.second;
  //  return *this; //! to make chainable.
  //}

  // Maybe better as:
  //template<typename charT, typename traits, typename T1, typename T2>
  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<T1, T2>& p)
  //{
  //  return os << p.first << ", " << p.second;
  //}
  //
  //// Explicit double, double.
  //template<typename charT, typename traits>
  //inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const std::pair<double, double>& p)
  //{
  //  return os << p.first << ", " << p.second;
  //}
  // but OK for this purpose.
} // namespace detail

} // namespace svg
} // namespace boost

#endif // BOOST_SVG_PAIR_HPP



