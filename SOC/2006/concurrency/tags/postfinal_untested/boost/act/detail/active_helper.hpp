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
//#include "../action/action_no_default.hpp"


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
private:
  class active_object_destroyer
  {
  public:
    active_object_destroyer( ChildType const& target_init )
      : target_m( target_init )
    {
    }
  public:
    // ToDo: Make version which accounts for void
    //       (use metafunction which yields dummy type)
    void operator ()( RepresentedType const& target ) const
    {
      target_m.~ChildType();
    }
  private:
    // ToDo: Make version which accounts for void
    ChildType const volatile& target_m;
  };
protected:
  action< void, ActModel > destroy() const
  {
    return queue_function( active_object_destroyer( derived() ) );
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

BOOST_PP_REPEAT( BOOST_ACT_MAX_PARAMS
               , BOOST_ACT_DETAIL_ACTIVE_QUEUE_FUNC
               , BOOST_PP_NIL
               )
};

#define BOOST_ACT_DETAIL_PROMOTE_ACTIVE_MEMBERS( active_helper_base )          \
using active_helper_base::queue_function;                                      \
using active_helper_base::destroy;

}
}
}

#include "../type_traits/is_act_model.hpp"

#include "active_call_enablers.hpp"

#endif
