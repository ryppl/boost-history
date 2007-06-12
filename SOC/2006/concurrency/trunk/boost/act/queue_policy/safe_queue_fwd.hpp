/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_SAFE_QUEUE_FWD_HPP
#define BOOST_ACT_QUEUE_POLICY_SAFE_QUEUE_FWD_HPP

namespace boost { namespace act { namespace queue_policy {

namespace detail
{

struct queue_no_arg;

}

template< typename QueuePolicy, typename ValueType
        , typename Arg0 = detail::queue_no_arg
        , typename Arg1 = detail::queue_no_arg
        >
struct safe_queue;

} } }

#endif
