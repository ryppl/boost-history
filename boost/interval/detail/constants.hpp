#ifndef BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
#define BOOST_INTERVAL_DETAIL_CONSTANTS_HPP

#include <boost/cstdint.hpp>

namespace boost {
  namespace interval_lib {
    namespace detail {

typedef union {
  int b;
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
  static float pi_up()       { return cf_pi_u.f; }
  static float pi_1_2_down() { return cf_pi_1_2_d.f; }
  static float pi_1_2_up()   { return cf_pi_1_2_u.f; }
  static float pi_2_1_down() { return cf_pi_2_1_d.f; }
  static float pi_2_1_up()   { return cf_pi_2_1_u.f; }
};

typedef union {
  long long int b;
  double d;
} binary_double;

static const binary_double cd_pi_1_2_d = { 0x3FF921FB54442D18LL };
static const binary_double cd_pi_1_2_u = { 0x3FF921FB54442D19LL };
static const binary_double cd_pi_d     = { 0x400921FB54442D18LL };
static const binary_double cd_pi_u     = { 0x400921FB54442D19LL };
static const binary_double cd_pi_2_1_d = { 0x401921FB54442D18LL };
static const binary_double cd_pi_2_1_u = { 0x401921FB54442D19LL };

struct ieee_double_constants
{
  static double pi_down()     { return cd_pi_d.d; }
  static double pi_up()       { return cd_pi_u.d; }
  static double pi_1_2_down() { return cd_pi_1_2_d.d; }
  static double pi_1_2_up()   { return cd_pi_1_2_u.d; }
  static double pi_2_1_down() { return cd_pi_2_1_d.d; }
  static double pi_2_1_up()   { return cd_pi_2_1_u.d; }
};

    } // namespace detail
  } // namespace interval_lib
} // namespace boost


#endif // BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
