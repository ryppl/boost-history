#ifndef BOOST_MPL_LIST_C_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_LIST_C_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/list.hpp>
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
list_c
: list<integral_c<Integral,Args>...>
{
    typedef Integral value_type;
    typedef list_c type;
};

}//exit mpl namespace
}//exit boost namespace
#endif
