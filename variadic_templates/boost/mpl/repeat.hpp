//package with repeated elements.
#ifndef BOOST_MPL_PACKAGE_REPEAT_HPP_INCLUDED
#define BOOST_MPL_PACKAGE_REPEAT_HPP_INCLUDED
#include <boost/mpl/at_c_always.hpp>
#include <boost/mpl/at_c_indices.hpp>
#include <boost/mpl/package_range_c.hpp>
namespace boost { namespace mpl {
  template
  < typename Element
  , unsigned Size=0
  >
struct repeat
{
        typedef
        typename
      at_c_indices
      < at_c_always<Element>::template at_c
      , typename package_range_c<unsigned,0,Size>::type
      >::type
    type
    ;
};
  
}}
#endif
