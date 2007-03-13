/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONSUMER_CHOOSER_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONSUMER_CHOOSER_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>

namespace boost
{
namespace act
{

template< typename ConsumerAllocator, bool IsSafe, bool IsScoped
        , typename Normalize, typename Process
        , typename ResultTypeMetaClass = mpl::always< void >
        , typename ConsumeEnablerMetaClass = mpl::always< mpl::true_ >
        >
struct consumer_chooser_c
  : mpl::eval_if_c
    <
      IsSafe
    , mpl::if_c
      <
        IsScoped
      , typename ConsumerAllocator::safe_scoped_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      , typename ConsumerAllocator::safe_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      >
    , mpl::if_c
      <
        IsScoped
      , typename ConsumerAllocator::scoped_consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      , typename ConsumerAllocator::consumer
          < Normalize, Process, ResultTypeMetaClass, ConsumeEnablerMetaClass >
      >
    >
{
};

template< typename ConsumerAllocator, typename IsSafe, typename IsScoped
        , typename Normalize, typename Process
        , typename ResultTypeMetaClass = mpl::always< void >
        , typename ConsumeEnablerMetaClass = mpl::always< mpl::true_ >
        >
struct consumer_chooser
  : consumer_chooser_c< ConsumerAllocator, IsSafe::value, IsScoped::value
                      , Normalize, Process
                      , ResultTypeMetaClass, ConsumeEnablerMetaClass
                      >
{
};

}
}

#endif
