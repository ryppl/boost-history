#ifndef BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
#define BOOST_INTERVAL_DETAIL_CONSTANTS_HPP

namespace boost {
  namespace interval_lib {
    namespace detail {

typedef union {
  unsigned int b;
  float f;
} binary_float;

static const binary_float cf_pi_1_2_d = { 0x3FC90FDA };
static const binary_float cf_pi_1_2_u = { 0x3FC90FDB };
static const binary_float cf_pi_d     = { 0x40490FDA };
static const binary_float cf_pi_u     = { 0x40490FDB };
static const binary_float cf_pi_2_1_d = { 0x40C90FDA };
static const binary_float cf_pi_2_1_u = { 0x40C90FDB };

struct ieee_float_constants
{
  static float pi_down()     { return cf_pi_d.f; }
  static float pi_up  ()     { return cf_pi_u.f; }
  static float pi_1_2_down() { return cf_pi_1_2_d.f; }
  static float pi_1_2_up  () { return cf_pi_1_2_u.f; }
  static float pi_2_1_down() { return cf_pi_2_1_d.f; }
  static float pi_2_1_up  () { return cf_pi_2_1_u.f; }
};

typedef union {
  unsigned
#ifdef BOOST_HAS_MS_INT64
  __int64 b;
# define _m_ibc_(a) a##ui64
#elif defined(BOOST_HAS_LONG_LONG)
  long long int b;
# define _m_ibc_(a) a##uLL 
#else
# error The compiler does not support long long nor __int64 types
#endif
  double d;
} binary_double;

static const binary_double cd_pi_1_2_d = { _m_ibc_(0x3FF921FB54442D18) };
static const binary_double cd_pi_1_2_u = { _m_ibc_(0x3FF921FB54442D19) };
static const binary_double cd_pi_d     = { _m_ibc_(0x400921FB54442D18) };
static const binary_double cd_pi_u     = { _m_ibc_(0x400921FB54442D19) };
static const binary_double cd_pi_2_1_d = { _m_ibc_(0x401921FB54442D18) };
static const binary_double cd_pi_2_1_u = { _m_ibc_(0x401921FB54442D19) };

#undef _m_ibc_

struct ieee_double_constants
{
  static double pi_down()     { return cd_pi_d.d; }
  static double pi_up  ()     { return cd_pi_u.d; }
  static double pi_1_2_down() { return cd_pi_1_2_d.d; }
  static double pi_1_2_up  () { return cd_pi_1_2_u.d; }
  static double pi_2_1_down() { return cd_pi_2_1_d.d; }
  static double pi_2_1_up  () { return cd_pi_2_1_u.d; }
};

    } // namespace detail
  } // namespace interval_lib
} // namespace boost


#endif // BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
