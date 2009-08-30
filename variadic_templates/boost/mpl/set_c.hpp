#ifndef BOOST_MPL_SET_C_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_SET_C_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/set.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost
{
namespace mpl
{
  template
  < typename Integral
  , Integral... Args
  >
  struct
set_c
  : set<integral_c<Integral,Args>...>
{
    typedef Integral value_type;
    typedef set_c type;
};

}//exit mpl namespace
}//exit boost namespace
#endif

