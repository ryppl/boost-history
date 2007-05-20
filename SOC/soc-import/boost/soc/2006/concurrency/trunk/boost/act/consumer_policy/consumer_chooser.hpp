/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONSUMER_CHOOSER_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONSUMER_CHOOSER_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>

namespace boost
{
namespace act
{

template< typename ConsumerPolicy, bool IsSafe, bool IsScoped
        , BOOST_ACT_CONSUMER_PARAMS()
        >
struct consumer_chooser_c
  : mpl::eval_if_c
    <
      IsSafe
    , mpl::if_c
      <
        IsScoped
      , typename ConsumerPolicy::safe_scoped_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      , typename ConsumerPolicy::safe_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      >
    , mpl::if_c
      <
        IsScoped
      , typename ConsumerPolicy::scoped_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      , typename ConsumerPolicy::consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      >
    >
{
};

template< typename ConsumerPolicy, typename IsSafe, typename IsScoped
        , BOOST_ACT_CONSUMER_PARAMS_DECL()
        >
struct consumer_chooser
  : consumer_chooser_c< ConsumerPolicy, IsSafe::value, IsScoped::value
                      , Normalize, Process
                      , ResultTypeMetaClass, ConsumeEnablerMetaClass
                      >
{
};

}
}

#endif
