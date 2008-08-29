//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_POLYNOMIAL_HPP
#define BOOST_MATH_TOOLS_POLYNOMIAL_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <algorithm>
#include <complex>
#include <functional>
#include <iosfwd>
#include <vector>
#include <boost/assert.hpp>
#include <boost/math/common_factor.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/math/special_functions/round.hpp>
#include "polynomial/fast_fourier_transform.hpp"
#include "polynomial/polynomial_special_forms.hpp"
#include "polynomial/polynomial_special_functions.hpp"

namespace boost {
namespace math {
namespace tools {

  template<typename FieldType>
  class polynomial;
  template<typename IntegerType>
  polynomial<IntegerType> gcd(polynomial<IntegerType> u, polynomial<IntegerType> v);

  namespace detail {

    template<typename InputIterator>
    inline typename std::iterator_traits<InputIterator>::value_type cont_evaluate(InputIterator first, InputIterator last) {
      BOOST_ASSERT(first != last);
      typename std::iterator_traits<InputIterator>::value_type cont = *first++;
      while(first != last)
        cont = boost::math::gcd(cont, *first++);
      return cont;
    }

    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void polynomial_add_in_place(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
      while(first1 != last1 && first2 != last2)
        *result++ = (*first1++) + (*first2++);
      while(first1 != last1)
        *result++ = *first1++;
      while(first2 != last2)
        *result++ = *first2++;
    }

    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void polynomial_sub_in_place(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
      while(first1 != last1 && first2 != last2)
        *result++ = (*first1++) - (*first2++);
      while(first1 != last1)
        *result++ = *first1++;
      while(first2 != last2)
        *result++ = -(*first2++);
    }

    template<typename IntegerType>
    polynomial<IntegerType> modulo_for_gcd(std::vector<IntegerType> u, const std::vector<IntegerType>& v) {
      typedef typename polynomial<IntegerType>::size_type size_type;
      if(u.size() < v.size())
        return u;
      if(v.size() == 1)
        return polynomial<IntegerType>();
      size_type k = u.size() - v.size();
      size_type n = v.size() - 1;
      do {
        size_type j = n + k - 1;
        do {
          if(j < k)
            u[j] = v[n] * u[j];
          else
            u[j] = v[n] * u[j] - u[n + k] * v[j - k];
        }
        while(j--);
      }
      while(k--);
      while(u.size() >= v.size())
        u.pop_back();
      while(u.size() > 1 && u.back() == boost::numeric_cast<IntegerType>(0))
        u.pop_back();
      return polynomial<IntegerType>(u);
    }

  } // namespace detail

  template<typename FieldType>
  class polynomial {
    std::vector<FieldType> coefficients;
    std::vector<double> preconditions;
    template<typename U>
    U round(const U& v);
    void normalize();
    template<typename U>
    static FieldType cvt(U const& n) {
      return boost::numeric_cast<FieldType>(n);
    }
    static FieldType zero() {
      return cvt(0.0);
    }
    friend polynomial<FieldType> boost::math::tools::gcd<>(polynomial<FieldType> u, polynomial<FieldType> v);
  public:
    // typedefs:
    typedef typename std::vector<FieldType>::value_type value_type;
    typedef typename std::vector<FieldType>::size_type size_type;

    // constructors:
    polynomial() {
      coefficients.push_back(zero());
    }
    template<typename InputIterator>
    polynomial(InputIterator first, InputIterator last) : coefficients(first, last) {
      normalize();
    }
    template<typename U>
    polynomial(const U* coef, const size_type n) : coefficients(coef, coef + n) {
      normalize();
    }
    template<typename U>
    polynomial(const U& v) {
      coefficients.push_back(v);
      normalize();
    }
    template<typename InputIterator>
    polynomial(InputIterator first1, InputIterator last1, InputIterator first2) : coefficients(std::distance(first1, last1)) {
      boost::math::tools::interpolate_polynomial(first1, last1, first2, coefficients.begin());
      normalize();
    }
    polynomial(std::vector<FieldType>& c) {
      coefficients.swap(c);
      normalize();
    }

    // copy constructor:
    template<typename U>
    polynomial(const polynomial<U>& poly) {
      std::transform(poly.begin(), poly.end(), std::back_inserter(coefficients), std::ptr_fun<U, FieldType>(boost::numeric_cast));
      normalize();
    }

