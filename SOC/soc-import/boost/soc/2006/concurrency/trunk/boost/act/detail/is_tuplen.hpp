//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_IS_TUPLEN_HPP
#define BOOST_ACT_DETAIL_IS_TUPLEN_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/if.hpp>

#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

#define BOOST_ACT_DETAIL_IS_TUPLEN_MACRO( z, num_params )                      \
template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Elem ) >             \
struct BOOST_PP_CAT( is_tuple, num_params )                                    \
       <                                                                       \
         ::boost::tuple< BOOST_PP_ENUM_PARAMS_Z( z, num_params, Elem ) >       \
       >                                                                       \
  : ::boost::mpl::not_                                                         \
    <                                                                          \
      ::boost::is_same< BOOST_PP_CAT( Elem, BOOST_PP_DEC( num_params ) )       \
                      , ::boost::tuples::null_type                             \
                      >                                                        \
    >                                                                          \
{                                                                              \
}; 

#define BOOST_ACT_DETAIL_IS_TUPLE0_MACRO( dummy0, dummy1 )                     \
template<>                                                                     \
struct is_tuple0< ::boost::tuple<> >                                           \
  : ::boost::mpl::true_                                                        \
{                                                                              \
}; 

#define BOOST_ACT_DETAIL_IS_TUPLEN_REP_MACRO(z,num_params,dummy)               \
template< typename Type >                                                      \
struct BOOST_PP_CAT( is_tuple, num_params )                                    \
  : ::boost::mpl::false_                                                       \
{                                                                              \
};                                                                             \
                                                                               \
BOOST_PP_IF( num_params                                                        \
           , BOOST_ACT_DETAIL_IS_TUPLEN_MACRO                                  \
           , BOOST_ACT_DETAIL_IS_TUPLE0_MACRO                                  \
           )( z, num_params ) 

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_IS_TUPLEN_REP_MACRO
               , BOOST_PP_NIL
               )

}
}
}

#endif
