//
// unbounded_queue.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UNBOUNDED_QUEUE_HPP
#define UNBOUNDED_QUEUE_HPP

#include <deque>

namespace boost {
  namespace channel {

    /**
     * the simplest msg_que: unbounded size
     */
    template<typename elem_type, typename synch_policy, typename platform>
    class unbounded_que {
    public:
      std::deque<elem_type> data_;
      typename synch_policy::mutex lock_;
      bool empty() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.empty();
      }
      size_t size() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.size();
      }
      void put(elem_type & e) {
        typename synch_policy::scoped_lock lock(lock_);
        data_.push_back(e);
      }
      void get(elem_type & e) {
        typename synch_policy::scoped_lock lock(lock_);
        if (!data_.empty()) {
          e = data_.front();
          data_.pop_front();
        }
      }
    };

  }
}

#endif