    // modification:
    template<typename U>
    polynomial<FieldType>& operator=(const polynomial<U>& poly);
    polynomial<FieldType>& operator=(std::vector<FieldType>& c) {
      coefficients.swap(c);
      normalize();
      return *this;
    }
    template<typename InputIterator>
    void interpolate(InputIterator first1, InputIterator last1, InputIterator first2) {
      coefficients.resize(std::distance(first1, last1));
      boost::math::tools::interpolate_polynomial(first1, last1, first2, coefficients.begin());
      normalize();
    }
    void swap(polynomial<FieldType>& poly) {
      coefficients.swap(poly.coefficients);
    }

    // access:
    size_type size() const {
      return coefficients.size();
    }
    size_type degree() const {
      return coefficients.size() - 1;
    }
    const value_type& operator[](const size_type i) const {
      return coefficients[i];
    }
    value_type& operator[](const size_type i) {
      return coefficients[i];
    }
    typename std::vector<FieldType>::const_iterator begin() const {
      return coefficients.begin();
    }
    typename std::vector<FieldType>::iterator begin() {
      return coefficients.begin();
    }
    typename std::vector<FieldType>::const_iterator end() const {
      return coefficients.end();
    }
    typename std::vector<FieldType>::iterator end() {
      return coefficients.end();
    }

    // evaluation:
    FieldType operator()(const FieldType& z) const {
      return boost::math::tools::evaluate_polynomial(coefficients.rbegin(), coefficients.rend(), z);
    }
    FieldType evaluate(const FieldType& z) const {
      return boost::math::tools::evaluate_polynomial(coefficients.rbegin(), coefficients.rend(), z);
    }
    FieldType evaluate_faithfully(const FieldType& z) const {
      return boost::math::tools::evaluate_polynomial_faithfully(coefficients.rbegin(), coefficients.rend(), z);
    }
    void precondition_coefficients() {
      BOOST_ASSERT(size() > 4 && size() < 8);
      preconditions.resize(size());
      boost::math::tools::precondition_polynomial_coefficients(begin(), end(), preconditions.begin());
    }
    FieldType evaluate_by_preconditioning(const FieldType& z) const {
      return boost::math::tools::evaluate_polynomial_by_preconditioning(preconditions.begin(), preconditions.end(), z);
    }
    // derivatives and integrals:
    polynomial<FieldType> derivative() const;
    polynomial<FieldType> integral(const FieldType& c) const;
    FieldType definite_integral(const FieldType& a, const FieldType& b) const;

    // operators:

    const polynomial<FieldType>& operator+() const;
    polynomial<FieldType> operator-() const;

    // addition:
    template<typename U>
    polynomial<FieldType>& operator+=(const polynomial<U>& poly);
    template<typename U>
    polynomial<FieldType>& operator+=(const U& v);

    // substraction:
    template<typename U>
    polynomial<FieldType>& operator-=(const polynomial<U>& poly);
    template<typename U>
    polynomial<FieldType>& operator-=(const U& v);

    // multiplication:
    template<typename U>
    polynomial<FieldType>& operator*=(const polynomial<U>& poly);
    template<typename U>
    polynomial<FieldType>& operator*=(const U& v);

    // division:
    template<typename U>
    polynomial<FieldType>& operator/=(const polynomial<U>& poly);
    template<typename U>
    polynomial<FieldType>& operator/=(const U& v);

    // modulo:
    template<typename U>
    polynomial<FieldType>& operator%=(const polynomial<U>& poly);
  };

  template<typename FieldType>
  template<typename U>
  inline U polynomial<FieldType>::round(const U& v) {
    return boost::math::round(v);
  }

  // specializations for floating-point types (rounding shouldn't be done):
  template<>
  template<typename U>
  inline U polynomial<float>::round(const U& v) {
    return v;
  }

  template<>
  template<typename U>
  inline U polynomial<double>::round(const U& v) {
    return v;
  }

  template<>
  template<typename U>
  inline U polynomial<long double>::round(const U& v) {
    return v;
  }


  template<typename FieldType>
  inline void polynomial<FieldType>::normalize() {
    while(coefficients.size() > 1 && coefficients.back() == zero())
      coefficients.pop_back();
    if(coefficients.empty())
      coefficients.push_back(zero());
  }

  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator=(const polynomial<U>& poly) {
    if(static_cast<void const*>(this) != static_cast<void const*>(&poly)) {
      std::vector<FieldType> tmp(poly.size());
      typename std::vector<FieldType>::iterator tmp_it = tmp.begin();
      typename std::vector<U>::const_iterator poly_it = poly.begin();
      while(tmp_it != tmp.end())
        *tmp_it++ = cvt(*poly_it++);
      coefficients.swap(tmp);
      normalize();
    }
    return *this;
  }


