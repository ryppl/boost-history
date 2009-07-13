// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_UNBOUNDED_TRAITS_HPP
#define BOOST_MP_MATH_INTEGER_UNBOUNDED_TRAITS_HPP

#include <cstddef> // size_t
#include <limits>
#include <boost/config.hpp>
#include <boost/mp_math/integer/detail/base/primitive_ops.hpp>


namespace boost {
namespace mp_math {

template<
  typename DigitT =
  #ifndef BOOST_HAS_LONG_LONG
    unsigned long int
  #else
    unsigned long long int
  #endif
  ,
  typename SizeT = std::size_t,
  bool debug = false
>
struct unbounded_traits
{
  typedef DigitT digit_type;
  typedef SizeT  size_type;

  typedef detail::base::primitive_ops<digit_type, size_type> ops_type;

  static const bool enable_debug_mode = debug;

  static const size_type radix_bits = std::numeric_limits<digit_type>::digits;
  static const size_type digit_bits = std::numeric_limits<digit_type>::digits;
  static const digit_type max_digit_value = static_cast<digit_type>(-1);

  static size_type toom_mul_threshold;
  static size_type toom_sqr_threshold;
  static size_type karatsuba_mul_threshold;
  static size_type karatsuba_sqr_threshold;
};


template<typename D, typename S, bool b>
typename unbounded_traits<D,S,b>::size_type
unbounded_traits<D,S,b>::toom_mul_threshold = 350;

template<typename D, typename S, bool b>
typename unbounded_traits<D,S,b>::size_type
unbounded_traits<D,S,b>::toom_sqr_threshold = 400;

template<typename D, typename S, bool b>
typename unbounded_traits<D,S,b>::size_type
unbounded_traits<D,S,b>::karatsuba_mul_threshold = 80;

template<typename D, typename S, bool b>
typename unbounded_traits<D,S,b>::size_type
unbounded_traits<D,S,b>::karatsuba_sqr_threshold = 120;



} // namespace mp_math
} // namespace boost

#endif

