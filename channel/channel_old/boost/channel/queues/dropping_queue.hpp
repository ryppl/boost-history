//
// dropping_queue.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DROPPING_QUEUE_HPP
#define DROPPING_QUEUE_HPP

#include <deque>

namespace boost {
  namespace channel {

    /**
     * droppined queue: after reaching que size limit, old msgs will be dropped to give
     * space to new msgs
     */
    template<typename elem_type, typename synch_policy, typename platform>
    class dropping_que {
    public:
      typename synch_policy::mutex lock_;
      std::deque<elem_type> data_;
      size_t max_sz_;
      enum def_sz_type {
        def_sz = 100
      };
      dropping_que(int s = def_sz) :
        max_sz_(s) {
      }
      bool empty() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.empty();
      }
      size_t size() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.size();
      }
      void set_max_que_sz(size_t ms) {
        max_sz_ = ms;
      }
      size_t get_max_que_sz(void) {
        return max_sz_;
      }
      void put(elem_type & e) {
        typename synch_policy::scoped_lock lock(lock_);
        if (data_.size() >= max_sz_)
          data_.pop_front(); //remove old data
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

