#ifndef BOOST_INTERVAL_CHECKING_HPP
#define BOOST_INTERVAL_CHECKING_HPP

#include <stdexcept>

namespace boost {
  namespace interval_lib {

template<class T>
struct checking_nothing
{
  static T inf() { return std::numeric_limits<T>::infinity(); }
  static T nan() { return std::numeric_limits<T>::quiet_NaN(); }
  static bool is_nan(const T& x) { return detail::is_nan(x); }
  BOOST_STATIC_CONSTANT(bool, test_nan_input = false);
  static T empty_lower() { return std::numeric_limits<T>::quiet_NaN(); }
  static T empty_upper() { return std::numeric_limits<T>::quiet_NaN(); }
  static bool is_empty(const T& x, const T& y) { return !(x <= y); }
  BOOST_STATIC_CONSTANT(bool, test_empty_input = false);
};

template<class T>
struct checking_lax: checking_nothing<T>
{};

template<class T>
struct checking_strict
{
  static T inf() { return std::numeric_limits<T>::infinity(); }
  static T nan() { return std::numeric_limits<T>::quiet_NaN(); }
  static bool is_nan(const T& x) { return detail::is_nan(x); }
  BOOST_STATIC_CONSTANT(bool, test_nan_input = false);
  static T empty_lower() { throw; }
  static T empty_upper() { throw; }
  static bool is_empty(const T& x, const T& y) { return !(x <= y); }
  BOOST_STATIC_CONSTANT(bool, test_empty_input = false);
};

    namespace detail {

template <class T> inline bool is_nan(const T& x) { return x != x; }

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& r) {
  typedef typename Traits::checking checking;
  return (checking::test_empty_input && empty(r));
}

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x, const interval<T, Traits>& y) {
  typedef typename Traits::checking checking;
  return (checking::test_empty_input && (empty(x) || empty(y)));
}

template<class T, class Traits> inline
bool test_input(const T& x, const interval<T, Traits>& y) {
  typedef typename Traits::checking checking;
  return (checking::test_nan_input && checking::is_nan(x) ||
	  checking::test_empty_input && empty(y));
}

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x, const T& y) {
  typedef typename Traits::checking checking;
  return (checking::test_empty_input && empty(x) ||
	  checking::test_nan_input && checking::is_nan(y));
}

template<class T, class Traits> inline
bool test_input(const T& x) {
  typedef typename Traits::checking checking;
  return (checking::test_nan_input && checking::is_nan(x));
}

template<class T, class Traits> inline
bool test_input(const T& x, const T& y) {
  typedef typename Traits::checking checking;
  return (checking::test_nan_input && checking::is_nan(x) ||
	  checking::test_nan_input && checking::is_nan(y));
}

    } // namespace detail

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_CHECKING_HPP
