//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_CHEBYSHEV_FORM_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_CHEBYSHEV_FORM_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace boost {
namespace math {
namespace tools {

  namespace detail {

    template<typename OutputIterator, typename Size>
    void polynomial_chebyshev_even_form(OutputIterator first, const Size sz) {
      typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
      BOOST_ASSERT(sz & 1);
      value_type zero = boost::numeric_cast<value_type>(0.0);
      *first++ = zero;
      value_type coefficient = boost::numeric_cast<value_type>(sz);
      if(sz & 2)
        coefficient = -coefficient;
      *first++ = coefficient;
      for(Size i = 2; i <= sz; ++i) {
        if(i & 1) {
          value_type tmp = boost::numeric_cast<value_type>((sz + i) / 2);
          coefficient = -coefficient * boost::numeric_cast<value_type>(4.0 * (tmp - i + 1) * (tmp - 1)) / boost::numeric_cast<value_type>((i - 1) * i);
          *first++ = coefficient;
        }
        else
          *first++ = zero;
      }
    }

    template<typename OutputIterator, typename Size>
    void polynomial_chebyshev_odd_form(OutputIterator first, Size sz) {
      typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
      BOOST_ASSERT(!(sz & 1));
      value_type zero = boost::numeric_cast<value_type>(0.0);
      value_type coefficient = boost::numeric_cast<value_type>(1.0);
      if(sz & 2)
        coefficient = -coefficient;
      *first++ = coefficient;
      for(Size i = 1; i <= sz; ++i) {
        if(i & 1)
          *first++ = zero;
        else {
          value_type tmp = boost::numeric_cast<value_type>((sz + i) / 2);
          coefficient = -coefficient * boost::numeric_cast<value_type>(4.0 * (tmp - i + 1) * (tmp - 1)) / boost::numeric_cast<value_type>((i - 1) * i);
          *first++ = coefficient;
        }
      }
    }

  } // namespace detail

  template<typename OutputIterator, typename Size>
  inline void polynomial_chebyshev_form(OutputIterator first, const Size sz) {
    if(sz == 0)
      return;
    if(sz & 1)
      detail::polynomial_chebyshev_odd_form(first, sz - 1);
    else
      detail::polynomial_chebyshev_even_form(first, sz - 1);
  }

  template<typename OutputIterator>
  inline void polynomial_chebyshev_form(OutputIterator first, OutputIterator last) {
    if(first == last)
      return;
    polynomial_chebyshev_form(first, std::distance(first, last));
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_CHEBYSHEV_FORM_HPP

