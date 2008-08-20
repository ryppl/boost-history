#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_SPECIAL_FUNCTIONS_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_SPECIAL_FUNCTIONS_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace boost {
namespace math {
namespace tools {

  namespace detail {

    template<typename T>
    inline std::pair<T, T> split(const T& a) {
      double z = a * 134217729.0;
      double x = z - (z - a);
      return std::make_pair(boost::numeric_cast<T>(x), boost::numeric_cast<T>(a - x));
    }

    template<typename T>
    inline std::pair<T, T> two_sum(const T& a, const T& b) {
      T x = a + b;
      T z = x - a;
      return std::make_pair(x, (a - (x - z)) + (b - z));
    }

    template<typename T>
    inline std::pair<T, T> two_prod(const T& a, const T& b) {
      T x = a * b;
      std::pair<T, T> split_a = split(a);
      std::pair<T, T> split_b = split(b);
      return std::make_pair(x, split_a.second * split_b.second - (((x - split_a.first * split_b.first)
                               - split_a.second * split_b.first) - split_a.first * split_b.second));
    }

  } // namespace detail

  template<typename InputIterator>
  inline typename std::iterator_traits<InputIterator>::value_type
  evaluate_polynomial(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::value_type x) {
    // Horner scheme
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    BOOST_ASSERT(first != last);
    value_type r = *first++;
    while(first != last)
      r = r * x + *first++;
    return r;
  }

  template<typename InputIterator>
  typename std::iterator_traits<InputIterator>::value_type
  evaluate_polynomial_faithfully(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::value_type x) {
    // Compensated Horner algorithm
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    BOOST_ASSERT(first != last);
    std::pair<value_type, value_type> split_x = detail::split(x);
    value_type r = *first++;
    value_type c = boost::numeric_cast<value_type>(0.0);
    while(first != last) {
      value_type p = r * x;
      std::pair<value_type, value_type> split_r = detail::split(r);
      value_type pi = split_r.second * split_x.second - (((p - split_r.first * split_x.first)
                      - split_r.second * split_x.first) - split_r.first * split_x.second);
      r = p + *first;
      value_type t = r - p;
      value_type sigma = (p - (r - t)) + (*first++ - t);
      c = c * x + (pi + sigma);
    }
    return r + c;
  }

  template<typename InputIterator, typename OutputIterator>
  OutputIterator precondition_polynomial_coefficients(InputIterator first, InputIterator last, OutputIterator result) {
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    typedef typename std::iterator_traits<OutputIterator>::value_type result_value_type;
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    difference_type sz = std::distance(first, last);
    BOOST_ASSERT(sz > 4 && sz < 8);
    std::vector<double> c;
    std::transform(first, last, std::back_inserter(c), std::ptr_fun<value_type, double>(boost::numeric_cast));
    std::vector<double> r;
    double tmp1, tmp2, tmp3, tmp4;
    switch(sz) {
      case 5 : r.push_back(0.5 * (c[3] / c[4] - 1.0));
               tmp1 = c[2] / c[4] - r[0] * (r[0] + 1.0);
               r.push_back(c[1] / c[4] - r[0] * tmp1);
               r.push_back(tmp1 - 2.0 * r[1]);
               r.push_back(c[0] / c[4] - r[1] * (r[1] + r[2]));
               r.push_back(c[4]);
               break;
      case 6 : r.push_back(0.5 * (c[4] / c[5] - 1.0));
               tmp1 = c[3] / c[5] - r[0] * (r[0] + 1.0);
               r.push_back(c[2] / c[5] - r[0] * tmp1);
               r.push_back(tmp1 - 2.0 * r[1]);
               r.push_back(c[1] / c[5] - r[1] * (r[1] + r[2]));
               r.push_back(c[5]);
               r.push_back(c[0]);
               break;
      default: BOOST_ASSERT(27.0 * c[3] * c[6] * c[6] - 18.0 * c[4] * c[5] * c[6] + 5.0 * c[5] * c[5] * c[5] != 0.0);
               for(std::vector<double>::iterator it = c.begin(); it != c.end() - 1; ++it)
                 *it /= c.back();
               r.push_back(c[5] / 3.0);
               tmp1 = r[0] * r[0];
               tmp2 = tmp1 * r[0];
               tmp3 = tmp1 * tmp2;
               r.push_back((c[1] - r[0] * c[2] + tmp1 * c[3] - tmp2 * c[4] + 2.0 * tmp3)
                           / (c[3] - 2.0 * r[0] * c[4] + 5.0 * tmp2));
               tmp1 = 2.0 * r[0];
               tmp2 = c[4] - r[0] * tmp1 - r[1];
               tmp3 = c[3] - r[0] * tmp2 - r[1] * tmp1;
               tmp4 = c[2] - r[0] * tmp3 - r[1] * tmp2;
               double r3 = 0.5 * (tmp3 - (r[0] - 1.0) * tmp2 + (r[0] - 1.0) * (r[0] * r[0] - 1.0)) - r[1];
               r.push_back(tmp2 - (r[0] * r[0] - 1.0) - r3 - 2.0 * r[1]);
               r.push_back(r3);
               r.push_back(tmp4 - (r[2] + r[1]) * (r[3] + r[1]));
               r.push_back(c[0] - r[1] * tmp4);
               r.push_back(c[6]);
    }
    return std::transform(r.begin(), r.end(), result, std::ptr_fun<double, result_value_type>(boost::numeric_cast));
  }

