
#ifndef BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
#define BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/aux_/config/lambda.hpp>

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) /**/
#   define BOOST_MPL_AUX_LAMBDA_SUPPORT(i,name,params) /**/

#else

#   include <boost/mpl/aux_/config/workaround.hpp>
#   include <boost/mpl/aux_/preprocessor/params.hpp>
#   include <boost/preprocessor/tuple/to_list.hpp>
#   include <boost/preprocessor/list/for_each_i.hpp>
#   include <boost/preprocessor/inc.hpp>
#   include <boost/preprocessor/cat.hpp>

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC(R,typedef_,i,param) \
    typedef_ param BOOST_PP_CAT(arg,BOOST_PP_INC(i)); \
    /**/

// agurt, 07/mar/03: restore an old revision for the sake of SGI MIPSpro C++
#if BOOST_WORKAROUND(__EDG_VERSION__, <= 238) 

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    BOOST_STATIC_CONSTANT(int, arity = i); \
    BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind \
    { \
        template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
            : name< BOOST_MPL_PP_PARAMS(i,U) > \
        { \
        }; \
    }; \
    /**/

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    /**/

#elif BOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(BOOST_INTEL_CXX_VERSION)
// agurt, 18/jan/03: old EDG-based compilers actually enforce 11.4 para 9
// (in strict mode), so we have to provide an alternative to the 
// MSVC-optimized implementation

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    BOOST_STATIC_CONSTANT(int, arity = i); \
    BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    struct rebind; \
/**/

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
template< BOOST_MPL_PP_PARAMS(i,typename T) > \
struct name<BOOST_MPL_PP_PARAMS(i,T)>::rebind \
{ \
    template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< BOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/

#else // __EDG_VERSION__

namespace boost { namespace mpl { namespace aux {
struct has_rebind_tag;
template< typename T > struct has_rebind;
}}}

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
    BOOST_STATIC_CONSTANT(int, arity = i); \
    BOOST_PP_LIST_FOR_EACH_I_R( \
          1 \
        , BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
        , typedef \
        , BOOST_PP_TUPLE_TO_LIST(i,params) \
        ) \
    friend class BOOST_PP_CAT(name,_rebind); \
    typedef BOOST_PP_CAT(name,_rebind) rebind; \
/**/

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
template< BOOST_MPL_PP_PARAMS(i,typename T) > \
char operator|( \
      ::boost::mpl::aux::has_rebind_tag \
    , ::boost::mpl::aux::has_rebind< name<BOOST_MPL_PP_PARAMS(i,T)> >* \
    ); \
/**/

#   if !defined(__BORLANDC__)
#   define BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class BOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
        : name< BOOST_MPL_PP_PARAMS(i,U) > \
    { \
    }; \
/**/
#   else
#   define BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
    BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class BOOST_PP_CAT(name,_rebind) \
{ \
 public: \
    template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
    { \
        typedef typename name< BOOST_MPL_PP_PARAMS(i,U) >::type type; \
    }; \
/**/
#   endif // __BORLANDC__

#endif // __EDG_VERSION__

#endif // BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#endif // BOOST_MPL_AUX_LAMBDA_SUPPORT_HPP_INCLUDED
