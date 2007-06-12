/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_EXECUTE_IF_HPP
#define BOOST_ACT_DETAIL_EXECUTE_IF_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mpl/if.hpp>

namespace boost
{
namespace act
{
namespace detail
{

struct empty_execute
{
#define BOOST_ACT_DETAIL_MAX_NUM_EXECUTE_IF_PARAMS 20 

static void execute()
{
}

// ToDo: Rewrite using file iteration

#define BOOST_ACT_DETAIL_EMPTY_EXECUTE_FUN( z, num_params, dummy )             \
template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >            \
static void execute( BOOST_PP_ENUM_BINARY_PARAMS_Z( z                          \
                                                  , num_params                 \
                                                  , Param                      \
                                                  , const& BOOST_PP_INTERCEPT  \
                                                  )                            \
                   )                                                           \
{                                                                              \
}

BOOST_PP_REPEAT_FROM_TO
(
  1, BOOST_PP_INC( BOOST_ACT_DETAIL_MAX_NUM_EXECUTE_IF_PARAMS )
, BOOST_ACT_DETAIL_EMPTY_EXECUTE_FUN, BOOST_PP_NIL
)

};

template< bool Condition
        , typename IfTrueFun
        , typename IfFalseFun = empty_execute
        >
struct execute_if_c
  : mpl::if_c< Condition
             , IfTrueFun
             , IfFalseFun
             >
             ::type
{
};

template< typename Condition
        , typename IfTrueFun
        , typename IfFalseFun = empty_execute
        >
struct execute_if
  : execute_if_c< Condition::value
                , IfTrueFun
                , IfFalseFun
                >
{
};

}
}
}

#endif
