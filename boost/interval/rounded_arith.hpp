#ifndef BOOST_INTERVAL_ROUNDED_ARITH_HPP
#define BOOST_INTERVAL_ROUNDED_ARITH_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

namespace boost {

  namespace detail {
template<class T> inline bool sign(const T& x) { return x < 0; }
template<class T> inline bool is_nan(const T& x) { return x != x; }
  }

  namespace interval_lib {

template<class T, class Rounding>
struct rounded_arithmetic_standard: Rounding {
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
struct rounded_arithmetic_opposite_trick: Rounding {
  rounded_arithmetic_opposite_trick() { upward(); }
  T add_down(const T& x, const T& y) { return -force_rounding((-x) - y); }
  T add_up  (const T& x, const T& y) { return force_rounding(x + y); }
  T sub_down(const T& x, const T& y) { return -force_rounding(y - x); }
  T sub_up  (const T& x, const T& y) { return force_rounding(x - y); }
  T mul_down(const T& x, const T& y) { return -force_rounding(x * (-y)); }
  T mul_up  (const T& x, const T& y) { return force_rounding(x * y); }
  T div_down(const T& x, const T& y) { return -force_rounding(x / (-y)); }
  T div_up  (const T& x, const T& y) { return force_rounding(x / y); }
  T sqrt_down(const T& x)
  { return sub_down(std::sqrt(x), std::numeric_limits<T>::min()); }
  T sqrt_up(const T& x) { return force_rounding(std::sqrt(x)); }
  T int_down(const T& x) { return -to_int(-x); }
  T int_up(const T& x) { return to_int(x); }
};

template<class T, class Rounding>
struct rounded_transc_dummy: Rounding {
  T exp_down(const T& x) { return 0; }
  T exp_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T log_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T log_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T sin_down(const T& x) { return -1; }
  T sin_up  (const T& x) { return 1; }
  T cos_down(const T& x) { return -1; }
  T cos_up  (const T& x) { return 1; }
  T tan_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T tan_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T asin_down(const T& x) { return -pi_1_2_up(); }
  T asin_up  (const T& x) { return pi_1_2_up(); }
  T acos_down(const T& x) { return 0; }
  T acos_up  (const T& x) { return pi_up(); }
  T atan_down(const T& x) { return -pi_1_2_up(); }
  T atan_up  (const T& x) { return pi_1_2_up(); }
  T sinh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T sinh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T cosh_down(const T& x) { return 1; }
  T cosh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T tanh_down(const T& x) { return -1; }
  T tanh_up  (const T& x) { return 1; }
  T asinh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T asinh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T acosh_down(const T& x) { return 0; }
  T acosh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
  T atanh_down(const T& x) { return -std::numeric_limits<T>::infinity(); }
  T atanh_up  (const T& x) { return std::numeric_limits<T>::infinity(); }
};

template<class Rounding>
struct save_state: Rounding
{
  typename Rounding::rounding_mode mode;
  save_state() { mode = get_rounding_mode(); }
  ~save_state() { set_rounding_mode(mode); }
};

template<class T>
struct rounding_control
{
  void upward() {}
  void downward() {}
  static T force_rounding(const T& x) { return x; }
};

  } // namespace interval
} // namespace boost

// define appropriate specialization of rounding_control for built-in types
#if defined(__STDC__) && __STDC_VERSION__ >= 199901L || \
    defined(__linux__) && defined(__USE_ISOC99)
//#include <boost/detail/isoc99_rounding_control.hpp>
#include <boost/interval/detail/linux_rounding_control.hpp>
#elif defined(__linux__)
#include <boost/interval/detail/linux_rounding_control.hpp>
#elif defined(powerpc)
#include <boost/interval/detail/ppc_rounding_control.hpp>
#elif defined(sun)
#include <boost/interval/detail/solaris_rounding_control.hpp>
#elif defined(__BORLANDC__)
#include <boost/interval/detail/bcc_rounding_control.hpp>
#elif defined(BOOST_MSVC)
#include <boost/interval/detail/msvc_rounding_control.hpp>
#else
#error Please specify rounding control mechanism.
#endif

namespace boost {
  namespace interval_lib {

template<class T>
struct rounded_arithmetic:
    rounded_transc_dummy
      <T, rounded_arithmetic_standard<T, rounding_control<T> > >
{};

template<>
struct rounded_arithmetic<float>:
    rounded_transc_dummy
      <float, rounded_arithmetic_opposite_trick
        <float, save_state<rounding_control<float> > > >
{};

template<>
struct rounded_arithmetic<double>:
    rounded_transc_dummy
      <float, rounded_arithmetic_opposite_trick
	<double, save_state<rounding_control<double> > > >
{};

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_ROUNDED_ARITH_HPP
