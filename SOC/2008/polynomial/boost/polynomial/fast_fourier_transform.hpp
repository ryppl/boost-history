//  (C) Copyright Pawel Kieliszczyk 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_FAST_FOURIER_TRANSFORM_HPP
#define BOOST_MATH_TOOLS_FAST_FOURIER_TRANSFORM_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <cmath>
#include <boost/math/constants/constants.hpp>

namespace boost {
namespace math {
namespace tools {

  namespace detail {

    template<class InputIterator, class RandomAccessIterator>
    void bit_reverse_copy(InputIterator first, InputIterator last, RandomAccessIterator out) {
        typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
        difference_type sz = std::distance(first, last);
        for(difference_type i = 0; i < sz; ++i) {
            difference_type shift = sz / 2;
            difference_type pos = 0;
            difference_type k = i;
            while(k != 0) {
                if(k & 1)
                    pos += shift;
                k /= 2;
                shift /= 2;
            }
            out[pos] = *first++;
        }
    }

    template<class Complex>
    struct Complex_traits {
      typedef typename Complex::value_type value_type;
    };

    template<class Complex>
    inline typename Complex_traits<Complex>::value_type real_part(Complex const& c) {
      return c.real();
    }

    template<class Complex>
    inline typename Complex_traits<Complex>::value_type imag_part(Complex const& c) {
      return c.imag();
    }

    template<class Complex, class Real, class Imag>
    inline Complex create_complex(Real const& r, Imag const& i) {
      return Complex(r, i);
    }
 
    template<class Complex>
    inline Complex complex_add(Complex const& a, Complex const& b) {
      return create_complex<Complex>(real_part(a) + real_part(b), imag_part(a) + imag_part(b));
    }

    template<class Complex>
    inline Complex complex_sub(Complex const& a, Complex const& b) {
      return create_complex<Complex>(real_part(a) - real_part(b), imag_part(a) - imag_part(b));
    }

    template<class Complex>
    inline Complex complex_mul(Complex const& a, Complex const& b) {
      return create_complex<Complex>(real_part(a) * real_part(b) - imag_part(a) * imag_part(b),
                                     imag_part(a) * real_part(b) + real_part(a) * imag_part(b));
    }

    template<class Complex, class ScalarType>
    inline Complex complex_div_by_scalar(Complex const& comp, ScalarType const& scal) {
      return create_complex<Complex>(real_part(comp) / scal, imag_part(comp) / scal);
    }

  } // namespace detail

  template<class InputIterator, class OutputIterator>
  void fast_fourier_transform(InputIterator first, InputIterator last, OutputIterator out) {
    using namespace boost::math;
    using namespace boost::math::tools;
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<InputIterator>::value_type complex_type;
    typedef typename detail::Complex_traits<complex_type>::value_type value_type;
    typedef std::vector<complex_type> complex_type_sequence;
    typedef typename complex_type_sequence::size_type size_type;
    difference_type sz = std::distance(first, last);
    complex_type_sequence dft(sz);
    detail::bit_reverse_copy(first, last, dft.begin());
    difference_type l = static_cast<difference_type>(log2(sz));
    difference_type m = 1;
    for(difference_type s = 1; s <= l; ++s) {
      difference_type half_m = m;
      m *= 2;
      complex_type omega_m(std::pow(constants::e<value_type>(),
                           2.0 * constants::pi<value_type>() / m * detail::create_complex<complex_type>(0.0, 1.0)));
      for(difference_type k = 0; k < sz; k += m) {
        complex_type omega = detail::create_complex<complex_type>(1.0, 0.0);
        for(difference_type j = 0; j < half_m; ++j) {
          complex_type t = detail::complex_mul(omega, dft[k + j + half_m]);
          complex_type u = dft[k + j];
          dft[k + j] = detail::complex_add(u, t);
          dft[k + j + half_m] = detail::complex_sub(u, t);
          omega = detail::complex_mul(omega, omega_m);
        }
      }
    }
    for(size_type i = 0; i < dft.size(); ++i)
      *out++ = dft[i];
  }

  template<class InputIterator, class OutputIterator>
  void inverse_fast_fourier_transform(InputIterator first, InputIterator last, OutputIterator out) {
    using namespace boost::math;
    using namespace boost::math::tools;
    typedef typename std::iterator_traits<InputIterator>::difference_type difference_type;
    typedef typename std::iterator_traits<InputIterator>::value_type complex_type;
    typedef typename detail::Complex_traits<complex_type>::value_type value_type;
    typedef std::vector<complex_type> complex_type_sequence;
    typedef typename complex_type_sequence::size_type size_type;
    difference_type sz = std::distance(first, last);
    complex_type_sequence dft(sz);
    detail::bit_reverse_copy(first, last, dft.begin());
    difference_type l = static_cast<difference_type>(log2(sz));
    difference_type m = 1;
    for(difference_type s = 1; s <= l; ++s) {
      difference_type half_m = m;
      m <<= 1;
      complex_type omega_m(std::pow(constants::e<value_type>(),
                           -2.0 * constants::pi<value_type>() / m * detail::create_complex<complex_type>(0.0, 1.0)));
      for(difference_type k = 0; k < sz; k += m) {
        complex_type omega = detail::create_complex<complex_type>(1.0, 0.0);
        for(difference_type j = 0; j < half_m; ++j) {
          complex_type t = detail::complex_mul(omega, dft[k + j + half_m]);
          complex_type u = dft[k + j];
          dft[k + j] = detail::complex_add(u, t);
          dft[k + j + half_m] = detail::complex_sub(u, t);
          omega = detail::complex_mul(omega, omega_m);
        }
      }
    }
    value_type n = dft.size();
    for(typename complex_type_sequence::const_iterator it = dft.begin(); it != dft.end(); ++it)
      *out++ = detail::complex_div_by_scalar(*it, n);
  }

} // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_FAST_FOURIER_TRANSFORM_HPP