  // derivatives and integrals:
  template<typename FieldType>
  inline polynomial<FieldType> polynomial<FieldType>::derivative() const {
    std::vector<FieldType> tmp(degree());
    boost::math::tools::copy_polynomial_derivative(coefficients.begin(), coefficients.end(), tmp.begin());
    return polynomial<FieldType>(tmp.begin(), tmp.end());
  }

  template<typename FieldType>
  inline polynomial<FieldType> polynomial<FieldType>::integral(const FieldType& c = FieldType(0.0)) const {
    std::vector<FieldType> tmp(size() + 1);
    boost::math::tools::copy_polynomial_integral(coefficients.begin(), coefficients.end(), tmp.begin(), c);
    return polynomial<FieldType>(tmp.begin(), tmp.end());
  }

  template<typename FieldType>
  inline FieldType polynomial<FieldType>::definite_integral(const FieldType& a, const FieldType& b) const {
    return boost::math::tools::evaluate_polynomial_definite_integral(coefficients.begin(), coefficients.end(), a, b);
  }


  // addition:
  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator+=(const polynomial<U>& poly) {
    std::vector<FieldType> tmp(std::max(size(), poly.size()));
    boost::math::tools::detail::polynomial_add_in_place(begin(), end(), poly.begin(), poly.end(), tmp.begin());
    std::swap(coefficients, tmp);
    normalize();
    return *this;
  }

  template<typename FieldType>
  template<typename U>
  inline polynomial<FieldType>& polynomial<FieldType>::operator+=(const U& v) {
    coefficients.front() += cvt(v);
    return *this;
  }

  // substraction:
  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator-=(const polynomial<U>& poly) {
    if(static_cast<void const*>(this) != static_cast<void const*>(&poly)) {
      std::vector<FieldType> tmp(std::max(size(), poly.size()));
      boost::math::tools::detail::polynomial_sub_in_place(begin(), end(), poly.begin(), poly.end(), tmp.begin());
      std::swap(coefficients, tmp);
      normalize();
    }
    else {
      coefficients.clear();
      coefficients.push_back(zero());
    }
    return *this;
  }

  template<typename FieldType>
  template<typename U>
  inline polynomial<FieldType>& polynomial<FieldType>::operator-=(const U& v) {
    coefficients.front() -= cvt(v);
    return *this;
  }

  // multiplication:
  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator*=(const polynomial<U>& poly) {
    double l = log2(std::max(coefficients.size(), poly.size()) * 2);
    size_type new_sz = static_cast<size_type>(floor(l));
    if(l > new_sz)
      new_sz = static_cast<size_type>(pow(2.0, static_cast<int>(new_sz + 1)));
    else
      new_sz = static_cast<size_type>(pow(2.0, static_cast<int>(new_sz)));
    std::vector<std::complex<double> > dft1, dft2;
    dft1.reserve(new_sz);
    dft2.reserve(new_sz);
    std::transform(begin(), end(), std::back_inserter(dft1), std::ptr_fun<double, FieldType>(boost::numeric_cast));
    std::fill_n(std::back_inserter(dft1), new_sz - dft1.size(), zero());
    std::transform(poly.begin(), poly.end(), std::back_inserter(dft2), std::ptr_fun<double, FieldType>(boost::numeric_cast));
    std::fill_n(std::back_inserter(dft2), new_sz - dft2.size(), zero());
    boost::math::tools::fast_fourier_transform(dft1.begin(), dft1.end(), dft1.begin());
    boost::math::tools::fast_fourier_transform(dft2.begin(), dft2.end(), dft2.begin());
    std::transform(dft1.begin(), dft1.end(), dft2.begin(), dft1.begin(), std::multiplies<std::complex<double> >());
    boost::math::tools::inverse_fast_fourier_transform(dft1.begin(), dft1.end(), dft1.begin());
    coefficients.resize(coefficients.size() + poly.size() - 1);
    typename std::vector<std::complex<double> >::iterator dft1_it = dft1.begin();
    typename std::vector<FieldType>::iterator c_it = begin();
    // probably std::transform might be used here but I can't make it work properly:
    while(c_it != end())
      *c_it++ = cvt(this->round((dft1_it++)->real()));
    normalize();
    return *this;
  }

  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator*=(const U& v) {
    std::vector<FieldType> tmp;
    for(typename std::vector<FieldType>::iterator c_it = begin(); c_it != end(); ++c_it)
      tmp.push_back(cvt(*c_it * v));
    std::swap(coefficients, tmp);
    normalize();
    return *this;
  }

