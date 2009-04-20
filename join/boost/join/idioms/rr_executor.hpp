//
// boost/join/rr_executor.hpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_RR_EXECUTOR_HPP
#define BOOST_JOIN_RR_EXECUTOR_HPP

#include <iostream>
#include <map>
#include <boost/thread.hpp>
#include <boost/join/join.hpp>

namespace boost {
  namespace join {

    template <template <size_t> class scheduler=sched_pri_round_robin, size_t sz=32>
    class rr_executor_t : public joint_t<scheduler, sz> {
    public:
      typedef scheduler<sz> sched_type;
      typedef joint_t<scheduler, sz> joint_type;
      typedef typename joint_type::callable task;
      typedef std::map<size_t, async<task> >  que_map_type;

      //static api: one default task queue
      async<task> execute;
      synch<void,void> shutdown;

      void spawn(task t) { execute(t); }

      //dynamic api: dynamically added task queues
      async<task> task_queue(size_t i=0) {
        //since rr_executor itself uses async/synch_p<> methods, we can have task
        //queues [1-(sz-5)]
        if (i>(sz-5)) i = i%(sz-5); 
        if(i==0) return execute;
        if(this->my_scheduler() != schedule_round_robin) {
          return execute;
        }
        typename que_map_type::iterator iter;
        if ((iter=que_map_.find(i)) != que_map_.end())
          return iter->second;
        else {
          this->log->msg(" creating task_queue ......");
          async<task> nq;
          chord(run, nq, &rr_executor_t::exec_cb);
          que_map_[i] = nq;
          return nq;        
        }
      }

      rr_executor_t(int num, const char *name = 0) : 
        joint_type(0, 0, name) {
        chord(run, execute, &rr_executor_t::exec_cb);
        chord(run, stopped, &rr_executor_t::stop_cb, 1);
        chord(shutdown, started, &rr_executor_t::shutdown_cb, 1);
        chord(shutdown, stopped, &rr_executor_t::stopped_cb, 1);
        for(int i=0; i<num; i++)
          threads_.create_thread(boost::bind(&rr_executor_t::main_loop, this));
        started(); //init state
      }
      ~rr_executor_t() {
        shutdown();
      }

    private:
      synch<bool,void> run;
      //rr_executor states
      async<void> started;
      async<void> stopped;
      boost::thread_group threads_;
      //dynamic api: dynamically added task queues
      que_map_type que_map_;

      void main_loop(void) {
        this->log->msg("a thread starts...");
        while(run()) {}
        this->log->msg("a thread exits...");
      }
      bool exec_cb(void_t run, task tasklet) {
        this->log->msg("start one task...");
        try {
          tasklet();
        }
        catch (join_exception &je) {
          this->log->msg(je.what());
        }
        catch (...) {
          this->log->msg("UNKNOWN exceptions happen inside a rr_executor thread, ignore.");
        }
        this->log->msg("finish one task...");
        return true; //worker thread continue
      }
      bool stop_cb(void_t run, void_t stopd) {
        stopped(); 
        return false; //worker thread exit
      }
      void shutdown_cb(void_t shdn, void_t started) {
        this->log->msg("shutdown...");
        stopped();
        //waiting for the threads to exit
        this->log->msg("wait...");
        threads_.join_all();    
        this->log->msg("all threads exit, done...");
      }
      void stopped_cb(void_t shdn, void_t stopd) {
        this->log->msg("stopped...");
        stopped();
      }
    };

    //define a default rr_executor_t taking 32 async / synch methods or at most 27 task queues
    class rr_executor : public rr_executor_t<> {
    public:
      rr_executor(int num, const char *name = 0) : 
        rr_executor_t<>(num, name)
      {
      }
    };

  }
}

#endif