  template<typename InputIterator, typename ValueType>
  ValueType evaluate_polynomial_by_preconditioning(InputIterator first, InputIterator last, const ValueType& x) {
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    difference_type sz = std::distance(first, last);
    BOOST_ASSERT(sz > 4 && sz < 8);
    double y, z, r;
    switch(sz) {
      case 5 : y = (x + *first++) * x;
               y += *first++;
               r = (y + x + *first++) * y;
               r += *first++;
               return boost::numeric_cast<ValueType>(r * *first);
      case 6 : y = (x + *first++) * x;
               y += *first++;
               r = (y + x + *first++) * y;
               r += *first++;
               r *= *first++ * x;
               return boost::numeric_cast<ValueType>(r + *first);
      default: z = (x + *first++) * x;
               z += *first++;
               y = z + x + *first++;
               r = (z - x + *first++) * y;
               r = (r + *first++) * z;
               r += *first++;
               return boost::numeric_cast<ValueType>(r * *first);
    }
  }

  template<typename InputIterator, typename OutputIterator>
  OutputIterator copy_polynomial_derivative(InputIterator first, InputIterator last, OutputIterator result) {
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    if(first++ == last)
      return result;
    value_type n = boost::numeric_cast<value_type>(1.0);
    while(first != last) {
      *result++ = *first++ * n;
      n += boost::numeric_cast<value_type>(1.0);
    }
    return result;
  }

  template<typename InputIterator, typename OutputIterator>
  OutputIterator copy_polynomial_integral(InputIterator first, InputIterator last, OutputIterator result,
                                          typename std::iterator_traits<OutputIterator>::value_type c
                                            = (typename std::iterator_traits<OutputIterator>::value_type)(0.0)
                                         ) {
    typedef typename std::iterator_traits<OutputIterator>::value_type result_value_type;
    *result++ = c;
    result_value_type n = boost::numeric_cast<result_value_type>(1.0);
    while(first != last) {
      *result++ = boost::numeric_cast<result_value_type>(*first++) / n;
      n += boost::numeric_cast<result_value_type>(1.0);
    }
    return result;
  }

  template<typename InputIterator>
  inline typename std::iterator_traits<InputIterator>::value_type
  evaluate_polynomial_definite_integral(InputIterator first, InputIterator last,
                                        typename std::iterator_traits<InputIterator>::value_type a,
                                        typename std::iterator_traits<InputIterator>::value_type b
                                       ) {
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    std::vector<double> in(std::distance(first, last) + 1);
    copy_polynomial_integral(first, last, in.begin());
    return boost::numeric_cast<value_type>(evaluate_polynomial_faithfully(in.rbegin(), in.rend(), b)
                                           - evaluate_polynomial_faithfully(in.rbegin(), in.rend(), a));
  }

  template<typename InputIterator, typename OutputIterator>
  OutputIterator interpolate_polynomial(InputIterator first1, InputIterator last1, InputIterator first2, OutputIterator result) {
    // Newton algorithm
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    typedef typename std::iterator_traits<OutputIterator>::value_type result_value_type;
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    typedef typename std::vector<value_type>::size_type size_type;
    std::vector<value_type> x;
    std::vector<double> divided_differences;
    while(first1 != last1) {
      x.push_back(*first1++);
      divided_differences.push_back(boost::numeric_cast<double>(*first2++));
    }
    for(size_type i = 1; i < x.size(); ++i)
      for(size_type j = x.size() - 1; j >= i; --j)
        divided_differences[j] = (divided_differences[j] - divided_differences[j-1]) / (x[j] - x[j-i]);
    for(size_type i = x.size() - 1; i-- != 0;)
      for(size_type j = i; j < divided_differences.size() - 1; ++j)
        divided_differences[j] -= x[i] * divided_differences[j+1];
    std::transform(divided_differences.begin(), divided_differences.end(), result, std::ptr_fun<double, result_value_type>(boost::numeric_cast));
    return result;
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_SPECIAL_FUNCTIONS_HPP

