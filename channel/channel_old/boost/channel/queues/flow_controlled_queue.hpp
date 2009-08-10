//
// flow_controlled_queue.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FLOW_CONTROLLED_QUEUE_HPP
#define FLOW_CONTROLLED_QUEUE_HPP

#include <deque>

namespace boost {
  namespace channel {

    /**
     * flow controled queue: follow the design of ACE Message_Queue:
     * 1. flow is controlled thru high/low water marks
     * 2. support priorities of enque and deque
     */
    template<typename elem_type, typename synch_policy, typename timeout_type>
    class flow_controlled_que {
    public:
    };
  }
}

#endif

