/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_ARG_HPP
#define BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_ARG_HPP

#include <cstddef>

#include <boost/act/config/queue_policy/bounded_queue_policy.hpp>

#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy_arguments_fwd.hpp>

#include <boost/mpl/size_t.hpp>

#include <boost/parameter.hpp>

#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/act/detail/revise_argument.hpp>

// Note: This macros transforms max_size_c to max_size
#define BOOST_ACT_BOUNDED_QUEUE_POLICY_REVISED_ARGS()                          \
  BOOST_PP_ENUM_BINARY_PARAMS                                                  \
  (                                                                            \
    BOOST_ACT_NUM_BOUNDED_QUEUE_POLICY_PARAMS()                                \
  , typename ::boost::act::detail::revise_argument< Arg                        \
  , >::type BOOST_PP_INTERCEPT                                                 \
  )

namespace boost { namespace act {

namespace detail
{
typedef parameter::parameters
        <
          parameter::optional< tag::max_size_t >
        , parameter::optional< tag::allocator >
        >
        bounded_queue_policy_signature;
}

template< ::std::size_t MaxSize >
struct max_size
{
  typedef max_size_t< mpl::size_t< MaxSize > > revised_type;
};

template< BOOST_ACT_BOUNDED_QUEUE_POLICY_PARAMS() >
struct bounded_queue_policy_traits
{
private:
  typedef typename detail::bounded_queue_policy_signature::bind
          < BOOST_ACT_BOUNDED_QUEUE_POLICY_REVISED_ARGS() >::type
          boost_act_detail_args;
public:
  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::max_size_t
          , mpl::size_t< BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE() >
          >
          ::type max_size_t;

  typedef typename parameter::binding
          <
            boost_act_detail_args
          , tag::allocator
          , BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_ALLOCATOR()
          >
          ::type allocator;
};

#define BOOST_ACT_BOUNDED_QUEUE_POLICY_TRAITS()                                \
::boost::act::bounded_queue_policy_traits                                      \
< BOOST_ACT_BOUNDED_QUEUE_POLICY_ARGS() >

#define BOOST_ACT_BOUNDED_QUEUE_POLICY_MEMBERS()                               \
typedef typename BOOST_ACT_BOUNDED_QUEUE_POLICY_TRAITS()::max_size_t           \
                                                          max_size_t;          \
typedef typename BOOST_ACT_BOUNDED_QUEUE_POLICY_TRAITS()::allocator allocator;

} }

#endif
