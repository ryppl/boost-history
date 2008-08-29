//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_HERMITE_FORM_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_HERMITE_FORM_HPP

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
    void polynomial_hermite_even_form(OutputIterator first, const Size sz) {
      typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
      BOOST_ASSERT(sz & 1);
      value_type zero = boost::numeric_cast<value_type>(0.0);
      *first++ = zero;
      value_type coefficient = boost::numeric_cast<value_type>(2.0);
      for(Size i = (sz + 1) / 2; i <= sz; ++i)
        coefficient *= boost::numeric_cast<value_type>(i);
      if(sz & 2)
        coefficient = -coefficient;
      *first++ = coefficient;
      for(Size i = 2; i <= sz; ++i) {
        if(i & 1) {
          coefficient = -coefficient * boost::numeric_cast<value_type>(2.0 * (sz - i + 2.0)) / boost::numeric_cast<value_type>((i - 1) * i);
          *first++ = coefficient;
        }
        else
          *first++ = zero;
      }
    }

    template<typename OutputIterator, typename Size>
    void polynomial_hermite_odd_form(OutputIterator first, const Size sz) {
      typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
      BOOST_ASSERT(!(sz & 1));
      value_type zero = boost::numeric_cast<value_type>(0.0);
      value_type coefficient = boost::numeric_cast<value_type>(1.0);
      for(Size i = (sz + 2) / 2; i <= sz; ++i)
        coefficient *= boost::numeric_cast<value_type>(i);
      if(sz & 2)
        coefficient = -coefficient;
      *first++ = coefficient;
      for(Size i = 1; i <= sz; ++i) {
        if(i & 1)
          *first++ = zero;
        else {
          coefficient = -coefficient * boost::numeric_cast<value_type>(2.0 * (sz - i + 2.0)) / boost::numeric_cast<value_type>((i - 1) * i);
          *first++ = coefficient;
        }
      }
    }

  } // namespace detail

  template<typename OutputIterator, typename Size>
  inline void polynomial_hermite_form(OutputIterator first, const Size sz) {
    typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
    if(sz == 0)
      return;
    if(sz & 1)
      detail::polynomial_hermite_odd_form(first, sz - 1);
    else
      detail::polynomial_hermite_even_form(first, sz - 1);
  }

  template<typename OutputIterator>
  inline void polynomial_hermite_form(OutputIterator first, OutputIterator last) {
    if(first == last)
      return;
    polynomial_hermite_form(first, std::distance(first, last));
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_HERMITE_FORM_HPP

