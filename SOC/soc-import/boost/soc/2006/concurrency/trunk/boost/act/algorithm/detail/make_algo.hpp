//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

#include "../../detail/type_from_function_params.hpp"
#include "../../detail/param_list_container.hpp"
#include "../../type_traits/is_algo_model.hpp"


#define BOOST_ACT_DETAIL_MAKE_ALGO( t_params, return_type, name, params )      \
BOOST_ACT_DETAIL_MAKE_ALGO_DECL( t_params, return_type, name, params )         \
BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( t_params, return_type, name, params ) 

#define BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( t_params, return_type, name, params ) \
namespace detail                                                               \
{                                                                              \
                                                                               \
template< typename SurgeActDetailAlgoModel >                                   \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
BOOST_PP_CAT( name, _type )< SurgeActDetailAlgoModel >                         \
::operator ()() const { return *this; }                                        \
                                                                               \
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
                                                                               \
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


#define BOOST_ACT_DETAIL_MAKE_ALGO_DECL( t_params, return_type, name, params ) \
template< typename AlgoModel >                                                 \
struct BOOST_PP_CAT( name, _impl );                                            \
                                                                               \
namespace detail                                                               \
{                                                                              \
struct no_algo_model;                                                          \
                                                                               \
                                                                               \
template< typename SurgeActDetailAlgoModel = no_algo_model >                   \
struct BOOST_PP_CAT( name, _type )                                             \
{                                                                              \
  BOOST_PP_CAT( name, _type ) operator ()() const;                             \
                                                                               \
  template< BOOST_ACT_DETAIL_ENUM_PLIST( ( 2, ( t_params, 0 ) ) ) >            \
  typename ::boost::act::detail::type_from_function_params                     \
            < void return_type, SurgeActDetailAlgoModel >::type                \
  operator ()( BOOST_ACT_DETAIL_ENUM_PLIST                                     \
               ( ( 2, ( params, 0 ) ) )                                        \
             ) const;                                                          \
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
                                                                               \
}                                                                              \
                                                                               \
BOOST_PP_CAT( detail::name, _type )<> const name                               \
  = BOOST_PP_CAT( detail::name, _type )<>(); 

#endif

