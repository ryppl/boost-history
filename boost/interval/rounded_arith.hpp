#ifndef BOOST_INTERVAL_ROUNDED_ARITH_HPP
#define BOOST_INTERVAL_ROUNDED_ARITH_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

namespace boost {

  namespace detail {

template<class T> bool sign(const T& x) { return x < 0; }
template<class T> bool is_nan(const T& x) { return x != x; }

  } // namespace detail

  namespace interval_lib {

    namespace detail {

template<class T> T smallest()
{
  if (std::numeric_limits<T>::is_integer) return 1;
  else return std::numeric_limits<T>::min();
}

    } // namespace detail

template<class T, class Rounding>
struct rounded_arith_exact: Rounding {
  void init() { }
  T add_down(const T& x, const T& y) { return x + y; }
  T add_up  (const T& x, const T& y) { return x + y; }
  T sub_down(const T& x, const T& y) { return x - y; }
  T sub_up  (const T& x, const T& y) { return x - y; }
  T mul_down(const T& x, const T& y) { return x * y; }
  T mul_up  (const T& x, const T& y) { return x * y; }
  T div_down(const T& x, const T& y) { return x / y; }
  T div_up  (const T& x, const T& y) { return x / y; }
  T sqrt_down(const T& x) { return std::sqrt(x); }
  T sqrt_up  (const T& x) { return std::sqrt(x); }
  T int_down(const T& x) { return std::floor(x); }
  T int_up  (const T& x) { return std::ceil(x); }
};

template<class T, class Rounding>
struct rounded_arith_std: Rounding {
  void init() { }
  T add_down(const T& x, const T& y)
  { return (downward(), force_rounding(x + y)); }
  T add_up(const T& x, const T& y)
  { return (upward(), force_rounding(x + y)); }
  T sub_down(const T& x, const T& y)
  { return (downward(), force_rounding(x - y)); }
  T sub_up(const T& x, const T& y)
  { return (upward(), force_rounding(x - y)); }
  T mul_down(const T& x, const T& y)
  { return (downward(), force_rounding(x * y)); }
  T mul_up(const T& x, const T& y)
  { return (upward(), force_rounding(x * y)); }
  T div_down(const T& x, const T& y)
  { return (downward(), force_rounding(x / y)); }
  T div_up(const T& x, const T& y)
  { return (upward(), force_rounding(x / y)); }
  T sqrt_down(const T& x)
  { return (downward(), force_rounding(std::sqrt(x))); }
  T sqrt_up(const T& x)
  { return (upward(), force_rounding(std::sqrt(x))); }
  T int_down(const T& x)
  { return (downward(), to_int(x)); }
  T int_up(const T& x)
  { return (upward(), to_int(x)); }
};

template<class T, class Rounding>
struct rounded_arith_opp: Rounding {
  void init() { upward(); }
  T add_down(const T& x, const T& y) { return -force_rounding((-x) - y); }
  T add_up  (const T& x, const T& y) { return force_rounding(x + y); }
  T sub_down(const T& x, const T& y) { return -force_rounding(y - x); }
  T sub_up  (const T& x, const T& y) { return force_rounding(x - y); }
  T mul_down(const T& x, const T& y) { return -force_rounding(x * (-y)); }
  T mul_up  (const T& x, const T& y) { return force_rounding(x * y); }
  T div_down(const T& x, const T& y) { return -force_rounding(x / (-y)); }
  T div_up  (const T& x, const T& y) { return force_rounding(x / y); }
  T sqrt_down(const T& x)
  { return sub_down(std::sqrt(x), detail::smallest<T>()); }
  T sqrt_up(const T& x) { return force_rounding(std::sqrt(x)); }
  T int_down(const T& x) { return -to_int(-x); }
  T int_up(const T& x) { return to_int(x); }
};

  } // namespace interval_lib
} // namespace boost


#endif // BOOST_INTERVAL_ROUNDED_ARITH_HPP
