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
  static bool is_nan(const T&) { return false; }
  static T empty_lower() { return std::numeric_limits<T>::quiet_NaN(); }
  static T empty_upper() { return std::numeric_limits<T>::quiet_NaN(); }
  static bool is_empty(const T&, const T&) { return false; }
};

template<class T>
struct checking_strict
{
  static T inf() { return std::numeric_limits<T>::infinity(); }
  static T nan() { throw std::logic_error("boost::interval: NaN output"); }
  static bool is_nan(const T& x) {
      if (detail::is_nan(x))
        throw std::logic_error("boost::interval: NaN input");
      return false;
    }
  static T empty_lower()
    { throw std::logic_error("boost::interval: Empty output"); }
  static T empty_upper()
    { throw std::logic_error("boost::interval: Empty output"); }
  static bool is_empty(const T&, const T&) { return false; }
};

template<class T>
struct checking_complete
{
  static T inf() { return std::numeric_limits<T>::infinity(); }
  static T nan() { return std::numeric_limits<T>::quiet_NaN(); }
  static bool is_nan(const T& x) { return detail::is_nan(x); }
  static T empty_lower() { return T(1); }
  static T empty_upper() { return T(0); }
  static bool is_empty(const T& x, const T& y) { return !(x <= y); }
};

    namespace detail {

template <class T> inline bool is_nan(const T& x) { return x != x; }

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x) {
  typedef typename Traits::checking checking;
  return checking::is_empty(x.lower(), x.upper());
}

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x, const interval<T, Traits>& y) {
  typedef typename Traits::checking checking;
  return checking::is_empty(x.lower(), x.upper()) ||
	 checking::is_empty(y.lower(), y.upper());
}

template<class T, class Traits> inline
bool test_input(const T& x, const interval<T, Traits>& y) {
  typedef typename Traits::checking checking;
  return checking::is_nan(x) || checking::is_empty(y.lower(), y.upper());
}

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x, const T& y) {
  typedef typename Traits::checking checking;
  return checking::is_empty(x.lower(), x.upper()) || checking::is_nan(y);
}

template<class T, class Traits> inline
bool test_input(const T& x) {
  typedef typename Traits::checking checking;
  return checking::is_nan(x);
}

template<class T, class Traits> inline
bool test_input(const T& x, const T& y) {
  typedef typename Traits::checking checking;
  return checking::is_nan(x) || checking::is_nan(y);
}

    } // namespace detail

  } // namespace interval
} // namespace boost

#endif // BOOST_INTERVAL_CHECKING_HPP
