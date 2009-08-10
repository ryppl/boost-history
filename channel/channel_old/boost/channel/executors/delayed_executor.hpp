//
// delayed_executor.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DELAYED_EXECUTOR_HPP
#define DELAYED_EXECUTOR_HPP

#include <deque>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/channel/executors/executor_base.hpp>

namespace boost {
  namespace channel {

    //in single-thread async app: operations can be delayed:
    //executed later by the same (single) thread, the original calling context is gone
    template<typename synch_policy>
    class delayed_executor {
    public:
      std::deque<async_task_base *> tasks_;
      typename synch_policy::mutex mutex_;

      delayed_executor() {
      }
      ~delayed_executor() {
        typename synch_policy::scoped_lock lock(mutex_);
        typename std::deque<async_task_base *>::iterator iter;
        for (iter = tasks_.begin(); iter != tasks_.end(); iter++)
          (*iter)->destroy();
      }
      template<typename task_type>
      async_task_base * execute(task_type task) {
        async_task_base *t = new async_task<task_type> (task);
        typename synch_policy::scoped_lock lock(mutex_);
        tasks_.push_back(t);
        return t;
      }
      bool cancel(async_task_base *task) {
        typename synch_policy::scoped_lock lock(mutex_);
        typename std::deque<async_task_base *>::iterator iter;
        iter = std::find(tasks_.begin(), tasks_.end(), task);
        if (iter != tasks_.end()) {
          (*iter)->destroy();
          tasks_.erase(iter);
          return true;
        }
        return false;
      }
      //complete scheduled tasks
      void run(void) {
        typename synch_policy::scoped_lock lock(mutex_);
        typename std::deque<async_task_base *>::iterator iter;
        for (iter = tasks_.begin(); iter != tasks_.end(); iter++) {
          (*iter)->execute();
          (*iter)->destroy();
        }
        tasks_.clear();
      }
    };

  }
}

#endif

