/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_SAFE_QUEUE_HPP
#define BOOST_ACT_QUEUE_POLICY_SAFE_QUEUE_HPP

#include <boost/act/queue_policy/safe_queue_fwd.hpp>
#include <boost/act/queue_policy/queue_arguments.hpp>
#include <boost/act/queue_policy/fully_safe_queue.hpp>
#include <boost/act/queue_policy/safe_single_push_queue.hpp>
#include <boost/act/queue_policy/safe_single_pop_queue.hpp>
#include <boost/act/queue_policy/safe_single_push_pop_queue.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace act { namespace queue_policy {

template< typename QueuePolicy, typename ValueType
        , typename Arg0, typename Arg1
        >
struct safe_queue
{
private:
  typedef act::detail::is_queue_push_info< Arg0 > arg0_is_push_info;
  typedef act::detail::is_queue_push_info< Arg1 > arg1_is_push_info;

  typedef act::detail::is_queue_pop_info< Arg0 > arg0_is_pop_info;
  typedef act::detail::is_queue_pop_info< Arg1 > arg1_is_pop_info;

  typedef is_same< Arg0, detail::queue_no_arg > arg0_is_no_arg;
  typedef is_same< Arg1, detail::queue_no_arg > arg1_is_no_arg;

  typedef mpl::and_< arg0_is_push_info
                   , mpl::or_< arg1_is_pop_info
                             , arg1_is_no_arg
                             >
                   >
                   use_arg0_push;

  typedef mpl::and_< arg0_is_pop_info
                   , mpl::or_< arg1_is_push_info
                             , arg1_is_no_arg
                             >
                   >
                   use_arg0_pop;

  typedef mpl::and_< arg1_is_push_info
                   , mpl::or_< arg0_is_pop_info
                             , arg0_is_no_arg
                             >
                   >
                   use_arg1_push;

  typedef mpl::and_< arg1_is_pop_info
                   , mpl::or_< arg0_is_push_info
                             , arg0_is_no_arg
                             >
                   >
                   use_arg1_pop;

  BOOST_MPL_ASSERT_MSG( !(arg0_is_push_info::value && arg1_is_push_info::value)
                      , TWO_PUSH_ARGUMENTS_PASSED_TO_SAFE_QUEUE
                      , (Arg0, Arg1)
                      );

  BOOST_MPL_ASSERT_MSG( !(arg0_is_pop_info::value && arg1_is_pop_info::value)
                      , TWO_POP_ARGUMENTS_PASSED_TO_SAFE_QUEUE
                      , (Arg0, Arg1)
                      );

  typedef typename mpl::eval_if< use_arg0_push
                               , mpl::identity< Arg0 >
                               , mpl::if_< use_arg1_push
                                         , Arg1
                                         , many_pushers
                                         >
                               >
                               ::type has_one_pusher;

  typedef typename mpl::eval_if< use_arg0_pop
                               , mpl::identity< Arg0 >
                               , mpl::if_< use_arg1_pop
                                         , Arg1
                                         , many_poppers
                                         >
                               >
                               ::type has_one_popper;
public:
  typedef typename mpl::eval_if
          <
            has_one_pusher
          , mpl::eval_if< has_one_popper
                        , queue_policy::safe_single_push_pop_queue
                          <
                            QueuePolicy, ValueType
                          >
                        , queue_policy::safe_single_push_queue
                          <
                            QueuePolicy, ValueType
                          >
                        >
          , mpl::eval_if< has_one_popper
                        , queue_policy::safe_single_pop_queue
                          <
                            QueuePolicy, ValueType
                          >
                        , queue_policy::fully_safe_queue
                          <
                            QueuePolicy, ValueType
                          >
                        >
          >
          ::type type;
};

} } }

#endif
