#ifndef BOOST_MPL_APPLY_PACK_HPP_INCLUDED
#define BOOST_MPL_APPLY_PACK_HPP_INCLUDED
#include <boost/mpl/package.hpp>
#include <boost/mpl/arg.hpp>
namespace boost
{
  namespace mpl
  {
      template 
      < typename Pack
      , template<typename... Args>class Op
      > 
    struct apply_pack
    ;
      template 
      < typename... T
      , template<typename... Arg>class Op
      > 
    struct apply_pack
      < package<T...>
      , Op
      >
    : Op<T...>
    {
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
