//variadic version of bind.hpp
#ifndef BOOST_MPL_BINDV_HPP_INCLUDED
#define BOOST_MPL_BINDV_HPP_INCLUDED
#include <boost/mpl/bind.hpp>
#include <boost/mpl/package.hpp>
#include <boost/mpl/fold_assoc_pack.hpp>
#include <boost/mpl/apply_pack.hpp>
#include <boost/mpl/join_pack.hpp>
#include <boost/mpl/repeat.hpp>
#include <boost/mpl/at.hpp>

namespace boost
{
  namespace mpl
  {
      template 
      < typename F
      , typename... T
      > 
    struct bindv
    ;
    
    namespace aux
    {
          template
          < typename T
          , typename U
          >
        struct resolve_bind_argn
        ;
          template
          < typename T
          , typename... U
          >
        struct resolve_bind_argn
          < T
          , package<U...>
          >
        : resolve_bind_arg
          < T
          #define RESOLVE_BIND_ARG_AT_C_US(z,n,data) , typename at_c<package<U...>,n>::type
          BOOST_PP_REPEAT(BOOST_MPL_LIMIT_METAFUNCTION_ARITY,RESOLVE_BIND_ARG_AT_C_US,~)
          #undef  RESOLVE_BIND_ARG_AT_C_US
          >
        {};
        
          template
          < typename T
          , typename... U
          >
        struct resolve_bind_argv
        : resolve_bind_argn
          < T
          , typename join_pack
            < package<U...>
            , typename repeat
              < na
              , BOOST_MPL_LIMIT_METAFUNCTION_ARITY - sizeof...(U)
              >::type
            >::type
          >
        {
        };
          template
          < int N
          , typename... U
          >
        struct resolve_bind_argv
          < arg < N >
          , U...
          >
        {
                typedef 
              typename apply_wrap 
              < mpl::arg < N >
              , U...
              >::type
            type;
        };
        
          template 
          < typename F
          , typename... T
          , typename... U
          > 
        struct resolve_bind_argv
          < bindv< F, T... >
          , U...
          >
        {
            typedef bindv< F, T...> f_;
	    typedef typename apply_wrap < f_, U...>::type type;
        };
      
          template
          < typename... U
          >
        struct resolve_bind
        {
              template
              < typename NTs
              , typename A
              >
            struct op
            ;
              template
              < typename N
              , typename... T
              , typename A
              >
            struct op
              < package<N,T...>
              , A
              >
            {
                typedef aux::replace_unnamed_arg < A, N> r;
                typedef typename r::type a;
                typedef typename r::next n;
                    typedef 
                  typename resolve_bind_argv
                  < a
                  , U...
                  >::type 
                t;
                
                typedef package<n,T...,t> type;
            };
        };
    }
    
      template 
      < typename F
      , typename... T
      > 
    struct bindv
    {
          template 
          < typename... U
          > 
        struct apply
        {
        #if 0
         private:
        #endif
                typedef
              aux::resolve_bind<U...>
            resolve_bind_us
            ;
                typedef
              typename fold_assoc_pack
              < assoc_left
              , resolve_bind_us::template op
              , package<mpl::arg<1> >
              , F,T...
              >::type
            resolveds
            ;
                typedef
              typename pop_front<resolveds>::type
            fts
            ;
                typedef
              typename front<fts>::type
            f_
            ;
                typedef
              typename pop_front<fts>::type
            ts
            ;
              template
              < typename... V
              >
            struct apply_wrap_f
            : apply_wrap
              < f_
              , typename V::type...
              >
            {};
         public:
                typedef 
                typename
              apply_pack
              < ts
              , apply_wrap_f
              >
              ::type
            type;
        };
       
    };
    
  }//exit mpl namespace
}//exit boost namespace  

#endif
