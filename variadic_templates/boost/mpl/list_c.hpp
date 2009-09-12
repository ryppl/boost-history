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
  , Integral... Values
  >
  struct
list_c
  : list
    < integral_c<Integral,Values>...
    >
{
    typedef Integral value_type;
};

}//exit mpl namespace
}//exit boost namespace
#endif
