#ifndef BOOST_INTERVAL_DETAIL_TEST_INPUT_HPP
#define BOOST_INTERVAL_DETAIL_TEST_INPUT_HPP

#include <boost/interval/detail/interval_prototype.hpp>

namespace boost {

  namespace detail {
template<class T> inline
bool is_neg(const T& x) { return x < T(0); }
  } // namespace detail

  namespace interval_lib {
    namespace detail {

template<class T, class Traits> inline
bool test_input(const interval<T, Traits>& x) {
  typedef typename Traits::checking checking;
  return checking::is_empty(x.lower(), x.upper());
}

template<class T, class Traits1, class Traits2> inline
bool test_input(const interval<T, Traits1>& x, const interval<T, Traits2>& y) {
  typedef typename Traits1::checking checking1;
  typedef typename Traits2::checking checking2;
  return checking1::is_empty(x.lower(), x.upper()) ||
	 checking2::is_empty(y.lower(), y.upper());
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
  } // namespace interval_lib
} // namespace boost

#endif // BOOST_INTERVAL_DETAIL_TEST_INPUT_HPP
