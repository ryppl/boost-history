#ifndef BOOST_MPL_LIST_HPP_INCLUDED
#define BOOST_MPL_LIST_HPP_INCLUDED
#include <boost/mpl/list/list0.hpp>
#include <boost/mpl/fold_assoc_pack.hpp>

namespace boost
{
namespace mpl
{

  template
  < typename... Values
  >
  struct
list
//@diff-nv-mpl:
//  This specialization corresponds to all of the listI
//  for I=1..., which are defined in non-variadic mpl in 
//  one of the files:
//    boost/mpl/list/aux_/preprocessed/plain/listN.hpp
//  for some N in 10,20,...
  : fold_assoc_pack
    < assoc_right
    , l_item_fold
    , list0
    , Values...
    >::type
{
  #ifdef TYPEDEF_SEQ_TYPE
    typedef list type;
  #endif
};

  

}//exit mpl namespace
}//exit boost namespace
#endif