  // division:
  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator/=(const polynomial<U>& poly) {
    if(static_cast<void const*>(this) != static_cast<void const*>(&poly)) {
      std::vector<FieldType> tmp(coefficients);
      if(size() < poly.size()) {
        tmp.clear();
        tmp.push_back(zero());
        std::swap(coefficients, tmp);
        return *this;
      }
      size_type k = tmp.size() - poly.size();
      std::vector<double> new_coefficients;
      new_coefficients.resize(k + 1);
      while(true) {
        new_coefficients[k] = boost::numeric_cast<double>(tmp[poly.size() + k - 1]) / poly.coefficients.back();
        for(size_type j = poly.size() + k - 1; j > k; --j)
          tmp[j] = cvt(tmp[j] - new_coefficients[k] * poly[j - k]);
        tmp[k] = cvt(tmp[k] - new_coefficients[k] * poly[0]);
        if(k-- == 0)
          break;
      }
      tmp.resize(new_coefficients.size());
      std::transform(new_coefficients.begin(), new_coefficients.end(), tmp.begin(), std::ptr_fun<double, FieldType>(boost::numeric_cast));
      std::swap(coefficients, tmp);
      normalize();
    }
    else {
      coefficients.clear();
      coefficients.push_back(cvt(1.0));
    }
    return *this;
  }

  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator/=(const U& v) {
    std::vector<FieldType> tmp;
    for(typename std::vector<FieldType>::iterator c_it = begin(); c_it != end(); ++c_it)
      tmp.push_back(cvt(*c_it / v));
    std::swap(coefficients, tmp);
    normalize();
    return *this;
  }

  // modulo:
  template<typename FieldType>
  template<typename U>
  polynomial<FieldType>& polynomial<FieldType>::operator%=(const polynomial<U>& poly) {
    if(static_cast<void const*>(this) != static_cast<void const*>(&poly)) {
      if(size() < poly.size()) {
        return *this;
      }
      std::vector<FieldType> tmp(coefficients);
      size_type k = tmp.size() - poly.size();
      double q;
      while(true) {
        q = boost::numeric_cast<double>(tmp[poly.size() + k - 1]) / poly.coefficients.back();
        for(size_type j = poly.size() + k - 1; j > k; --j)
          tmp[j] = cvt(tmp[j] - q * poly[j - k]);
        tmp[k] = cvt(tmp[k] - q * poly[0]);
        if(k-- == 0)
          break;
      }
      while(tmp.size() >= poly.size())
        tmp.pop_back();
      std::swap(coefficients, tmp);
      normalize();
    }
    else {
      coefficients.clear();
      coefficients.push_back(zero());
    }
    return *this;
  }

  // greatest common divisor:
  template<typename IntegerType>
  polynomial<IntegerType> gcd(polynomial<IntegerType> u, polynomial<IntegerType> v) {
    IntegerType cont_u = boost::math::tools::detail::cont_evaluate(u.begin(), u.end());
    u /= cont_u;
    IntegerType cont_v = boost::math::tools::detail::cont_evaluate(v.begin(), v.end());
    v /= cont_v;
    while(true) {
      polynomial<IntegerType> r = boost::math::tools::detail::modulo_for_gcd(u.coefficients, v.coefficients);
      if(r.size() == 1) {
        if(*(r.begin()) == r.zero())
          break;
        v = polynomial<IntegerType>(boost::numeric_cast<IntegerType>(1.0));
        break;
      }
      u = v;
      v = r / boost::math::tools::detail::cont_evaluate(r.begin(), r.end());
    }
    if(v.coefficients.back() < v.zero())
      return -boost::math::gcd(cont_u, cont_v) * v;
    return boost::math::gcd(cont_u, cont_v) * v;
  }


  // operators:

  template<typename FieldType>
  inline const polynomial<FieldType>& polynomial<FieldType>::operator+() const {
    return *this;
  }

  template<typename FieldType>
  inline polynomial<FieldType> polynomial<FieldType>::operator-() const {
    std::vector<FieldType> tmp;
    tmp.reserve(size());
    std::transform(begin(), end(), std::back_inserter(tmp), std::negate<FieldType>());
    return polynomial<FieldType>(tmp);
  }

