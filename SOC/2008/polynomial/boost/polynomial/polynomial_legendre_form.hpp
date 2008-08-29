//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_LEGENDRE_FORM_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_LEGENDRE_FORM_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>
#include <boost/numeric/conversion/cast.hpp>

namespace boost {
namespace math {
namespace tools {

  // Creating Legendre polynomials using recurrence:
  template<typename OutputIterator, typename Size>
  void polynomial_legendre_form(OutputIterator first, Size sz) {
    typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
    if(sz == 0)
      return;
    if(sz == 1) {
      *first = boost::numeric_cast<value_type>(1.0);
      return;
    }
    value_type zero = boost::numeric_cast<value_type>(0.0);
    if(sz == 2) {
      *first++ = zero;
      *first = boost::numeric_cast<value_type>(1.0);
      return;
    }
    std::vector<double> p1;
    p1.push_back(1.0);
    std::vector<double> p2(p1);
    for(Size i = 2; i < sz; ++i) {
      double di = boost::numeric_cast<double>(i);
      if(i & 1) {
        typename std::vector<double>::iterator p2_it = p2.begin();
        double tmp1 = *p2_it;
        *p2_it = ((2.0 * di - 1.0) * *p2_it) / di;
        ++p2_it;
        typename std::vector<double>::iterator p1_it = p1.begin();
        while(p1_it != p1.end()) {
          double tmp2 = *p2_it;
          *p2_it = ((2.0 * di - 1.0) * *p2_it - (di - 1.0) * *p1_it) / di;
          ++p2_it;
          *p1_it++ = tmp1;
          tmp1 = tmp2;
        }
        p1.push_back(tmp1);
      }
      else {
        p2.push_back(zero);
        typename std::vector<double>::iterator p2_it = p2.begin();
        double tmp1 = *p2_it;
        *p2_it = ((2.0 * di - 1.0) * *p2_it) / di;
        ++p2_it;
        typename std::vector<double>::iterator p1_it = p1.begin();
        while(p1_it != p1.end()) {
          double tmp2 = *p2_it;
          *p2_it = ((2.0 * di - 1.0) * *p2_it - (di - 1.0) * *p1_it) / di;
          ++p2_it;
          *p1_it++ = tmp1;
          tmp1 = tmp2;
        }
      }
    }
    if(sz & 1) {
      for(typename std::vector<double>::reverse_iterator it = p2.rbegin(); it != p2.rend(); ++it) {
        if(it != p2.rbegin())
          *first++ = zero;
        *first++ = boost::numeric_cast<value_type>(*it);
      }
    }
    else {
      for(typename std::vector<double>::reverse_iterator it = p2.rbegin(); it != p2.rend(); ++it) {
        *first++ = zero;
        *first++ = boost::numeric_cast<value_type>(*it);
      }
    }
  }

  template<typename OutputIterator>
  inline void polynomial_legendre_form(OutputIterator first, OutputIterator last) {
    if(first == last)
      return;
    polynomial_legendre_form(first, std::distance(first, last));
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_LEGENDRE_FORM_HPP

