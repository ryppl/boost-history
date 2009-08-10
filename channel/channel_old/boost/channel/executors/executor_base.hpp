//
// executor_base.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXECUTOR_BASE_HPP
#define EXECUTOR_BASE_HPP

namespace boost {
  namespace channel {

    //async tasks: to be later executed in executor:
    //1. although in same thread, not in orginal calling context
    //2. in diff thread
    class async_task_base {
    public:
      enum state {
        waiting, running, completed
      };

      // Perform the task.
      void execute() {
        st_ = running;
        exec_func_(this);
        st_ = completed;
      }

      // Destroy the task.
      void destroy() {
        destroy_func_(this);
      }

    protected:
      typedef void (*exec_func_type)(async_task_base*);
      typedef void (*destroy_func_type)(async_task_base*);

      // Construct an task
      async_task_base(exec_func_type exec_func, destroy_func_type destroy_func) :
        exec_func_(exec_func), destroy_func_(destroy_func), st_(waiting) {
      }

      // Prevent deletion through this type.
      ~async_task_base() {
      }

    private:
      exec_func_type exec_func_;
      destroy_func_type destroy_func_;
      state st_;
    };

    //
    template<typename task_type>
    class async_task: public async_task_base {
    public:
      // Constructor.
      async_task(task_type task) :
        async_task_base(&async_task<task_type>::exec_task, &async_task<
                        task_type>::destroy_task), task_(task) {
      }

      // Invoke the task.
      static void exec_task(async_task_base* base) {
        static_cast<async_task<task_type>*> (base)->task_();
      }

      // Delete the task.
      static void destroy_task(async_task_base* base) {
        delete static_cast<async_task<task_type>*> (base);
      }

    private:
      task_type task_;
    };

    //define an abstract class, so that it cannt be
    //instantiated and execution must be in-line;
    //or it can provide a base class for dynamic
    //polymorphism of executors;
    class abstract_executor {
    public:
      template<typename task_type>
      async_task_base * execute(task_type task) {
        return NULL;
      }
      virtual bool cancel(async_task_base *task) = 0;
      virtual ~abstract_executor() {
      }
    };

  }
}

#endif

