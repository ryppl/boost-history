#ifndef BOOST_MPL_AT_C_INDICES_HPP_INCLUDED
#define BOOST_MPL_AT_C_INDICES_HPP_INCLUDED
#include <boost/mpl/package_c.hpp>
namespace boost
{
  namespace mpl
  {
      template 
      < template<typename IndexOp, IndexOp I>class Op
      , typename Indices=package_c<unsigned>
      , template<typename...>class Seq=package
      > 
    struct at_c_indices
    ;
      template 
      < typename Index
      , Index... Indices
      , template<typename IndexOp, IndexOp I>class Op
      , template<typename...>class Seq
      > 
    struct at_c_indices
      < Op
      , package_c<Index, Indices...>
      , Seq
      >
    : Seq<typename Op<Index,Indices>::type...>
    {
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
