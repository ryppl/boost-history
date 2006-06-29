//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/array/pop_front.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

#include "../../detail/type_from_function_params.hpp"
#include "../../detail/param_list_container.hpp"
#include "../../type_traits/is_algo_model.hpp"

// ToDo: include default algo model declaration

#define BOOST_ACT_DETAIL_MAKE_ALGO( t_params, return_type, name, params )      \
BOOST_ACT_DETAIL_DECLARE_ALGO( t_params, return_type, name, params )           \
BOOST_ACT_DETAIL_IMPLEMENT_ALGO( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION( name )                   \
namespace detail                                                               \
{                                                                              \
template< typename SurgeActDetailAlgoModel >                                   \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
::operator ()() const { return *this; }                                        \
                                                                               \
template< typename SurgeActDetailAlgoModel >                                   \
template< typename SurgeActDetailNewAlgoModel >                                \
typename ::boost::enable_if                                                    \
<                                                                              \
  ::boost::mpl::and_                                                           \
  <                                                                            \
    ::boost::is_same< SurgeActDetailAlgoModel, no_algo_model >                 \
  , is_algo_model< SurgeActDetailNewAlgoModel >                                \
  >                                                                            \
, BOOST_PP_CAT( name, _type )< SurgeActDetailNewAlgoModel >                    \
>                                                                              \
::type                                                                         \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
::operator []( SurgeActDetailNewAlgoModel ) const                              \
{                                                                              \
  return BOOST_PP_CAT( name, _type )< SurgeActDetailNewAlgoModel >();          \
}                                                                              \
}

#define BOOST_ACT_DETAIL_IMPLEMENT_ALGO( t_params, return_type, name, params ) \
BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION( name )                           \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER(t_params,return_type,name,params) \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL(t_params,return_type,name,params) \
namespace detail                                                               \
{                                                                              \
template< typename SurgeActDetailAlgoModel >                                   \
template< BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( t_params, 0 ) ) ) >              \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, SurgeActDetailAlgoModel >::type                 \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
::operator ()( BOOST_ACT_DETAIL_ENUM_PLIST                                     \
               ( ( 2, ( params, 0 ) ) )                                        \
             ) const                                                           \
{                                                                              \
  typedef typename ::boost::mpl::if_                                           \
          <                                                                    \
            ::boost::is_same< SurgeActDetailAlgoModel, no_algo_model >         \
          , default_algo_model                                                 \
          , SurgeActDetailAlgoModel                                            \
          >                                                                    \
          ::type algo_model;                                                   \
                                                                               \
  return BOOST_PP_CAT( ::boost::act::name, _impl )< algo_model >               \
           ::execute( BOOST_ACT_DETAIL_ENUM_PLIST_ARGS                         \
                      ( ( 2, ( params, 0 ) ) )                                 \
                    );                                                         \
}                                                                              \
}                                                                              \
                                                                               \
template< typename AlgoModel                                                   \
          BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST( ( 2, ( t_params, 0 ) ) )       \
        >                                                                      \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, AlgoModel >::type                               \
BOOST_PP_CAT( boost_act_detail_algo_impl_, name )                              \
( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 0 ) ) ) );                       \
                                                                               \
template< typename AlgoModel >                                                 \
struct BOOST_PP_CAT( name, _impl )                                             \
{                                                                              \
  template< BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( t_params, 0 ) ) ) >            \
  static typename ::boost::act::detail::type_from_function_params              \
           < void return_type, AlgoModel >::type                               \
  execute( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 0 ) ) ) )               \
  {                                                                            \
    return BOOST_PP_CAT( boost_act_detail_algo_impl_, name )< AlgoModel >      \
           ( BOOST_ACT_DETAIL_ENUM_PLIST_ARGS( ( 2, ( params, 0 ) ) ) );       \
  }                                                                            \
};                                                                             \
                                                                               \
template< typename AlgoModel                                                   \
          BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST( ( 2, ( t_params, 0 ) ) )       \
        >                                                                      \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, AlgoModel >::type                               \
BOOST_PP_CAT( boost_act_detail_algo_impl_, name )                              \
( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 0 ) ) ) )

#define BOOST_ACT_DETAIL_DECLARE_ALGO( t_params, return_type, name, params )   \
BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS( name, 1                               \
                                       , ( ( t_params, return_type, params ) ) \
                                       )

#define BOOST_ACT_DETAIL_MAKE_ALGO_OVER_DECL( t_params, return_type, params )  \
  template< BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( t_params, 0 ) ) ) >            \
  typename ::boost::act::detail::type_from_function_params                     \
            < void return_type, SurgeActDetailAlgoModel >::type                \
  operator ()( BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( params, 0 ) ) ) ) const;

#define BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS( name, num, overloads )        \
template< typename AlgoModel >                                                 \
struct BOOST_PP_CAT( name, _impl );                                            \
                                                                               \
namespace detail                                                               \
{                                                                              \
struct no_algo_model;                                                          \
                                                                               \
template< typename SurgeActDetailAlgoModel = no_algo_model >                   \
struct BOOST_PP_CAT( name, _type )                                             \
{                                                                              \
  BOOST_PP_CAT( name, _type ) operator ()() const;                             \
                                                                               \
  BOOST_PP_FOR( ( num, overloads )                                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_PREDICATE                      \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_OP                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_FOR_MACRO                      \
              )                                                                \
                                                                               \
  template< typename SurgeActDetailNewAlgoModel >                              \
  typename ::boost::enable_if                                                  \
  <                                                                            \
    ::boost::mpl::and_                                                         \
    <                                                                          \
      ::boost::is_same< SurgeActDetailAlgoModel, no_algo_model >               \
    , is_algo_model< SurgeActDetailNewAlgoModel >                              \
    >                                                                          \
  , BOOST_PP_CAT( name, _type )< SurgeActDetailNewAlgoModel >                  \
  >                                                                            \
  ::type                                                                       \
  operator []( SurgeActDetailNewAlgoModel ) const;                             \
};                                                                             \
}                                                                              \
                                                                               \
BOOST_PP_CAT( detail::name, _type )<> const name                               \
  = BOOST_PP_CAT( detail::name, _type )<>();


#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_PREDICATE( r, overloads )              \
BOOST_PP_ARRAY_SIZE( overloads )

#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_OP( r, overloads )                     \
BOOST_PP_ARRAY_POP_FRONT( overloads )

#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_FOR_MACRO( r, overloads )              \
BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_DECL                                      \
  ( BOOST_PP_ARRAY_ELEM( 0, overloads ) )

#define BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_DECL( param_list )                \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_DECL param_list

#endif
