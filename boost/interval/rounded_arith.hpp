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
struct rounded_arithmetic_opposite_trick: Rounding {
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
  { return sub_down(std::sqrt(x), std::numeric_limits<T>::min()); }
  T sqrt_up(const T& x) { return force_rounding(std::sqrt(x)); }
  T int_down(const T& x) { return -to_int(-x); }
  T int_up(const T& x) { return to_int(x); }
};

template<class Rounding>
struct save_state: Rounding
{
  typename Rounding::rounding_mode mode;
  save_state() {
    mode = get_rounding_mode();
    init();
  }
  ~save_state() { set_rounding_mode(mode); }
};

template<class T>
struct unprotect {
  typedef T type;
};

template<class Rounding>
struct unprotect<save_state<Rounding> > {
  typedef Rounding type;
};

template<class T, class Compare, class Rounding, class Checking>
struct unprotect<interval_traits<T, Compare, Rounding, Checking> > {
  typedef
    interval_traits<T, Compare, typename unprotect<Rounding>::type, Checking>
    type;
};

template<class T, class Traits>
struct unprotect<interval<T, Traits> > {
  typedef interval<T, typename unprotect<Traits>::type> type;
};

    namespace detail {

template<class Rounding>
struct save_state_tonearest: Rounding {
  typename Rounding::rounding_mode mode;
  save_state_tonearest() {
    mode = get_rounding_mode();
    tonearest();
  }
  ~save_state_tonearest() { set_rounding_mode(mode); }
};

template<class Rounding>
struct tonearest {
  typedef save_state_tonearest<Rounding> type;
};

template<class Rounding>
struct tonearest<save_state<Rounding> > {
  typedef save_state_tonearest<Rounding> type;
};

    } // namespace detail

template<class T>
struct rounding_control
{
  void upward() {}
  void downward() {}
  static T force_rounding(const T& x) { return x; }
};

  } // namespace interval
} // namespace boost

#include <boost/interval/detail/constants.hpp>

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
{
  typedef
    rounded_transc_dummy
      <T, rounded_arithmetic_standard<T, rounding_control<T> > >
    unprotected;
};

    namespace detail {

template<class T>
struct ra_aux:
    save_state
      <rounded_transc_dummy
         <T, rounded_arithmetic_opposite_trick<T, rounding_control<T> > > >
{
  typedef
    rounded_transc_dummy
      <T, rounded_arithmetic_opposite_trick<T, rounding_control<T> > >
    unprotected;
};

    } // namespace detail

template<class T>
struct unprotect<rounded_arithmetic<T> > {
  typedef typename rounded_arithmetic<T>::unprotected type;
};

template<> struct rounded_arithmetic<float>: detail::ra_aux<float> {};
template<> struct rounded_arithmetic<double>: detail::ra_aux<double> {};

  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_ROUNDED_ARITH_HPP
