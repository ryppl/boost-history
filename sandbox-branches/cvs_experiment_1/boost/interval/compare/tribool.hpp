#ifndef BOOST_INTERVAL_COMPARE_TRIBOOL_HPP
#define BOOST_INTERVAL_COMPARE_TRIBOOL_HPP

#include <boost/interval/detail/interval_prototype.hpp>
#include <boost/interval/detail/test_input.hpp>
#include <boost/tribool.hpp>

namespace boost {
  namespace interval_lib {
    namespace compare {
      namespace tribool {

template<class T, class Policies1, class Policies2> inline
tribool operator<(const interval<T, Policies1>& x, const interval<T, Policies2>& y)
{
  if (detail::test_input(x, y)) throw comparison_error();
  if (x.upper() < y.lower()) return true;
  if (x.lower() < y.upper()) return false;
  return indeterminate;
}

// FIXME: to complete 

      } // namespace tribool
    } // namespace compare
  } // namespace interval_lib
} // namespace boost


#endif // BOOST_INTERVAL_COMPARE_TRIBOOL_HPP
