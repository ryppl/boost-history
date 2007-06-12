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
