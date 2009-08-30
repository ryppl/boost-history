#ifndef BOOST_MPL_LIST_HPP_INCLUDED
#define BOOST_MPL_LIST_HPP_INCLUDED
#include <boost/mpl/list/list0.hpp>

namespace boost
{
namespace mpl
{

  template
  < typename Head
  , typename... Tail
  >
  struct
list
  < Head
  , Tail...
  >
  : l_item
    < long_<1+sizeof...(Tail)>
    , Head
    , list<Tail...>
    >
//@diff-nv-mpl:
//  This specialization corresponds to all of the listI
//  for I=1..., which are defined in non-variadic mpl in 
//  one of the files:
//    boost/mpl/list/aux_/preprocessed/plain/listN.hpp
//  for some N in 10,20,...
{
    typedef list type;
};
  

}//exit mpl namespace
}//exit boost namespace
#endif
