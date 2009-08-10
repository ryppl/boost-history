//
// thread_pool_executor.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef THREAD_POOL_EXECUTOR_HPP
#define THREAD_POOL_EXECUTOR_HPP

#include <deque>
#include <algorithm>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/channel/executors/executor_base.hpp>

namespace boost {
  namespace channel {

    //a dedicated pool of threads to exec async tasks
    //a simple thread pool executor with unlimited buffering
    template<typename synch_policy>
    class thread_pool_executor {
    public:
      enum state {
        running, stop
      };

      thread_pool_executor(int num_thr) :
        num_thr_(num_thr) {
        st_ = running;
        for (int i = 0; i < num_thr; i++)
          threads_.create_thread(
                                 boost::bind(&thread_pool_executor::run, this));
      }

      ~thread_pool_executor() {
        typename synch_policy::scoped_lock lock(mutex_);
        if (st_ == running) {
          lock.unlock();
          shut_down_wait();
        }
      }

      ///executor life cycle
      state get_state(void) {
        typename synch_policy::scoped_lock lock(mutex_);
        return st_;
      }

      //gracefully shut down
      void shut_down_wait() {
        typename synch_policy::scoped_lock lock(mutex_);
        if (st_ == running) {
          st_ = stop;
          lock.unlock();
          cond_.notify_all();
          //waiting for the threads to exit
          threads_.join_all();
        }
      }

      void wait() {
        typename synch_policy::scoped_lock lock(mutex_);
        while (st_ == running && !tasks_.empty()) {
          cond_wait_.wait(lock);
        }
      }

      //abruptively shut down
      void shut_down_now() {
        typename synch_policy::scoped_lock lock(mutex_);
        st_ = stop;
        //kill threads
      }

      ///submit a task
      template<typename task_type>
      async_task_base * execute(task_type task) {
        typename synch_policy::scoped_lock lock(mutex_);
        if (st_ == running) {
          async_task_base * t = new async_task<task_type> (task);
          tasks_.push_back(t);
          cond_.notify_one();
          return t;
        }
        return NULL;
      }

      ///scancel a task
      bool cancel(async_task_base *task) {
        typename synch_policy::scoped_lock lock(mutex_);
        if (st_ == running) {
          typename std::deque<async_task_base *>::iterator iter;
          iter = std::find(tasks_.begin(), tasks_.end(), task);
          if (iter != tasks_.end()) {
            (*iter)->destroy();
            tasks_.erase(iter);
            return true;
          }
        }
        return false;
      }

    private:
      async_task_base * next_task(void) {
        async_task_base * t = NULL;
        typename synch_policy::scoped_lock lock(mutex_);
        while (tasks_.empty() && st_ != stop) {
          cond_wait_.notify_one();
          cond_.wait(lock);
        }
        if (st_ == stop)
          return NULL;
        t = tasks_.front();
        tasks_.pop_front();
        return t;
      }

      void run(void) {
        while (st_ != stop) {
          async_task_base * t = next_task();
          if (st_ != stop && t != NULL)
            t->execute();
        }
      }

    private:
      std::deque<async_task_base *> tasks_;
      boost::thread_group threads_;
      typename synch_policy::mutex mutex_;
      typename synch_policy::condition cond_;
      typename synch_policy::condition cond_wait_;
      state st_;
      int num_thr_;
    };

  }
}

#endif

