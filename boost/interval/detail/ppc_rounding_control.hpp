#ifndef BOOST_DETAIL_PPC_ROUNDING_CONTROL_HPP
#define BOOST_DETAIL_PPC_ROUNDING_CONTROL_HPP

#ifndef BOOST_INTERVAL_HPP
#error Internal header file: This header must be included by <boost/interval.hpp> only.
#endif

#ifndef __GNUC__
#error This header only works with GNU CC.
#endif

#ifndef POWERPC
#error This header only works on PPC CPUs.
#endif

namespace boost {
  namespace interval_lib {
    namespace detail {

typedef union {
  long long int imode;
  double dmode;
} rounding_mode_struct;

static const rounding_mode_struct mode_upward     = { 0xFFF8000000000002LL };
static const rounding_mode_struct mode_downward   = { 0xFFF8000000000003LL };
static const rounding_mode_struct mode_tonearest  = { 0xFFF8000000000001LL };
static const rounding_mode_struct mode_towardzero = { 0xFFF8000000000000LL };

struct ppc_rounding_control
{
  void set_rounding_mode(double mode) const
  {
    __asm__ __volatile__ ("mtfsf 255,%0" : : "f" (mode));
  }
  double get_rounding_mode() const
  {
   double tmp;
    __asm__ __volatile__ ("mffs %0" : "=f" (tmp));
    return tmp;
  }
  void downward() { set_rounding_mode(mode_downward.dmode); }
  void upward() { set_rounding_mode(mode_upward.dmode); }
  void tonearest() { set_rounding_mode(mode_tonearest.dmode); }
  void towardzero() { set_rounding_mode(mode_towardzero.dmode); }

  typedef double rounding_mode;
};

template<>
struct rounding_control<float>: ppc_rounding_control
{
  static float force_rounding(const float r)
  {
    float tmp;
    __asm__ __volatile__ ("frsp %0, %1" : "=f" (tmp) : "f" (r));
    return tmp;
  }
};

    } // namespace detail

template<>
struct rounding_control<double>: ppc_rounding_control
{
  static double force_rounding(const double& r) { return r; }
};

  } // namespace interval_lib
} // namespace boost

#endif /* BOOST_DETAIL_PPC_ROUNDING_CONTROL_HPP */
