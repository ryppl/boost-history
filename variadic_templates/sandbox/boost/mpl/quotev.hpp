//variadic version of quote.hpp
#ifndef BOOST_MPL_QUOTEV_HPP_INCLUDED
#define BOOST_MPL_QUOTEV_HPP_INCLUDED
#include <boost/mpl/void.hpp>
#include <boost/mpl/quote.hpp>

namespace boost
{
  namespace mpl
  {
      template 
      < template < typename... P> class F
      , typename Tag = void_
      > 
    struct quotev
    {
        template < typename... U > struct apply
        : quote_impl < F < U... >, aux::has_type < F < U... > >::value >
        {
        };
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
