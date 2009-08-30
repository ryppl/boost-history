#ifndef BOOST_MPL_DEQUE_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_DEQUE_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/vector.hpp>

namespace boost
{
namespace mpl
{
  template
  < typename... Args
  >
  struct
deque
  : vector<Args...>
{
};

}//exit mpl namespace
}//exit boost namespace
#endif
