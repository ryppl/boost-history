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

    namespace detail {

struct fpu_rounding_modes
{
  unsigned short tonearest;
  unsigned short downward;
  unsigned short upward;
  unsigned short towardzero;
};

// exceptions masked, extended precision
// hardware default is 0x037f (0x1000 only has a meaning on 287)
static const fpu_rounding_modes rnd_mode = { 0x137f, 0x177f, 0x1b7f, 0x1f7f };

struct x86_rounding_control: x86_rounding
{
  static void tonearest()  { set_rounding_mode(rnd_mode.tonearest);  }
  static void downward()   { set_rounding_mode(rnd_mode.downward);   }
  static void upward()     { set_rounding_mode(rnd_mode.upward);     }
  static void towardzero() { set_rounding_mode(rnd_mode.towardzero); }
};

    } // namespace detail

template<>
struct rounding_control<float>: detail::x86_rounding_control
{
  static float force_rounding(const float& r) 
  { volatile float r_ = r; return r_; }
};

template<>
struct rounding_control<double>: detail::x86_rounding_control
{
  static double force_rounding(const double& r) 
  { volatile double r_ = r; return r_; }
};

template<>
struct rounding_control<long double>: detail::x86_rounding_control
{
  static const long double& force_rounding(const long double& r) { return r; }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_X86_ROUNDING_CONTROL_HPP */
