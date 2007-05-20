/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_HPP
#define BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_HPP

#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_scoped_consumer_impl.hpp>

#include <boost/act/consumer_policy/concurrent_consumer_policy/scoped_consumer_fwd.hpp>

#include <boost/act/consumer_policy/consumer_arguments.hpp>

namespace boost { namespace act { namespace consumer_policy {

template< typename ThreadPolicy, typename QueuePolicy
        , BOOST_ACT_CONSUMER_PARAMS()
        >
struct scoped_consumer< concurrent_consumer_policy< ThreadPolicy, QueuePolicy >
                      , BOOST_ACT_CONSUMER_ARGS()
                      >
{
  typedef detail::concurrent_scoped_consumer_impl
          <
            ThreadPolicy
          , QueuePolicy
          , typename BOOST_ACT_CONSUMER_TRAITS()::constraint
          , typename BOOST_ACT_CONSUMER_TRAITS()::processor_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::processor_shared_data_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::normalizer_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::denormalizer_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::normalized_type
          >
          type;
};

} } }

#endif
