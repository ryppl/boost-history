//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_MAKE_ALGO_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/array/pop_front.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/repetition/for.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>

#include "../../detail/type_from_function_params.hpp"
#include "../../detail/param_list_container.hpp"
#include "../../type_traits/is_algo_model.hpp"
#include "../../type_traits/are_extended_algo_params.hpp"
#include "../../default_algo_model/default_algo_model_fwd.hpp"

// ToDo: Include proper files for mpl and other stuff

namespace boost
{
namespace act
{
namespace detail
{
template< typename AlgoModel >
struct no_extended_algo_params
{
  no_extended_algo_params() {}
  no_extended_algo_params( AlgoModel const& ) {}
};

template< typename AlgoModel >
struct is_no_extended_algo_params
  : mpl::false_
{
};

template< typename AlgoModel >
struct is_no_extended_algo_params< no_extended_algo_params< AlgoModel > >
  : mpl::true_
{
};

}

template< typename AlgoModel, typename Enabler >
struct associated_algo_model< detail::no_extended_algo_params< AlgoModel >
                            , Enabler
                            >
{
  typedef AlgoModel type;
};

template< typename Type >
struct are_extended_algo_params< detail::is_no_extended_algo_params< Type > >
  : mpl::true_
{
};

}
}
/*
typename ::boost::enable_if                                                    \
<                                                                              \
  ::boost::mpl::and_                                                           \
  <                                                                            \
    ::boost::act::detail::is_no_extended_algo_params                           \
      < ExtendedParamsType >                                                   \
  , ::boost::mpl::or_< ::boost::act::is_algo_model< NewExtendedParamsType >    \
                     , ::boost::act::are_extended_algo_params                  \
                         < NewExtendedParamsType >                             \
                     >                                                         \
  >                                                                            \
, BOOST_PP_CAT( name, _type )                                                  \
  < typename ::boost::mpl::if_                                                 \
    < ::boost::act::is_algo_model< NewExtendedParamsType >                     \
    , ::boost::act::detail::no_extended_algo_params< NewExtendedParamsType >   \
    , NewExtendedParamsType                                                    \
    >                                                                          \
    ::type                                                                     \
  >                                                                            \
>                                                                              \
::type                                                                         \
BOOST_PP_CAT( name, _type )< ExtendedParamsType >                              \
::operator []( NewExtendedParamsType const& extended_params ) const            \
*/

