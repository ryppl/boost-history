//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTIVE_HELPER_HPP
#define BOOST_ACT_DETAIL_ACTIVE_HELPER_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "../action/as_target.hpp"

#include "../act_fwd.hpp"
#include "detail_fwd.hpp"
#include "tie.hpp"
#include "result_type.hpp"
#include "../action.hpp"


#include "../config/max_active_call_params.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename ChildType, typename RepresentedType, typename ActModel >
class active_helper
{
private:
  ChildType& derived()
  {
    return static_cast< ChildType& >( *this );
  }

  ChildType const& derived() const
  {
    return static_cast< ChildType const& >( *this );
  }
protected:
#define BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNC( z, num_params, dummy )             \
template< typename FunctionType                                                \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
action< typename detail::result_type< FunctionType >::type, ActModel >         \
queue_function                                                                 \
(                                                                              \
  FunctionType function                                                        \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
)                                                                              \
{                                                                              \
  return action< typename detail::result_type< FunctionType >::type, ActModel >\
         (                                                                     \
           ::boost::act::as_target( derived() )                                \
         , function                                                            \
           BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, arg )               \
         );                                                                    \
}                                                                              \
                                                                               \
template< typename FunctionType                                                \
          BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )     \
        >                                                                      \
action< typename detail::result_type< FunctionType >::type, ActModel >         \
queue_function                                                                 \
(                                                                              \
  FunctionType function                                                        \
  BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, & arg )        \
) const                                                                        \
{                                                                              \
  return action< typename detail::result_type< FunctionType >::type, ActModel >\
         (                                                                     \
           ::boost::act::as_target( derived() )                                \
         , function                                                            \
           BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, arg )               \
         );                                                                    \
}

BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
               , BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNC
               , BOOST_PP_NIL
               )
};

#define BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )          \
using active_helper_base::queue_function;

}
}
}

#include "../type_traits/is_act_model.hpp"

#include "active_call_enablers.hpp"

#include "../config/default_act_model.hpp"

#endif
