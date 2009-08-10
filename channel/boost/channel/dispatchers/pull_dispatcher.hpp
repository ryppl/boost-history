//
// pull_dispatcher.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef PULL_DISPATCHER_HPP
#define PULL_DISPATCHER_HPP

#include <boost/channel/queues/unbounded_queue.hpp>
#include <boost/channel/dispatchers/pull_dispatcher_base.hpp>
#include <boost/channel/dispatchers/arbiters_async.hpp>
#include <boost/channel/dispatchers/arbiters_sync.hpp>

namespace boost {
  namespace channel {

    template<typename name_space, typename platform,
             template<class , class , class > class queue_type = unbounded_que>
    struct pull_dispatcher {
      typedef detail::pull_sender_base<name_space, platform, queue_type> sender;
      typedef detail::pull_recver_base<name_space, platform, queue_type> recver;
      typedef detail::choice_arbiter_async<name_space, platform, queue_type>
      choice_async;
      typedef detail::join_arbiter_async<name_space, platform, queue_type>
      join_async;
      typedef detail::choice_arbiter_sync<name_space, platform, queue_type>
      choice_sync;
      typedef detail::join_arbiter_sync<name_space, platform, queue_type>
      join_sync;
    };

  }
}

#endif