#define BOOST_ACT_DETAIL_MAKE_ALGO( t_params, return_type, name, params )      \
BOOST_ACT_DETAIL_DECLARE_ALGO( t_params, return_type, name, params )           \
BOOST_ACT_DETAIL_IMPLEMENT_ALGO( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_PREPARE_ALGO_OVER( t_params, return_type, params )    \
  template< typename ExtendedParamsType                                        \
          , BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                               \
              ( BOOST_ACT_SIMPLE_PLIST( t_params ) )                           \
          >                                                                    \
  static typename ::boost::act::detail::type_from_function_params              \
            < void return_type, ExtendedParamsType >::type                     \
  execute( ExtendedParamsType const& extended_params                           \
           BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                                \
             ( BOOST_ACT_SIMPLE_PLIST( params ) )                              \
         )                                                                     \

// ToDo: Change operator[] return (use lazy_enable_if
#define BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION( name, num, overloads )   \
namespace detail                                                               \
{                                                                              \
template< typename ExtendedParamsType >                                        \
BOOST_PP_CAT( name, _type )< ExtendedParamsType >                              \
BOOST_PP_CAT( name, _type )< ExtendedParamsType >                              \
::operator ()() const { return *this; }                                        \
                                                                               \
template< typename ExtendedParamsType >                                        \
template< typename NewExtendedParamsType >                                     \
typename ::boost::enable_if                                                    \
<                                                                              \
  ::boost::mpl::true_                                                          \
, BOOST_PP_CAT( name, _type )                                                  \
  < typename ::boost::mpl::if_                                                 \
    < ::boost::act::is_algo_model< NewExtendedParamsType >                     \
    , ::boost::act::detail::no_extended_algo_params< NewExtendedParamsType >   \
    , NewExtendedParamsType                                                    \
    >                                                                          \
    ::type                                                                     \
  >                                                                            \
>                                                                              \
::type                                                                         \
BOOST_PP_CAT( name, _type )< ExtendedParamsType >                              \
::operator []( NewExtendedParamsType const& extended_params ) const            \
{                                                                              \
  typedef typename ::boost::mpl::if_                                           \
          < ::boost::act::is_algo_model< NewExtendedParamsType >               \
          , ::boost::act::detail::no_extended_algo_params                      \
              < NewExtendedParamsType >                                        \
          , NewExtendedParamsType                                              \
          >                                                                    \
          ::type extended_params_type;                                         \
                                                                               \
  return BOOST_PP_CAT( name,_type )< extended_params_type >( extended_params );\
}                                                                              \
}                                                                              \
                                                                               \
template< typename AlgoModel >                                                 \
struct BOOST_PP_CAT( name, _impl )                                             \
{                                                                              \
  BOOST_PP_FOR( ( num, overloads )                                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_PREDICATE                      \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_OP                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_PREP_FOR_MACRO                      \
              )                                                                \
};

#define BOOST_ACT_DETAIL_IMPLEMENT_ALGO( t_params, return_type, name, params ) \
BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION                                   \
  ( name, 1, ( ( t_params, return_type, params ) ) )                           \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER(t_params,return_type,name,params) \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL( t_params, return_type, name, params )

#define BOOST_ACT_DETAIL_MAKE_ALGO_OVER_IMPL(t_params,return_type,name,params) \
namespace detail                                                               \
{                                                                              \
template< typename ExtendedParamsType >                                        \
template< BOOST_ACT_DETAIL_ENUM_PLIST( BOOST_ACT_SIMPLE_PLIST( t_params ) ) >  \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, ExtendedParamsType >::type                      \
BOOST_PP_CAT( name, _type )< ExtendedParamsType >                              \
::operator ()( BOOST_ACT_DETAIL_ENUM_PLIST                                     \
               ( BOOST_ACT_SIMPLE_PLIST( params ) )                            \
             ) const                                                           \
{                                                                              \
  typedef typename ::boost::act::associated_algo_model                         \
            < ExtendedParamsType >                                             \
          ::type algo_model;                                                   \
                                                                               \
  return BOOST_PP_CAT( ::boost::act::name, _impl )< algo_model >               \
           ::execute( extended_params()                                        \
                      BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST_ARGS                \
                      ( BOOST_ACT_SIMPLE_PLIST( params ) )                     \
                    );                                                         \
}                                                                              \
}                                                                              \
                                                                               \
template< typename AlgoModel                                                   \
        , typename ExtendedParamsType                                          \
          BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                                 \
            ( BOOST_ACT_SIMPLE_PLIST( t_params ) )                             \
        >                                                                      \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, ExtendedParamsType >                            \
         ::type                                                                \
BOOST_PP_CAT( boost_act_detail_algo_impl_, name )                              \
( ExtendedParamsType const& extended_params                                    \
  BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST( BOOST_ACT_SIMPLE_PLIST( params ) )     \
);                                                                             \
                                                                               \
template< typename AlgoModel >                                                 \
template< typename ExtendedParamsType                                          \
            BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                               \
              ( BOOST_ACT_SIMPLE_PLIST( t_params ) )                           \
          >                                                                    \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, ExtendedParamsType >::type                      \
BOOST_PP_CAT( name, _impl )< AlgoModel >::execute                              \
  ( ExtendedParamsType const& extended_params                                  \
    BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                                       \
      ( BOOST_ACT_SIMPLE_PLIST( params ) )                                     \
  )                                                                            \
{                                                                              \
  return BOOST_PP_CAT( boost_act_detail_algo_impl_, name )                     \
           < AlgoModel >                                                       \
         ( extended_params                                                     \
           BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST_ARGS                           \
             ( BOOST_ACT_SIMPLE_PLIST( params ) )                              \
         );                                                                    \
}                                                                              \
                                                                               \
template< typename AlgoModel                                                   \
        , typename ExtendedParamsType                                          \
          BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                                 \
            ( BOOST_ACT_SIMPLE_PLIST( t_params ) )                             \
        >                                                                      \
typename ::boost::act::detail::type_from_function_params                       \
           < void return_type, ExtendedParamsType >                            \
         ::type                                                                \
BOOST_PP_CAT( boost_act_detail_algo_impl_, name )                              \
( ExtendedParamsType const& extended_params                                    \
  BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST( BOOST_ACT_SIMPLE_PLIST( params ) )     \
)

#define BOOST_ACT_DETAIL_DECLARE_ALGO( t_params, return_type, name, params )   \
BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS( name, 1                               \
                                       , ( ( t_params, return_type, params ) ) \
                                       )

#define BOOST_ACT_DETAIL_MAKE_ALGO_OVER_PREP( t_params, return_type, params )  \
  template< typename ExtendedParamsType                                        \
            BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                               \
              ( BOOST_ACT_SIMPLE_PLIST( t_params ) )                           \
          >                                                                    \
  static typename ::boost::act::detail::type_from_function_params              \
           < void return_type, ExtendedParamsType >::type                      \
  execute( ExtendedParamsType const& extended_params                           \
           BOOST_ACT_DETAIL_ENUM_TRAILING_PLIST                                \
             ( BOOST_ACT_SIMPLE_PLIST( params ) )                              \
         );                                                                    \


#define BOOST_ACT_DETAIL_MAKE_ALGO_OVER_DECL( t_params, return_type, params )  \
  template< BOOST_ACT_DETAIL_ENUM_PLIST( BOOST_ACT_SIMPLE_PLIST( t_params ) ) >\
  typename ::boost::act::detail::type_from_function_params                     \
            < void return_type, ExtendedParamsType >::type                     \
  operator ()( BOOST_ACT_DETAIL_ENUM_PLIST                                     \
                 ( BOOST_ACT_SIMPLE_PLIST( params ) )                          \
             ) const;

// ToDo: Fix this (use different base, move accessors to there, make mutable)
#define BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS( name, num, overloads )        \
template< typename ExtendedParamsType >                                        \
struct BOOST_PP_CAT( name, _impl );                                            \
                                                                               \
namespace detail                                                               \
{                                                                              \
template< typename ExtendedParamsType                                          \
            = ::boost::act::detail::no_extended_algo_params                    \
                < ::boost::act::default_algo_model >                           \
        >                                                                      \
struct BOOST_PP_CAT( name, _type )                                             \
  : private ExtendedParamsType                                                 \
{                                                                              \
  BOOST_PP_CAT( name, _type )() {}                                             \
                                                                               \
  BOOST_PP_CAT( name, _type )                                                  \
    ( ExtendedParamsType const& extended_params_init )                         \
    : ExtendedParamsType( extended_params_init )                               \
  {                                                                            \
  }                                                                            \
                                                                               \
  BOOST_PP_CAT( name, _type ) operator ()() const;                             \
                                                                               \
  BOOST_PP_FOR( ( num, overloads )                                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_PREDICATE                      \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_OP                             \
              , BOOST_ACT_DETAIL_ALGO_OVER_DECL_FOR_MACRO                      \
              )                                                                \
                                                                               \
  template< typename NewExtendedParamsType >                                   \
  typename ::boost::enable_if                                                  \
  <                                                                            \
    ::boost::mpl::true_                                                        \
  , BOOST_PP_CAT( name, _type )                                                \
    < typename ::boost::mpl::if_                                               \
      < ::boost::act::is_algo_model< NewExtendedParamsType >                   \
      , ::boost::act::detail::no_extended_algo_params< NewExtendedParamsType > \
      , NewExtendedParamsType                                                  \
      >                                                                        \
      ::type                                                                   \
    >                                                                          \
  >                                                                            \
  ::type                                                                       \
  operator []( NewExtendedParamsType const& extended_params ) const;           \
                                                                               \
  ExtendedParamsType extended_params() const                                   \
  {                                                                            \
    return *static_cast< ExtendedParamsType const* >( this );                  \
  }                                                                            \
};                                                                             \
}                                                                              \
                                                                               \
BOOST_PP_CAT( detail::name, _type )<> const name;

#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_PREDICATE( r, overloads )              \
BOOST_PP_ARRAY_SIZE( overloads )

#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_OP( r, overloads )                     \
BOOST_PP_ARRAY_POP_FRONT( overloads )

#define BOOST_ACT_DETAIL_ALGO_OVER_DECL_FOR_MACRO( r, overloads )              \
BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_DECL                                      \
  ( BOOST_PP_ARRAY_ELEM( 0, overloads ) )

#define BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_DECL( param_list )                \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_DECL param_list

#define BOOST_ACT_DETAIL_ALGO_OVER_PREP_FOR_MACRO( r, overloads )              \
BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_PREP                                      \
  ( BOOST_PP_ARRAY_ELEM( 0, overloads ) )

#define BOOST_ACT_DETAIL_CALL_MAKE_ALGO_OVER_PREP( param_list )                \
BOOST_ACT_DETAIL_MAKE_ALGO_OVER_PREP param_list

#endif
