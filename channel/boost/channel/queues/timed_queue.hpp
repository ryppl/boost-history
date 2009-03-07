//
// timed_queue.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TIMED_QUEUE_HPP
#define TIMED_QUEUE_HPP

#include <deque>

namespace boost {
  namespace channel {

    /**
     * timed queue: msgs will be dropped after waiting in queue for some time
     * following JavaSpace in its use of "Lease" on entries in space, when
     * lease expires, the entry is removed
     */
    template<typename elem_type, typename synch_policy, typename timeout_type>
    class timed_que {
    public:
    };

  }
}

#endif

