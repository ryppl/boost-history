#ifndef BOOST_INTERVAL_ROUNDED_ARITH_HPP
#define BOOST_INTERVAL_ROUNDED_ARITH_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

namespace boost {

  namespace detail {

template<class T> bool sign(const T& x) { return x < 0; }
template<class T> bool is_nan(const T& x) { return x != x; }
template<class T> T smallest()
{
  if (std::numeric_limits<T>::is_integer) return 1;
  else return std::numeric_limits<T>::min();
}

  } // namespace detail

  namespace interval_lib {

template<class T>
struct rounding_control
{
  typedef void rounding_mode;
  void upward() { }
  void downward() { }
  void tonearest() { }
  rounding_mode get_rounding_mode() { }
  void set_rounding_mode(rounding_mode) { }
  static T pi_down() { return 3; }
  static T pi_up()   { return 4; }
  static T pi_2_1_down() { return 6; }
  static T pi_2_1_up()   { return 7; }
  static T pi_1_2_down() { return 1; }
  static T pi_1_2_up()   { return 2; }
  T to_int(const T& x) { return x; }
  T force_rounding(const T& x) { return x; }
};

template<class T, class Rounding = rounding_control<T> >
struct rounded_arithmetic_exact: Rounding {
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

template<class T, class Rounding = rounding_control<T> >
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

template<class T, class Rounding = rounding_control<T> >
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
  { return sub_down(std::sqrt(x), detail::smallest<T>()); }
  T sqrt_up(const T& x) { return force_rounding(std::sqrt(x)); }
  T int_down(const T& x) { return -to_int(-x); }
  T int_up(const T& x) { return to_int(x); }
};

    namespace detail {

template<class Rounding>
struct save_state_tonearest: Rounding
{
  typename Rounding::rounding_mode mode;
  save_state_tonearest() {
    mode = get_rounding_mode();
    tonearest();
  }
  ~save_state_tonearest() { set_rounding_mode(mode); }
};

template<class Rounding>
struct save_state_unprotected: Rounding
{
  typedef save_state_unprotected<Rounding> unprotected_rounding;
  typedef save_state_tonearest<Rounding> tonearest_rounding;
};

template<class Rounding>
struct tonearest
{
  typedef typename Rounding::tonearest_rounding type;
};

    } // namespace detail

template<class Rounding>
struct save_state: Rounding
{
  typename Rounding::rounding_mode mode;
  save_state() {
    mode = get_rounding_mode();
    init();
  }
  ~save_state() { set_rounding_mode(mode); }
  typedef detail::save_state_unprotected<Rounding> unprotected_rounding;
  typedef detail::save_state_tonearest<Rounding> tonearest_rounding;
};

template<class Rounding>
struct save_state_nothing: Rounding
{
  typedef save_state_nothing<Rounding> unprotected_rounding;
  typedef save_state_nothing<Rounding> tonearest_rounding;
};

template<class Rounding>
struct unprotect
{
  typedef typename Rounding::unprotected_rounding type;
};

template<class T, class Compare, class Rounding, class Checking>
struct unprotect<interval_traits<T, Compare, Rounding, Checking> >
{
  typedef
    interval_traits<T, Compare, typename unprotect<Rounding>::type, Checking>
    type;
};

template<class T, class Traits>
struct unprotect<interval<T, Traits> > 
{
  typedef interval<T, typename unprotect<Traits>::type> type;
};

  } // namespace interval
} // namespace boost

#include <boost/interval/detail/constants.hpp>

// define appropriate specialization of rounding_control for built-in types
#if defined(__i386__) || defined(__BORLANDC__) || defined(BOOST_MSVC)
#  include <boost/interval/detail/x86_rounding_control.hpp>
#elif defined(powerpc) || defined(__powerpc__)
#  include <boost/interval/detail/ppc_rounding_control.hpp>
#elif defined(sparc) || defined(__sparc__)
#  include <boost/interval/detail/sparc_rounding_control.hpp>
#elif defined(__USE_ISOC99)
#  include <boost/interval/detail/isoc99_rounding_control.hpp>
#else
#  error Please specify rounding control mechanism.
#endif

#include <boost/interval/rounded_transc.hpp>

namespace boost {
  namespace interval_lib {

template<class T>
struct rounded_arithmetic:
    save_state_nothing
      <rounded_transc_dummy
         <T, rounded_arithmetic_exact<T> > >
{ };

    namespace detail {

template<class T>
struct ra_aux:
    save_state
      <rounded_transc_dummy
         <T, rounded_arithmetic_opposite_trick<T> > >
{ };

    } // namespace detail

template<> struct rounded_arithmetic<float>: detail::ra_aux<float> {};
template<> struct rounded_arithmetic<double>: detail::ra_aux<double> {};

  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_ROUNDED_ARITH_HPP
