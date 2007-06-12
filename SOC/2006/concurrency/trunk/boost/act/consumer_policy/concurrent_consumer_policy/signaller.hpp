/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_SIGNALLER_HPP
#define BOOST_ACT_CONSUMER_POLICY_CONCURRENT_CONSUMER_POLICY_SIGNALLER_HPP

#include <boost/act/consumer_policy/signaller.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy_fwd.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_signaller_impl.hpp>

// ToDo: Handle void types
namespace boost { namespace act { namespace consumer_policy {

template< typename ThreadPolicy, typename QueuePolicy, typename ValueType >
struct signaller< concurrent_consumer_policy< ThreadPolicy, QueuePolicy >
                , ValueType
                >
{
  typedef detail::concurrent_signaller_impl< ValueType > type;
};

} } }

#endif
