#ifndef BOOST_INTERVAL_ARITH2_HPP
#define BOOST_INTERVAL_ARITH2_HPP

#include <boost/interval/detail/interval_prototype.hpp>
#include <boost/interval/detail/test_input.hpp>
#include <boost/interval/detail/bugs.hpp>
#include <boost/interval/arith.hpp>
#include <boost/interval/rounding.hpp>
#include <algorithm>
#include <cmath>

namespace boost {

template<class T, class Traits> inline
interval<T, Traits> fmod(const interval<T, Traits>& x,
			 const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  const T& yb = detail::is_neg(x.lower()) ? y.lower() : y.upper();
  T n = rnd.int_down(rnd.div_down(x.lower(), yb));
  return (const I&)x - n * (const I&)y;
}

template<class T, class Traits> inline
interval<T, Traits> fmod(const interval<T, Traits>& x, const T& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
  T n = rnd.int_down(rnd.div_down(x.lower(), y));
  return (const I&)x - n * I(y);
}

template<class T, class Traits> inline
interval<T, Traits> fmod(const T& x, const interval<T, Traits>& y)
{
  if (interval_lib::detail::test_input(x, y))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  typedef typename interval_lib::unprotect<interval<T, Traits> >::type I;
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
  T y = 1;
  while (pwr > 0) {
    if (pwr & 1) y = rnd.mul_up(x, y);
    pwr >>= 1;
    if (pwr > 0) x = rnd.mul_up(x, x);
  }
  return y;
}

    } // namespace detail
  } // namespace interval_lib

template<class T, class Traits> inline
interval<T, Traits> pow(const interval<T, Traits>& x, int pwr)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();

  if (pwr == 0) {
    return interval<T, Traits>(T(1));
  } else if (pwr < 0) {
    return T(1) / pow(x, -pwr);
  }

  using interval_lib::detail::pow_aux;

  typename Traits::rounding rnd;
  bool sgnl = detail::is_neg(x.lower());
  bool sgnu = detail::is_neg(x.upper());
  bool odd_pwr = pwr & 1;
  
  if (sgnu) {        // [-2,-1]
    T yl = pow_aux(-x.upper(), pwr, rnd);
    T yu = pow_aux(-x.lower(), pwr, rnd);
    if (odd_pwr)     // [-2,-1]^1
      return interval<T, Traits>(-yu, -yl, true);
    else             // [-2,-1]^2
      return interval<T, Traits>(yl, yu, true);
  } else if (sgnl) { // [-1,1]
    if (odd_pwr) {   // [-1,1]^1
      return interval<T, Traits>(-pow_aux(-x.lower(), pwr, rnd),
				 pow_aux(x.upper(), pwr, rnd), true);
    } else {         // [-1,1]^2
      BOOST_INTERVAL_using_max(max);
      T y = pow_aux(max(-x.lower(), x.upper()), pwr, rnd);
      return interval<T, Traits>(0, y, true);
    }
  } else {           // [1,2]
    return interval<T, Traits>(pow_aux(x.lower(), pwr, rnd),
			       pow_aux(x.upper(), pwr, rnd), true);
  }
}

template<class T, class Traits> inline
interval<T, Traits> sqrt(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x) || detail::is_neg(x.upper()))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  T l = (x.lower() <= T(0)) ? 0 : rnd.sqrt_down(x.lower());
  return interval<T, Traits>(l, rnd.sqrt_up(x.upper()), true);
}

template<class T, class Traits> inline
interval<T, Traits> square(const interval<T, Traits>& x)
{
  if (interval_lib::detail::test_input(x))
    return interval<T, Traits>::empty();
  typename Traits::rounding rnd;
  if (detail::is_neg(x.upper())) {
    return interval<T, Traits>(rnd.mul_down(x.upper(), x.upper()),
			       rnd.mul_up(x.lower(), x.lower()), true);
  } else if (!detail::is_neg(x.lower())) {
    return interval<T, Traits>(rnd.mul_down(x.lower(), x.lower()),
			       rnd.mul_up(x.upper(), x.upper()), true);
  } else {
    return interval<T, Traits>(0, (-x.lower() > x.upper() ? 
				   rnd.mul_up(x.lower(), x.lower()) :
				   rnd.mul_up(x.upper(), x.upper())), true);
  }
}

} // namespace boost

#endif // BOOST_INTERVAL_ARITH2_HPP
