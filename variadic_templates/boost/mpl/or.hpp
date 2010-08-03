#ifndef BOOST_MPL_OR_HPP_INCLUDED
#define BOOST_MPL_OR_HPP_INCLUDED

#include <boost/mpl/fold_null_unit.hpp>
#include <boost/mpl/converter_bool.hpp>
#include <boost/mpl/aux_/template_arityv.hpp>

namespace boost
{
namespace mpl
{

      template
      < typename... T
      >
    struct or_
    : fold_null_unit
      < true_
      , false_
      , converter_bool
      , T...
      >
    {
    };

}//exit mpl namespace
}//exit boost namespace
#endif
