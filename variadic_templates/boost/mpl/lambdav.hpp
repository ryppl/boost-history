//variadic version of lambda.hpp
#ifndef BOOST_MPL_LAMBDAV_HPP_INCLUDED
#define BOOST_MPL_LAMBDAV_HPP_INCLUDED
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/transform_pack.hpp>
#include <boost/mpl/apply_pack.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/quotev.hpp>
#include <boost/mpl/bindv.hpp>

namespace boost
{
  namespace mpl
  {
    namespace aux
    {
          template
          < typename LeMaybe
          >
        struct is_le
        {
            typedef typename LeMaybe::is_le type;
        };
        
          template 
          < typename IsLE
          , typename Tag
          , template < typename... P> class F
          > 
        struct le_resultv
        { 
              template
              < typename... L
              >
            struct _
            {
                typedef F < typename L::type...> result_;

	        typedef result_ type;
            };
        };

          template 
          < typename Tag
          , template < typename... P> class F
          >
        struct le_resultv
          < true_
          , Tag
          , F
          >
        {
              template
              < typename... L
              >
            struct _
            {
                typedef bindv < quotev< F, Tag >, typename L::result_...> result_;

                typedef mpl::protect < result_ > type;
            };
        };
    }
    
      template 
      < template < typename... P > class F
      , typename... T
      , typename Tag
      > 
    struct lambda
      < F< T... >
      , Tag
      , int_<sizeof...(T)>
      >
    {
        typedef package<lambda < T, Tag >...> ls;
        typedef typename transform_pack<ls, aux::is_le<arg<1> > >::type is_les;

        typedef typename apply_pack<is_les, or_>::type is_le;
        
            typedef 
          apply_pack
          < ls
          , aux::le_resultv < is_le, Tag, F>::template _
          >
        le_result_;

        typedef typename le_result_::result_ result_;
        typedef typename le_result_::type type;
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
