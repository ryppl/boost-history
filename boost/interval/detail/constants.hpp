#ifndef BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
#define BOOST_INTERVAL_DETAIL_CONSTANTS_HPP

namespace boost {
  namespace interval_lib {
    namespace detail {

// These constants should be exactly computed.
// Decimal representations wouldn't do it since the standard doesn't
// specify the rounding (even nearest) that should be used.

struct ieee_float_constants
{
  static float pi_down()     { return 13176794.0f/(1<<22); }
  static float pi_up  ()     { return 13176795.0f/(1<<22); }
  static float pi_1_2_down() { return 13176794.0f/(1<<23); }
  static float pi_1_2_up  () { return 13176795.0f/(1<<23); }
  static float pi_2_1_down() { return 13176794.0f/(1<<21); }
  static float pi_2_1_up  () { return 13176795.0f/(1<<21); }
};

struct ieee_double_constants
{
  static double pi_down()
  { return (3373259426.0 + 273688.0 / (1<<21)) / (1<<30); }
  static double pi_up  ()
  { return (3373259426.0 + 273689.0 / (1<<21)) / (1<<30); }
  static double pi_1_2_down()
  { return ((3373259426.0 + 273688.0 / (1<<21)) / (1<<30)) / 2; }
  static double pi_1_2_up  ()
  { return ((3373259426.0 + 273689.0 / (1<<21)) / (1<<30)) / 2; }
  static double pi_2_1_down()
  { return (3373259426.0 + 273688.0 / (1<<21)) / (1<<29); }
  static double pi_2_1_up  ()
  { return (3373259426.0 + 273689.0 / (1<<21)) / (1<<29); }
};

// For now, the long double version returns the same precision as double.
struct long_double_constants
{
  static long double pi_down()
  { return (3373259426.0 + 273688.0 / (1<<21)) / (1<<30); }
  static long double pi_up  ()
  { return (3373259426.0 + 273689.0 / (1<<21)) / (1<<30); }
  static long double pi_1_2_down()
  { return ((3373259426.0 + 273688.0 / (1<<21)) / (1<<30)) / 2; }
  static long double pi_1_2_up  ()
  { return ((3373259426.0 + 273689.0 / (1<<21)) / (1<<30)) / 2; }
  static long double pi_2_1_down()
  { return (3373259426.0 + 273688.0 / (1<<21)) / (1<<29); }
  static long double pi_2_1_up  ()
  { return (3373259426.0 + 273689.0 / (1<<21)) / (1<<29); }
};

    } // namespace detail
  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_DETAIL_CONSTANTS_HPP
