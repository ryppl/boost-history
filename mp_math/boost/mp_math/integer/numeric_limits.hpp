// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_NUMERIC_LIMITS_HPP
#define BOOST_MP_MATH_INTEGER_NUMERIC_LIMITS_HPP

#include <limits>

namespace std
{

template<class Alloc, class Traits>
class numeric_limits<
  boost::mp_math::unbounded_uint<Alloc,Traits>
>
{
  typedef boost::mp_math::unbounded_uint<Alloc,Traits> int_type;

public:

  static const bool is_specialized = true;

  static int_type min() throw() { return int_type(0U); }
  static int_type max() throw() { return int_type(0U); }

  static const int  digits   = 0;
  static const int  digits10 = 0;
  static const bool is_signed  = false;
  static const bool is_integer = true;
  static const bool is_exact   = true;
  static const int  radix      = 0;

  // This is a Boost extension since the radix member above cannot hold the true
  // radix.
  static const typename int_type::digit_type max_radix_value =
    int_type::max_digit_value;

  static int_type epsilon    () throw() { return int_type(0U); }
  static int_type round_error() throw() { return int_type(0U); }

  static const int  min_exponent   = 0;
  static const int  min_exponent10 = 0;
  static const int  max_exponent   = 0;
  static const int  max_exponent10 = 0;

  static const bool has_infinity      = false;
  static const bool has_quiet_NaN     = false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss   = false;

  static int_type infinity     () throw() { return int_type(0U); }
  static int_type quiet_NaN    () throw() { return int_type(0U); }
  static int_type signaling_NaN() throw() { return int_type(0U); }
  static int_type denorm_min   () throw() { return int_type(0U); }

  static const bool is_iec559  = false;
  static const bool is_bounded = false;
  static const bool is_modulo  = false;

  static const bool traps           = false;
  static const bool tinyness_before = false;
  static const float_round_style round_style = round_toward_zero;
};


template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_specialized;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::digits;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::digits10;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_signed;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_integer;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_exact;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::radix;
template<class A, class T>
const typename boost::mp_math::unbounded_uint<A,T>::digit_type
numeric_limits<boost::mp_math::unbounded_uint<A,T> >::max_radix_value;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::min_exponent;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::min_exponent10;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::max_exponent;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_uint<A,T> >::max_exponent10;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::has_infinity;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::has_quiet_NaN;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::has_signaling_NaN;
template<class A, class T>
const float_denorm_style numeric_limits<boost::mp_math::unbounded_uint<A,T> >::has_denorm;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_iec559;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_bounded;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::is_modulo;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::traps;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_uint<A,T> >::tinyness_before;
template<class A, class T>
const float_round_style numeric_limits<boost::mp_math::unbounded_uint<A,T> >::round_style;


template<class Alloc, class Traits>
class numeric_limits<
  boost::mp_math::unbounded_int<Alloc,Traits>
>
{
  typedef boost::mp_math::unbounded_int<Alloc,Traits> int_type;

public:

  static const bool is_specialized = true;

  static int_type min() throw() { return int_type(0U); }
  static int_type max() throw() { return int_type(0U); }

  static const int  digits   = 0;
  static const int  digits10 = 0;
  static const bool is_signed  = true;
  static const bool is_integer = true;
  static const bool is_exact   = true;
  static const int  radix      = 0;

  // This is a Boost extension since the radix member above cannot hold the true
  // radix.
  static const typename int_type::digit_type max_radix_value =
    int_type::max_digit_value;

  static int_type epsilon    () throw() { return int_type(0U); }
  static int_type round_error() throw() { return int_type(0U); }

  static const int  min_exponent   = 0;
  static const int  min_exponent10 = 0;
  static const int  max_exponent   = 0;
  static const int  max_exponent10 = 0;

  static const bool has_infinity      = false;
  static const bool has_quiet_NaN     = false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss   = false;

  static int_type infinity     () throw() { return int_type(0U); }
  static int_type quiet_NaN    () throw() { return int_type(0U); }
  static int_type signaling_NaN() throw() { return int_type(0U); }
  static int_type denorm_min   () throw() { return int_type(0U); }

  static const bool is_iec559  = false;
  static const bool is_bounded = false;
  static const bool is_modulo  = false;

  static const bool traps           = false;
  static const bool tinyness_before = false;
  static const float_round_style round_style = round_toward_zero;
};


template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_specialized;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::digits;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::digits10;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_signed;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_integer;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_exact;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::radix;
template<class A, class T>
const typename boost::mp_math::unbounded_int<A,T>::digit_type
numeric_limits<boost::mp_math::unbounded_int<A,T> >::max_radix_value;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::min_exponent;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::min_exponent10;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::max_exponent;
template<class A, class T>
const int numeric_limits<boost::mp_math::unbounded_int<A,T> >::max_exponent10;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::has_infinity;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::has_quiet_NaN;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::has_signaling_NaN;
template<class A, class T>
const float_denorm_style numeric_limits<boost::mp_math::unbounded_int<A,T> >::has_denorm;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_iec559;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_bounded;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::is_modulo;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::traps;
template<class A, class T>
const bool numeric_limits<boost::mp_math::unbounded_int<A,T> >::tinyness_before;
template<class A, class T>
const float_round_style numeric_limits<boost::mp_math::unbounded_int<A,T> >::round_style;


template<bool Signed, class Alloc, class Traits>
class numeric_limits<
  boost::mp_math::unbounded<Signed,Alloc,Traits>
>
{
  typedef boost::mp_math::unbounded<Signed,Alloc,Traits> int_type;

public:

  static const bool is_specialized = true;

  static int_type min() throw() { return int_type(0U); }
  static int_type max() throw() { return int_type(0U); }

  static const int  digits   = 0;
  static const int  digits10 = 0;
  static const bool is_signed  = Signed;
  static const bool is_integer = true;
  static const bool is_exact   = true;
  static const int  radix      = 0;

  // This is a Boost extension since the radix member above cannot hold the true
  // radix.
  static const typename int_type::digit_type max_radix_value =
    int_type::max_digit_value;

  static int_type epsilon    () throw() { return int_type(0U); }
  static int_type round_error() throw() { return int_type(0U); }

  static const int  min_exponent   = 0;
  static const int  min_exponent10 = 0;
  static const int  max_exponent   = 0;
  static const int  max_exponent10 = 0;

  static const bool has_infinity      = false;
  static const bool has_quiet_NaN     = false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss   = false;

  static int_type infinity     () throw() { return int_type(0U); }
  static int_type quiet_NaN    () throw() { return int_type(0U); }
  static int_type signaling_NaN() throw() { return int_type(0U); }
  static int_type denorm_min   () throw() { return int_type(0U); }

  static const bool is_iec559  = false;
  static const bool is_bounded = false;
  static const bool is_modulo  = false;

  static const bool traps           = false;
  static const bool tinyness_before = false;
  static const float_round_style round_style = round_toward_zero;
};


template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_specialized;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::digits;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::digits10;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_signed;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_integer;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_exact;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::radix;
template<bool S, class A, class T>
const typename boost::mp_math::unbounded<S,A,T>::digit_type
numeric_limits<boost::mp_math::unbounded<S,A,T> >::max_radix_value;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::min_exponent;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::min_exponent10;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::max_exponent;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::unbounded<S,A,T> >::max_exponent10;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::has_infinity;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::has_quiet_NaN;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::has_signaling_NaN;
template<bool S, class A, class T>
const float_denorm_style numeric_limits<boost::mp_math::unbounded<S,A,T> >::has_denorm;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_iec559;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_bounded;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::is_modulo;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::traps;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::unbounded<S,A,T> >::tinyness_before;
template<bool S, class A, class T>
const float_round_style numeric_limits<boost::mp_math::unbounded<S,A,T> >::round_style;


template<bool Signed, class Alloc, class Traits>
class numeric_limits<
  boost::mp_math::integer<
    boost::mp_math::unbounded<Signed,Alloc,Traits>
  >
>
{
  typedef boost::mp_math::integer<
    boost::mp_math::unbounded<Signed,Alloc,Traits>
  > int_type;

public:

  static const bool is_specialized = true;

  static int_type min() throw() { return int_type(0U); }
  static int_type max() throw() { return int_type(0U); }

  static const int  digits   = 0;
  static const int  digits10 = 0;
  static const bool is_signed  = Signed;
  static const bool is_integer = true;
  static const bool is_exact   = true;
  static const int  radix      = 0;

  // This is a Boost extension since the radix member above cannot hold the true
  // radix.
  static const typename int_type::digit_type max_radix_value =
    int_type::max_digit_value;

  static int_type epsilon    () throw() { return int_type(0U); }
  static int_type round_error() throw() { return int_type(0U); }

  static const int  min_exponent   = 0;
  static const int  min_exponent10 = 0;
  static const int  max_exponent   = 0;
  static const int  max_exponent10 = 0;

  static const bool has_infinity      = false;
  static const bool has_quiet_NaN     = false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss   = false;

  static int_type infinity     () throw() { return int_type(0U); }
  static int_type quiet_NaN    () throw() { return int_type(0U); }
  static int_type signaling_NaN() throw() { return int_type(0U); }
  static int_type denorm_min   () throw() { return int_type(0U); }

  static const bool is_iec559  = false;
  static const bool is_bounded = false;
  static const bool is_modulo  = false;

  static const bool traps           = false;
  static const bool tinyness_before = false;
  static const float_round_style round_style = round_toward_zero;
};


template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_specialized;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::digits;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::digits10;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_signed;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_integer;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_exact;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::radix;
template<bool S, class A, class T>
const typename boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> >::digit_type
numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::max_radix_value;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::min_exponent;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::min_exponent10;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::max_exponent;
template<bool S, class A, class T>
const int numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::max_exponent10;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::has_infinity;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::has_quiet_NaN;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::has_signaling_NaN;
template<bool S, class A, class T>
const float_denorm_style numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::has_denorm;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_iec559;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_bounded;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::is_modulo;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::traps;
template<bool S, class A, class T>
const bool numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::tinyness_before;
template<bool S, class A, class T>
const float_round_style numeric_limits<boost::mp_math::integer<boost::mp_math::unbounded<S,A,T> > >::round_style;

} // namespace std


#endif

