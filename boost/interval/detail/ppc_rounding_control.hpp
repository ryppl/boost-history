#ifndef BOOST_INTERVAL_DETAIL_PPC_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_DETAIL_PPC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __GNUC__
#error This header only works with GNU CC.
#endif

#if !defined(powerpc) && !defined(__powerpc__) && !defined(__ppc__)
#error This header only works on PPC CPUs.
#endif

namespace boost {
  namespace interval_lib {
    namespace detail {

typedef union {
  long long int imode;
  double dmode;
} rounding_mode_struct;

static const rounding_mode_struct mode_upward      = { 0xFFF8000000000002LL };
static const rounding_mode_struct mode_downward    = { 0xFFF8000000000003LL };
static const rounding_mode_struct mode_to_nearest  = { 0xFFF8000000000001LL };
static const rounding_mode_struct mode_toward_zero = { 0xFFF8000000000000LL };

struct ppc_rounding_control
{
  typedef double rounding_mode;

  static void set_rounding_mode(const rounding_mode mode)
  { __asm__ __volatile__ ("mtfsf 255,%0" : : "f"(mode)); }

  static void get_rounding_mode(rounding_mode& mode)
  { __asm__ __volatile__ ("mffs %0" : "=f"(mode)); }

  static void downward()    { set_rounding_mode(mode_downward.dmode);    }
  static void upward()      { set_rounding_mode(mode_upward.dmode);      }
  static void to_nearest()  { set_rounding_mode(mode_to_nearest.dmode);  }
  static void toward_zero() { set_rounding_mode(mode_toward_zero.dmode); }
};

    } // namespace detail

extern "C" {
  float rintf(float);
  double rint(double);
}

template<>
struct rounding_control<float>:
  detail::ppc_rounding_control,
  detail::ieee_float_constants
{
  static float force_rounding(const float r)
  {
    float tmp;
    __asm__ __volatile__ ("frsp %0, %1" : "=f" (tmp) : "f" (r));
    return tmp;
  }
  static float to_int(const float& x) { return rintf(x); }
};

template<>
struct rounding_control<double>:
  detail::ppc_rounding_control,
  detail::long_double_constants
{
  static const double & force_rounding(const double& r) { return r; }
  static double to_int(const double& r) { return rint(r); }
};

template<>
struct rounding_control<long double>:
  detail::ppc_rounding_control,
  detail::long_double_constants
{
  static const long double & force_rounding(const long double& r) { return r; }
  static long double to_int(const long double& r) { return rint(r); }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_INTERVAL_DETAIL_PPC_ROUNDING_CONTROL_HPP */
