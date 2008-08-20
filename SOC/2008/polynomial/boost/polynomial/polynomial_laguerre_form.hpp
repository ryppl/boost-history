#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_LAGUERRE_FORM_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_LAGUERRE_FORM_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/numeric/conversion/cast.hpp>

namespace boost {
namespace math {
namespace tools {

  template<typename OutputIterator, typename Size>
  void polynomial_laguerre_form(OutputIterator first, Size sz) {
    typedef typename std::iterator_traits<OutputIterator>::value_type value_type;
    if(sz == 0)
      return;
    double coefficient = 1.0;
    *first++ = boost::numeric_cast<value_type>(coefficient);
    for(Size i = 1; i < sz; ++i) {
      coefficient = -coefficient * (sz - i) / (i * i);
      *first++ = boost::numeric_cast<value_type>(coefficient);
    }
  }

  template<typename OutputIterator>
  inline void polynomial_laguerre_form(OutputIterator first, OutputIterator last) {
    if(first == last)
      return;
    polynomial_laguerre_form(first, std::distance(first, last));
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_LAGUERRE_FORM_HPP

