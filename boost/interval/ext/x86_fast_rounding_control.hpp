#ifndef BOOST_INTERVAL_EXT_X86_FAST_ROUNDING_CONTROL_HPP
#define BOOST_INTERVAL_EXT_X86_FAST_ROUNDING_CONTROL_HPP

namespace boost {
  namespace interval_lib {

    namespace detail {

// exceptions masked, expected precision (the mask is 0x0300)
static const fpu_rounding_modes rnd_mode_f = { 0x107f, 0x147f, 0x187f, 0x1c7f };
static const fpu_rounding_modes rnd_mode_d = { 0x127f, 0x167f, 0x1a7f, 0x1e7f };
static const fpu_rounding_modes rnd_mode_l = { 0x137f, 0x177f, 0x1b7f, 0x1f7f };

    } // namespace detail

template<class T>
struct x86_fast_rounding_control;

template<>
struct x86_fast_rounding_control<float>: detail::x86_rounding
{
  static void tonearest()  { set_rounding_mode(detail::rnd_mode_f.tonearest);  }
  static void downward()   { set_rounding_mode(detail::rnd_mode_f.downward);   }
  static void upward()     { set_rounding_mode(detail::rnd_mode_f.upward);     }
  static void towardzero() { set_rounding_mode(detail::rnd_mode_f.towardzero); }
  static const float& force_rounding(const float& r) { return r; }
};

template<>
struct x86_fast_rounding_control<double>: detail::x86_rounding
{
  static void tonearest()  { set_rounding_mode(detail::rnd_mode_d.tonearest);  }
  static void downward()   { set_rounding_mode(detail::rnd_mode_d.downward);   }
  static void upward()     { set_rounding_mode(detail::rnd_mode_d.upward);     }
  static void towardzero() { set_rounding_mode(detail::rnd_mode_d.towardzero); }
  static const double& force_rounding(const double& r) { return r; }
};

template<>
struct x86_fast_rounding_control<long double>: detail::x86_rounding
{
  static void tonearest()  { set_rounding_mode(detail::rnd_mode_l.tonearest);  }
  static void downward()   { set_rounding_mode(detail::rnd_mode_l.downward);   }
  static void upward()     { set_rounding_mode(detail::rnd_mode_l.upward);     }
  static void towardzero() { set_rounding_mode(detail::rnd_mode_l.towardzero); }
  static const long double& force_rounding(const long double& r) { return r; }
};

  } // namespace interval_lib
} // namespace interval

#endif // BOOST_INTERVAL_EXT_X86_FAST_ROUNDING_CONTROL_HPP
