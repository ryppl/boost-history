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
  static void sqrt_nan() {}
  static void logarithmic_nan() {}
  static void logarithmic_inf() {}
  static void trigonometric_nan() {}
  static void trigonometric_inf() {}
  static void hyperbolic_nan() {}
  static void hyperbolic_inf() {}
  /*
  static void created_empty() {}
  static void empty_lower() { return std::numeric_limits<T>::quiet_NaN(); }
  static void empty_upper() { return std::numeric_limits<T>::quiet_NaN(); }
  BOOST_STATIC_CONSTANT(bool, test_empty_input = false);
  */
};

template<class T>
struct checking_lax
{
  static void inverted_bound(const T&, const T&) {
    throw std::invalid_argument("boost::interval: inverted bounds");
  }
  static void divide_by_zero(const T&, const T&) {}
  static void sqrt_nan() {}
  static void logarithmic_nan() {}
  static void logarithmic_inf() {}
  static void trigonometric_nan() {}
  static void trigonometric_inf() {}
  static void hyperbolic_nan() {}
  static void hyperbolic_inf() {}
};

template<class T>
struct checking_strict
{
  static void inverted_bound(const T&, const T&) {
    throw std::invalid_argument("boost::interval: inverted bounds");
  }
  static void divide_by_zero(const T& l, const T& u) {
    if (l == 0 && u == 0)
      throw std::invalid_argument("boost::interval: division by zero");
  }
  static void sqrt_nan() {
    throw std::invalid_argument("boost::interval: square root of a negative interval");
  }
  static void logarithmic_nan() {
    throw std::invalid_argument("boost::interval: logarithmic nan");
  }
  static void logarithmic_inf() {}
  static void trigonometric_nan() {
    throw std::invalid_argument("boost::interval: trigonometric nan");
  }
  static void trigonometric_inf() {}
  static void hyperbolic_nan() {
    throw std::invalid_argument("boost::interval: hyperbolic nan");
  }
  static void hyperbolic_inf() {}
};

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_CHECKING_HPP