  // addition:
  template<typename FieldType>
  inline polynomial<FieldType> operator+(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    polynomial<FieldType> result(left);
    result += right;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator+(const polynomial<FieldType>& poly, const U& val) {
    polynomial<FieldType> result(poly);
    result += val;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator+(const U& val, const polynomial<FieldType>& poly) {
    polynomial<FieldType> result(poly);
    result += val;
    return result;
  }

  // substraction:
  template<typename FieldType>
  inline polynomial<FieldType> operator-(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    polynomial<FieldType> result(left);
    result -= right;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator-(const polynomial<FieldType>& poly, const U& val) {
    polynomial<FieldType> result(poly);
    result -= val;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator-(const U& val, const polynomial<FieldType>& poly) {
    polynomial<FieldType> result(-poly);
    result += val;
    return result;
  }

  // multiplication:
  template<typename FieldType>
  inline polynomial<FieldType> operator*(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    polynomial<FieldType> result(left);
    result *= right;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator*(const polynomial<FieldType>& poly, const U& val) {
    polynomial<FieldType> result(poly);
    result *= val;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator*(const U& val, const polynomial<FieldType>& poly) {
    polynomial<FieldType> result(poly);
    result *= val;
    return result;
  }

  // division:
  template<typename FieldType>
  inline polynomial<FieldType> operator/(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    polynomial<FieldType> result(left);
    result /= right;
    return result;
  }

  template<typename FieldType, typename U>
  inline polynomial<FieldType> operator/(const polynomial<FieldType>& poly, const U& val) {
    polynomial<FieldType> result(poly);
    result /= val;
    return result;
  }

  // modulo:
  template<typename FieldType>
  inline polynomial<FieldType> operator%(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    polynomial<FieldType> result(left);
    result %= right;
    return result;
  }

  // comparisons:
  template<typename FieldType>
  bool operator==(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    if(left.size() != right.size())
      return false;
    typename std::vector<FieldType>::const_iterator left_it = left.begin();
    typename std::vector<FieldType>::const_iterator right_it = right.begin();
    while(left_it != left.end())
      if(*left_it++ != *right_it++)
        return false;
    return true;
  }

  template<typename FieldType>
  bool operator!=(const polynomial<FieldType>& left, const polynomial<FieldType>& right) {
    if(left.size() != right.size())
      return true;
    typename std::vector<FieldType>::const_iterator left_it = left.begin();
    typename std::vector<FieldType>::const_iterator right_it = right.begin();
    while(left_it != left.end())
      if(*left_it++ != *right_it++)
        return true;
    return false;
  }

  // special forms:
  template<typename FieldType, typename Size>
  polynomial<FieldType> chebyshev_form(const Size sz) {
    std::vector<FieldType> tmp(sz + 1);
    boost::math::tools::polynomial_chebyshev_form(tmp.begin(), tmp.end());
    return boost::math::tools::polynomial<FieldType>(tmp);
  }

  template<typename FieldType, typename Size>
  polynomial<FieldType> hermite_form(const Size sz) {
    std::vector<FieldType> tmp(sz + 1);
    boost::math::tools::polynomial_hermite_form(tmp.begin(), tmp.end());
    return boost::math::tools::polynomial<FieldType>(tmp);
  }

  template<typename FieldType, typename Size>
  polynomial<FieldType> laguerre_form(const Size sz) {
    std::vector<FieldType> tmp(sz + 1);
    boost::math::tools::polynomial_laguerre_form(tmp.begin(), tmp.end());
    return boost::math::tools::polynomial<FieldType>(tmp);
  }

  template<typename FieldType, typename Size>
  polynomial<FieldType> legendre_form(const Size sz) {
    std::vector<FieldType> tmp(sz + 1);
    boost::math::tools::polynomial_legendre_form(tmp.begin(), tmp.end());
    return boost::math::tools::polynomial<FieldType>(tmp);
  }

  // output stream:
  template<typename charT, typename traits, typename FieldType>
  inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const polynomial<FieldType>& poly) {
    os << "{";
    typename std::vector<FieldType>::const_iterator it = poly.begin();
    while(it != poly.end()) {
      if(it != poly.begin())
        os << ", ";
      os << *it++;
    }
    os << "}";
    return os;
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_POLYNOMIAL_HPP

