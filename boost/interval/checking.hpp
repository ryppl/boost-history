#ifndef BOOST_INTERVAL_CHECKING_HPP
#define BOOST_INTERVAL_CHECKING_HPP

#include <stdexcept>

namespace boost {
  namespace interval_lib {

template<class T>
struct checking_nothing
{
  static void inverted_bound(const T&, const T&) {}
  static void divide_by_zero(const T&, const T&) {}
  static void negative_sqrt(const T&, const T&) {}
  static void logarithmic_nan() {}
  static void logarithmic_inf() {}
  static void trigonometric_nan() {}
  static void trigonometric_inf() {}
  static void hyperbolic_nan() {}
  static void hyperbolic_inf() {}
};

template<class T>
struct checking_lax
{
  static void inverted_bound(const T&, const T&) {
    throw std::invalid_argument("boost::interval: inverted bounds");
  }
  static void divide_by_zero(const T&, const T&) {}
  static void negative_sqrt(const T&, const T&) {}
  static void logarithmic_nan() {}
  static void logarithmic_inf() {}
  static void trigonometric_nan() {}
  static void trigonometric_inf() {}
  static void hyperbolic_nan() {}
  static void hyperbolic_inf() {}
};

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_CHECKING_HPP
