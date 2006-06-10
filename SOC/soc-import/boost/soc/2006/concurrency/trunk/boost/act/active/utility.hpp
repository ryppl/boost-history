//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACTIVE_UTILITY_HPP
#define BOOST_ACT_ACTIVE_UTILITY_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include "../detail/active_call_enablers.hpp"
#include "../detail/type_from_function_params.hpp"
#include "../detail/param_list_container.hpp"
#include "../action.hpp"
#include "../action/as_target.hpp"


namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionType, typename DummyType >
struct dummy_get_result_type
{
  typedef typename ::boost::function_traits< FunctionType >::result_type type;
};

}
}
}

#define BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                           \
typename ::boost::mpl::apply                                                   \
<                                                                              \
  typename ActiveDetails::return_meta                                          \
, typename ::boost::act::detail::type_from_function_params                     \
    < void ret, ActiveDetails >::type                                          \
, typename ActiveDetails::act_model                                            \
>                                                                              \
::type 

// ToDo: Act Model
#define BOOST_ACT_DETAIL_MEM_FUNC_HEADER( ret, num_params, params )            \
template< BOOST_PP_ENUM_PARAMS( num_params, typename Param ) >                 \
typename detail::active_call_enabler                                           \
<                                                                              \
  typename ActiveDetails::act_model                                            \
, typename ::boost::act::detail::type_from_function_params                     \
    < void ret, ActiveDetails >::type                                          \
  ( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 1 ) ) ) )                      \
, ::boost::mpl::deque                                                          \
    < BOOST_PP_ENUM_BINARY_PARAMS( num_params                                  \
                                 , Param                                       \
                                 , const& BOOST_PP_INTERCEPT                   \
                                 )                                             \
    >                                                                          \
>                                                                              \
::type 

// ToDo: Somehow get act model 
#define BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER( ret, dummy1, dummy2 )          \
BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret ) 

#define BOOST_ACT_MEM_FUN_PARAM_MACRO( z, param_index, params )                \
BOOST_PP_CAT( Param, param_index ) const&                                      \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params ) 

#define BOOST_ACT_MEM_FUN_PARAM_IMPL_MACRO( z, param_index, params )           \
BOOST_PP_CAT( Param, param_index ) &                                           \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params ) 

#define BOOST_ACT_DETAIL_PARAM_NAME( z, param_index, params )                  \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params ) 

#define BOOST_ACT_DETAIL_FUN_PARAM( z, param_index, params )                   \
BOOST_PP_CAT( Param, param_index ) &                                           \
BOOST_ACT_DETAIL_PLIST_NAME( param_index, params ) 

// ToDo: Handle asynchronous and synchronous differently
//       when working with the default act model
//       (disallow non-polymorphic thread safety)

// ToDo: Use metafunction
//       Make version for functions returning void
//       Account for reference or const reference differently
//       Make const mem func version
//       Assert internally if invalid
//       Remove enabler on nullary member function
//       Account for z
//       Make a version of function traits which takes a dummy template param
//       Fix function_type
#define BOOST_ACT_MEM_FUN_OVERLOAD( id, ret, name, num, params, qualifier )    \
struct BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)        \
{                                                                              \
  typedef typename ::boost::act::detail::type_from_function_params             \
            < void ret, ActiveDetails >::type result_type;                     \
                                                                               \
  template< typename TargetType                                                \
            BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )               \
          >                                                                    \
  typename ::boost::act::detail::type_from_function_params                     \
    < void ret, TargetType >::type                                             \
  operator ()( TargetType& target                                              \
               BOOST_PP_ENUM_TRAILING_BINARY_PARAMS( num, Param, & arg )       \
             ) const                                                           \
  {                                                                            \
    return BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),_impl)     \
             ( target BOOST_PP_ENUM_TRAILING_PARAMS( num, arg ) );             \
  }                                                                            \
};                                                                             \
                                                                               \
BOOST_PP_IF( num                                                               \
           , BOOST_ACT_DETAIL_MEM_FUNC_HEADER                                  \
           , BOOST_ACT_DETAIL_EMPTY_MEM_FUNC_HEADER                            \
           )                                                                   \
           ( ret, num, params )                                                \
name( BOOST_PP_ENUM( num, BOOST_ACT_MEM_FUN_PARAM_MACRO, params ) ) qualifier  \
{                                                                              \
  return BOOST_ACT_DETAIL_MEM_FUNC_RETURN_TYPE( ret )                          \
         (                                                                     \
           ::boost::act::as_target                                             \
             ( static_cast< typename ActiveDetails::child_type qualifier& >    \
                 ( *this )                                                     \
             )                                                                 \
         , BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),struct)()  \
           BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_PARAM_NAME, params )  \
         );                                                                    \
}                                                                              \
                                                                               \
template< typename TargetType                                                  \
          BOOST_PP_ENUM_TRAILING_PARAMS( num, typename Param )                 \
        >                                                                      \
static                                                                         \
typename ::boost::act::detail::type_from_function_params                       \
  < void ret, TargetType >::type                                               \
BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(detail_,name),id),_impl)                \
( TargetType& target                                                           \
  BOOST_PP_ENUM_TRAILING( num, BOOST_ACT_DETAIL_FUN_PARAM, params )            \
) 

#define BOOST_ACT_DETAIL_NO_QUALIFIER 

#define BOOST_ACT_MEM_FUN( ret, name, params )                                 \
BOOST_ACT_MEM_FUN_OVERLOAD( detail_act_overload                                \
                          , ret                                                \
                          , name                                               \
                          , BOOST_ACT_DETAIL_PLIST_SIZE( params )              \
                          , params                                             \
                          , BOOST_ACT_DETAIL_NO_QUALIFIER                      \
                          ) 

#define BOOST_ACT_CONST_MEM_FUN( ret, name, params )                           \
BOOST_ACT_MEM_FUN_OVERLOAD( detail_act_overload                                \
                          , ret                                                \
                          , name                                               \
                          , BOOST_ACT_DETAIL_PLIST_SIZE( params )              \
                          , params                                             \
                          , const                                              \
                          ) 

//#define BOOST_ACT_CHAIN_MEM_FUN(child,num,ret,name,params,model)               \


#endif
