#ifndef BOOST_MPL_VECTOR_C_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_VECTOR_C_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/vector.hpp>
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
vector_c
  : vector
    < integral_c<Integral,Values>...
    >
{
    typedef Integral value_type;
};

}//exit mpl namespace
}//exit boost namespace
#endif
