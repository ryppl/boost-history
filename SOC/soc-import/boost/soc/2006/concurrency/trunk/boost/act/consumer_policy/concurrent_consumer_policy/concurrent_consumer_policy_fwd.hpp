/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_POLICY_FWD_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_POLICY_FWD_HPP

#include <boost/act/worker_policy/thread_policy/basic_thread_policy_fwd.hpp>
#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy_fwd.hpp>

namespace boost { namespace act {

// ToDo: Change to use Boost.Parameter

// ToDo: Make defaults toggleable
template< typename ThreadPolicy = basic_thread_policy
        , typename QueuePolicy = bounded_queue_policy<>
        >
class concurrent_consumer_policy;

} }

#endif
