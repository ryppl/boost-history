#ifndef BOOST_INTERVAL_ARITH2_HPP
#define BOOST_INTERVAL_ARITH2_HPP

#include <boost/interval/detail/interval_prototype.hpp>
#include <boost/interval/detail/test_input.hpp>
#include <boost/interval/detail/bugs.hpp>
#include <boost/interval/detail/division.hpp>
#include <boost/interval/arith.hpp>
#include <boost/interval/policies.hpp>
#include <algorithm>
#include <cmath>
#include <utility>

namespace boost {

template<class T, class Policies> inline
interval<T, Policies> fmod(const interval<T, Policies>& x,
			   const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Policies> >::type I;
  const T& yb = detail::is_neg(x.lower()) ? y.lower() : y.upper();
  T n = rnd.int_down(rnd.div_down(x.lower(), yb));
  return (const I&)x - n * (const I&)y;
}

template<class T, class Policies> inline
interval<T, Policies> fmod(const interval<T, Policies>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Policies> >::type I;
  T n = rnd.int_down(rnd.div_down(x.lower(), y));
  return (const I&)x - n * I(y);
}

template<class T, class Policies> inline
interval<T, Policies> fmod(const T& x, const interval<T, Policies>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Policies> >::type I;
  const T& yb = detail::is_neg(x) ? y.lower() : y.upper();
  T n = rnd.int_down(rnd.div_down(x, yb));
  return x - n * (const I&)y;
}

  namespace interval_lib {
    namespace detail {

template<class T, class Rounding> inline
T pow_aux(const T& x_, int pwr, Rounding& rnd) // x and pwr are positive
{
  T x = x_;
  T y = (pwr & 1) ? x_ : 1;
  pwr >>= 1;
  while (pwr > 0) {
    x = rnd.mul_up(x, x);
    if (pwr & 1) y = rnd.mul_up(x, y);
    pwr >>= 1;
  }
  return y;
}

    } // namespace detail
  } // namespace interval_lib

template<class T, class Policies> inline
interval<T, Policies> pow(const interval<T, Policies>& x, int pwr)
{
  using interval_lib::detail::pow_aux;

  if (interval_lib::detail::test_input(x))
    return interval<T, Policies>::empty();

  if (pwr == 0) {
    return interval<T, Policies>(T(1));
  } else if (pwr < 0) {
    return T(1) / pow(x, -pwr);
  }

  typename Policies::rounding rnd;
  
  if (detail::is_neg(x.upper())) {        // [-2,-1]
    T yl = pow_aux(-x.upper(), pwr, rnd);
    T yu = pow_aux(-x.lower(), pwr, rnd);
    if (pwr & 1)     // [-2,-1]^1
      return interval<T, Policies>(-yu, -yl, true);
    else             // [-2,-1]^2
      return interval<T, Policies>(yl, yu, true);
  } else if (detail::is_neg(x.lower())) { // [-1,1]
    if (pwr & 1) {   // [-1,1]^1
      return interval<T, Policies>(-pow_aux(-x.lower(), pwr, rnd),
				   pow_aux(x.upper(), pwr, rnd), true);
    } else {         // [-1,1]^2
      BOOST_INTERVAL_using_max(max);
      T y = pow_aux(max(-x.lower(), x.upper()), pwr, rnd);
      return interval<T, Policies>(0, y, true);
    }
  } else {                                // [1,2]
    return interval<T, Policies>(pow_aux(x.lower(), pwr, rnd),
				 pow_aux(x.upper(), pwr, rnd), true);
  }
}

template<class T, class Policies> inline
interval<T, Policies> sqrt(const interval<T, Policies>& x)
{
  if (interval_lib::detail::test_input(x) || detail::is_neg(x.upper()))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  T l = (x.lower() <= T(0)) ? 0 : rnd.sqrt_down(x.lower());
  return interval<T, Policies>(l, rnd.sqrt_up(x.upper()), true);
}

template<class T, class Policies> inline
interval<T, Policies> square(const interval<T, Policies>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Policies>::empty();
  typename Policies::rounding rnd;
  if (detail::is_neg(x.upper())) {
    return interval<T, Policies>(rnd.mul_down(x.upper(), x.upper()),
				 rnd.mul_up(x.lower(), x.lower()), true);
  } else if (!detail::is_neg(x.lower())) {
    return interval<T, Policies>(rnd.mul_down(x.lower(), x.lower()),
				 rnd.mul_up(x.upper(), x.upper()), true);
  } else {
    return interval<T, Policies>(0, (-x.lower() > x.upper() ? 
				     rnd.mul_up(x.lower(), x.lower()) :
				     rnd.mul_up(x.upper(), x.upper())), true);
  }
}

  namespace interval_lib {

template<class T, class Policies> inline
interval<T, Policies> division_part1(const interval<T, Policies>& x,
				     const interval<T, Policies>& y, bool& b)
{
  typedef interval<T, Policies> I;
  b = false;
  if (detail::test_input(x, y))
    return I::empty();
  if (in_zero(y))
    if (y.lower() != T(0))
      if (y.upper() != T(0))
	return detail::div_zero_part1(x, y, b);
      else
	return detail::div_negative(x, y.lower());
    else
      if (y.upper() != T(0))
	return detail::div_positive(x, y.upper());
      else
	return I::empty();
  else
    return detail::div_non_zero(x, y);
}

template<class T, class Policies> inline
interval<T, Policies> division_part2(const interval<T, Policies>& x,
				     const interval<T, Policies>& y)
{
  return detail::div_zero_part2(x, y);
}

  } // namespace interval_lib

} // namespace boost

#endif // BOOST_INTERVAL_ARITH2_HPP
