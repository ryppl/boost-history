/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_TRACKER_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_TRACKER_HPP

#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_tracker_impl.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy_fwd.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/tracker_fwd.hpp>

namespace boost { namespace act { namespace consumer_policy {

// Todo: Push implementation to another type
template< typename ThreadPolicy, typename QueuePolicy, typename ValueType >
struct tracker< concurrent_consumer_policy< ThreadPolicy, QueuePolicy >
              , ValueType
              >
{
  typedef detail::concurrent_tracker_impl< ValueType > type;
};

} } }

#endif
