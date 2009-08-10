//
// threadpool_executor.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef THREADPOOL_EXECUTOR_HPP
#define THREADPOOL_EXECUTOR_HPP

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/threadpool/threadpool.hpp>

namespace boost {
  namespace channel {

    //a wrapper over Philipp Henkel's threadpool library
    template<typename pool_type = boost::threadpool::pool<boost::function0<void>,
                                     boost::threadpool::fifo_scheduler<boost::function0<void> > > >
    class threadpool_executor {
    public:
      threadpool_executor(int num_thr) :
        num_thr_(num_thr) {
        pool_ = pool_type::create_pool(num_thr_);
      }

      ~threadpool_executor() {
        shut_down_wait();
      }

      //gracefully shut down
      void shut_down_wait() {
        pool_->clear();
        pool_->resize(0);
        pool_->wait();
      }

      //gracefully shut down
      void wait() {
        pool_->wait();
      }

      //abruptively shut down
      void shut_down_now() {
        //how ?
      }

      ///submit a task
      template<typename task_type>
      bool execute(task_type task) {
        return pool_->schedule(task);
      }

      ///cancel a task.
      ///will implement it on top of "future" when it becomes available soon
      bool cancel(async_task_base *task) {
        return true;
      }

    private:
      int num_thr_;
      boost::shared_ptr<pool_type> pool_;
    };

}
}

#endif

