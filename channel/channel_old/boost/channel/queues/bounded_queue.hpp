//
// bounded_queue.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOUNDED_QUEUE_HPP
#define BOUNDED_QUEUE_HPP

#include <deque>

namespace boost {
  namespace channel {

    /**
     * size bounded queue:
     * . when queue is full, senders will be blocked
     * . when queue is empty, retrievers will be blocked
     */
    template<typename elem_type, typename synch_policy, typename platform>
    class bounded_que {
    public:
      typedef	typename platform::timeout_type timeout_type;
      typename synch_policy::mutex lock_;
      typename synch_policy::condition not_full_;
      typename synch_policy::condition not_empty_;
      int num_blocked_putter;
      int num_blocked_getter;
      std::deque<elem_type> data_;
      size_t max_sz_;
      timeout_type put_timeout_;
      timeout_type sett_timeout_;
      enum def_sz_type {def_sz = 100};
      bounded_que(int s = def_sz): max_sz_(s) {
        num_blocked_putter = 0;
        num_blocked_getter = 0;
      }
      void set_max_que_sz(size_t ms) {max_sz_ = ms;}
      size_t get_max_que_sz(void) {return max_sz_;}
      /*
        void set_timeout(timeout_type to);
        timeout_type get_timeout(void);
      */
      bool empty() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.empty();
      }
      bool full() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.size() >= max_sz_;
      }
      size_t size() {
        typename synch_policy::scoped_lock lock(lock_);
        return data_.size();
      }
      void put(elem_type & e) {
        typename synch_policy::scoped_lock lock(lock_);
        while (data_.size() >= max_sz_) {
          num_blocked_putter++;
          not_full_.wait(lock);
          num_blocked_putter--;
        }
        data_.push_back(e);
        if (num_blocked_getter > 0)
          not_empty_.notify_one();
      }
      void get(elem_type & e) {
        typename synch_policy::scoped_lock lock(lock_);
        while (data_.empty()) {
          num_blocked_getter++;
          not_empty_.wait(lock);
          num_blocked_getter--;
        }
        e = data_.front();
        data_.pop_front();
        if (num_blocked_putter > 0)
          not_full_.notify_one();
      }
    };

  }
}

#endif

