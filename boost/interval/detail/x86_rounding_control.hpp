#ifndef BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#  error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifdef __GNUC__
#  include <boost/interval/detail/x86gcc_rounding_control.hpp>
#elif __BORLANDC__
#  include <boost/interval/detail/bcc_rounding_control.hpp>
#elif _MSC_VER
#  include <boost/interval/detail/msvc_rounding_control.hpp>
#else
#  error Unsupported C++ compiler.
#endif

namespace boost {
  namespace interval_lib {

template<>
struct rounding_control<float>:
  detail::x86_rounding_control,
  detail::ieee_float_constants
{
  static float force_rounding(const float& r) 
  { volatile float r_ = r; return r_; }
};

template<>
struct rounding_control<double>:
  detail::x86_rounding_control,
  detail::ieee_double_constants
{
  static double force_rounding(const double& r) 
  { volatile double r_ = r; return r_; }
};

template<>
struct rounding_control<long double>:
  detail::x86_rounding_control,
  detail::ieee_double_constants
{
  static long double force_rounding(const long double& r) { return r; }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP */
