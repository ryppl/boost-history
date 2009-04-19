//
// boost/join/executor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_SIMPLE_EXECUTOR_HPP
#define BOOST_JOIN_SIMPLE_EXECUTOR_HPP

#include <iostream>
#include <map>
#include <boost/thread.hpp>
#include <boost/join/join.hpp>

namespace boost {
  namespace join {

    template <template <size_t> class scheduler=sched_first_match, size_t sz=32>
    class executor_t : public actor_t<async_p<typename actor_base::callable>, scheduler, sz> {
    public:
      typedef scheduler<sz> sched_type;
      typedef actor_t<async_p<typename actor_base::callable>, scheduler, sz> actor_type;
      typedef typename actor_type::callable task;

      //api: one default task queue
      async_p<task> execute;
      synch_p<void,void> shutdown;

      executor_t(int num, const char *name = 0) : actor_type(0, 0, name) {
        chord(run, execute, &executor_t::exec_cb);
        chord(run, stopped, &executor_t::stop_cb);
        chord(shutdown, started, &executor_t::shutdown_cb);
        chord(shutdown, stopped, &executor_t::stopped_cb);
        num_threads_ = num;
        for(int i=0; i<num; i++)
          threads_.create_thread(boost::bind(&executor_t::main_loop, this));
        started(); //init state
      }
      ~executor_t() {
        shutdown();
      }

    private:
      synch_p<bool,void> run;
      //executor states
      async_p<void> started;
      async_p<void> stopped;
      //thread pool
      boost::thread_group threads_;
      int num_threads_;

      //entry func of each thread - a loop which exists when no tasks in execute queue and shutdown
      void main_loop(void) {
        this->log.msg("a thread starts...");
        while(run()) {}
        this->log.msg("a thread exits...");
      }
      bool exec_cb(synch_v<bool,void> run, async_v<task> exec) {
        this->log.msg("start one task...");
        try {
          (exec.arg1)();
        }
        catch (join_exception &je) {
          this->log.msg(je.what());
        }
        catch (...) {
          this->log.msg("UNKNOWN exceptions happen inside a executor thread, ignore.");
        }
        this->log.msg("finish one task...");
        return true; //worker thread continue
      }
      bool stop_cb(synch_v<bool,void> run, async_v<void> stopd) {
        return false; //worker thread exit
      }
      void shutdown_cb(synch_v<void,void> shdn, async_v<void> started) {
        this->log.msg("shutdown...");
        for(int i=0; i<num_threads_; i++) //stop all threads in pool
          stopped();
        //waiting for the threads to exit
        this->log.msg("wait...");
        threads_.join_all();    
        this->log.msg("all threads exit, done...");
        stopped(); //add one more stopped(), in case shutdown() is called more than once
      }
      void stopped_cb(synch_v<void,void> shdn, async_v<void> stopd) {
        this->log.msg("stopped...");
        stopped();
      }
    };

    //define a default executor_t 
    class executor : public executor_t<> {
    public:
      executor(int num, const char *name = 0) : 
        executor_t<>(num, name)
      {
      }
    };

  }
}

#endif
