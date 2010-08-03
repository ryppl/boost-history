#ifndef BOOST_MPL_TRANSFORM_PACK_HPP_INCLUDED
#define BOOST_MPL_TRANSFORM_PACK_HPP_INCLUDED
#include <boost/mpl/package.hpp>
#include <boost/mpl/arg.hpp>
namespace boost
{
  namespace mpl
  {
      template 
      < typename Pack
      , typename Op
      > 
    struct transform_pack
    ;
      template 
      < typename... T
      , template<typename Arg>class Op
      > 
    struct transform_pack
      < package<T...>
      , Op<arg<1> >
      >
    {
            typedef 
          package
          < typename Op<T>::type...
          >
        type
        ;
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
