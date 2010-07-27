#ifndef BOOST_MPL_CONVERTER_BOOL_HPP_INCLUDED
#define BOOST_MPL_CONVERTER_BOOL_HPP_INCLUDED

#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/nested_type_wknd.hpp>

namespace boost
{
namespace mpl
{

      template
      < typename T
      >
    struct converter_bool
    : bool_<BOOST_MPL_AUX_NESTED_TYPE_WKND(T)::value>
    {};

}//exit mpl namespace
}//exit boost namespace
#endif
