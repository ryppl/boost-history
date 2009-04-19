//
// boost/join/port.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_PORT_HPP
#define BOOST_JOIN_PORT_HPP

#include <exception>
#include <stdexcept>
#include <deque>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/thread/condition.hpp>     //for thread backward compatibility
#include <boost/join/base/join_base.hpp>
#include <boost/join/base/exceptions.hpp>

namespace boost {
  namespace join {

    //common interface of async/synch ports to actor(owner)
    //
    class actor_base;
    class chord_base;
    template <typename ArgT> class async_v;
    template <typename ResT, typename ArgT> class synch_v;
    template <typename ArgT> class async_p;
    template <typename ResT, typename ArgT> class synch_p;

    template <> 
    class async_v<void> {
    public:
      ///hacks for compile
      port *port_;  
      async_v(port *p) : port_(p) {}
      void operator()(void) {
        port_->PUT();
      }
    };

    template <> 
    class synch_v<void, void> {
    public:
      ///hacks for compile
      port *port_;  
      synch_v(port *p) : port_(p) {}
      void operator()(void) {
        port_->PUT();
      }
    };


    template <typename ArgT> 
    class async_p : public port, private boost::noncopyable {
    public:
      typedef ArgT argument_type;
      typedef void result_type;
      typedef ArgT arg1_type;
      typedef ArgT args_type;
      typedef async_v<ArgT> var_type;

      async_p(size_t sz=0) : port(port::async), max_size_(sz), arg1_(0) {//sz==0 means unlimited
      } 

      void operator()(ArgT t) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        actor_->log.msg("async_p::operator(arg) enter");
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        chord_base *chd = 0;
        if((chd = test_chord_fire()) != 0) {  //notify actor about a new msg
          arg1_ = &t; //setup my arg
          boost::function0<void> cb;
          dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
          actor_->log.msg("a new async task spawned");
          boost::function1<void, actor_base::callable> spawn = actor_->get_spawn(); //spawn callback in executor
          actor_->check_heartbeat();
          //if check_heartbeat succeed, actor_ here should be reset to null
          lock.unlock();
          spawn(cb);
          return;
        }
        else {
          arg_.push_back(t);
          actor_->log.msg("async_p::operator(arg) exit");
        }
      }
      void put(ArgT t) {
        operator()(t);
      }
      ArgT &get(void) {
        if(arg1_ != 0)
          return *arg1_;
        else
          return arg_.front();
      }
      void pop_top(void) {
        if (arg1_ != 0)
          arg1_ = 0;
        else
          arg_.pop_front();
      }
      var_type top(void) {
        return var_type(this);
      }
      void reset(void) {
        port::reset();
        arg_.clear();
        arg1_ = 0;
      }
    private:
      size_t max_size_;
      std::deque<ArgT> arg_;
      ArgT *arg1_;
    };

    template <> 
    class async_p<void> : public port, private boost::noncopyable {
    public:
      typedef void argument_type;
      typedef void result_type;
      typedef async_v<void> var_type;

      async_p(size_t sz=0) : port(port::async), max_size_(sz) {
      } 

      void operator()(void) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        actor_->log.msg("async_p::operator(void) enter");
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();
        
        chord_base *chd = 0;
        if((chd = test_chord_fire()) != 0) {  
          boost::function0<void> cb;
          dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
          actor_->log.msg("a new async task spawned");
          boost::function1<void, actor_base::callable> spawn = actor_->get_spawn(); //spawn callback in executor
          actor_->check_heartbeat();
          //if check_heartbeat succeed, actor_ here will be null
          lock.unlock();
          spawn(cb);
          return;
        }
        actor_->log.msg("async_p::operator(void) exit");
      }
      void put(void) {
        operator()();
      }
      void PUT(void) {
        operator()();
      }
      void pop_top(void) {
      }
      var_type top(void) {
        return var_type(this);
      }
    private:
      size_t max_size_;
    };

    class synch_port : public port {
    public:
      synch_port() : port(port::synch), num_waiting_(0), fired_(0), reset_(false) {}

      //async call will call this to transfer control
      void transfer(chord_base* c) {
        if (fired_ == 0)
          fired_ = c;
        else
          fired_chords_.push_back(c);
        if (num_waiting_ > 0)
          cond_.notify_one();     
        actor_->log.msg("transfer");
      }
      chord_base * get_fired_chord(void) {
        chord_base *cd = 0;
        if (fired_ != 0) {
          cd = fired_;
          fired_ = 0;
        }
        else {
          if (!fired_chords_.empty()) { //assigned to complete chord callback
            cd = fired_chords_.front();
            fired_chords_.pop_front();
          } 
        }
        return cd;
      }

      void reset(void) {
        port::reset();
        fired_ = 0;
        fired_chords_.clear();
        reset_ = true;
        //tell all blocking threads to exit
        if (num_waiting_ > 0)
          cond_.notify_all();     
        //actor_->log.msg("reset");
      }

    protected:
      boost::condition cond_;
      size_t num_waiting_;
      chord_base *fired_;
      bool reset_;
      std::deque<chord_base*> fired_chords_;
    };

    template <typename ResT, typename ArgT>
    class synch_p : public synch_port, private boost::noncopyable {
    public:
      typedef ArgT argument_type;
      typedef ResT result_type;
      typedef synch_v<ResT, ArgT> var_type;

      synch_p(size_t sz=0) :  max_size_(sz), arg_(0) {//sz==0 means unlimited
      } 

      ResT operator()(ArgT t) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<ResT, ArgT>::operator(t) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<resT,argT> wait");
          num_waiting_++;
          cond_.wait(lock);
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          chd = get_fired_chord();
        }
        actor_->log.msg("synch<resT,argT> fired");
        arg_ = &t; //setup my arg
        boost::function0<ResT> cb;
        dynamic_cast<chord_oper<ResT> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //should not refer to actor_ after here, since check_hearbeat may reset it
        lock.unlock();
        return cb();  //invoke callback in my thread
      }

      ResT put(ArgT t) {
        return operator()(t);
      }

      ResT operator()(ArgT t, const boost::xtime& timeout) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<ResT, ArgT>::operator(t, timeout) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<resT,argT> wait");
          num_waiting_++;
          if(!cond_.timed_wait(lock, timeout) && ((chd = get_fired_chord()) == 0)) {
            port_revoked();
            num_waiting_--;
            actor_->log.msg("synch<resT,argT> timeout");
            throw synch_time_out_exception();
          }
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          if (chd == 0)
            chd = get_fired_chord();
        }
        actor_->log.msg("synch<resT,argT> fired");
        arg_ = &t; //setup my arg
        boost::function0<ResT> cb;
        dynamic_cast<chord_oper<ResT> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //should not refer to actor_ after here, since check_hearbeat may reset it
        lock.unlock();
        return cb();  //invoke callback in my thread
      }

      ResT put(ArgT t, const boost::xtime& w) {
        return operator()(t, w);
      }
  
      ArgT &get(void) {
        return *arg_;
      }
      void pop_top(void) {
      }
      var_type top(void) {
        return var_type(this);
      }

    private:
      size_t max_size_;
      ArgT *arg_;
    };


    template <typename ArgT>
    class synch_p<void, ArgT> : public synch_port, private boost::noncopyable {
    public:
      typedef ArgT argument_type;
      typedef void result_type;
      typedef synch_v<void, ArgT> var_type;

      synch_p(size_t sz=0) : max_size_(sz), arg_(0) {//sz==0 means unlimited
      } 

      void operator()(ArgT t) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<void, ArgT>::operator(t) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<void,argT> wait");
          num_waiting_++;
          cond_.wait(lock);
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          chd = get_fired_chord();
        }
        actor_->log.msg("synch<void, argT> fired");
        arg_ = &t; //setup my arg
        boost::function0<void> cb;
        dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        cb();  //invoke callback in my thread
        return;
      }

      void put(ArgT t) {
        operator()(t);
      }

      void operator()(ArgT t, const boost::xtime& timeout) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<void, ArgT>::operator(t, timeout) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<void,argT> wait");
          num_waiting_++;
          if(!cond_.timed_wait(lock, timeout) && ((chd = get_fired_chord()) == 0)) {
            port_revoked();
            num_waiting_--;
            actor_->log.msg("synch<void,argT> timeout");
            throw synch_time_out_exception();
          }
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          if (chd == 0)
            chd = get_fired_chord();
        }
        actor_->log.msg("synch<void, argT> fired");
        arg_ = &t; //setup my arg
        boost::function0<void> cb;
        dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        cb();  //invoke callback in my thread
        return;
      }

      void put(ArgT t, const boost::xtime& w) {
        operator()(t, w);
      }
    
      ArgT &get(void) {
        return *arg_;
      }
      void pop_top(void) {
      }
      var_type top(void) {
        return var_type(this);
      }

    private:
      size_t max_size_;
      ArgT *arg_;
    };

    template <typename ResT>
    class synch_p<ResT,void> : public synch_port, private boost::noncopyable {
    public:
      typedef void argument_type;
      typedef ResT result_type;
      typedef synch_v<ResT, void> var_type;

      synch_p(size_t sz=0) : max_size_(sz) {} 

      ResT operator()(void) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();
        
        actor_->log.msg("synch_p<ResT,void>::operator(void) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<resT,void> wait");
          num_waiting_++;
          cond_.wait(lock);
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          chd = get_fired_chord();
        }
        actor_->log.msg("synch<ResT,void> fired");
        boost::function0<ResT> cb;
        dynamic_cast<chord_oper<ResT> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        return cb();  //invoke callback in my thread
      }

      ResT put(void) {
        return operator()();
      }

      ResT operator()(const boost::xtime& timeout) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<ResT,void>::operator(timeout) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<resT,void> wait");
          num_waiting_++;
          if(!cond_.timed_wait(lock, timeout) && ((chd = get_fired_chord()) == 0)) {
            port_revoked();
            num_waiting_--;
            actor_->log.msg("synch<resT,void> timeout");
            throw synch_time_out_exception();
          }
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          if (chd == 0)
            chd = get_fired_chord();
        }
        actor_->log.msg("synch<ResT,void> fired");
        boost::function0<ResT> cb;
        dynamic_cast<chord_oper<ResT> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        return cb();  //invoke callback in my thread
      }

      ResT put(const boost::xtime& w) {
        return operator()(w);
      }

      void pop_top(void) {
      }

      var_type top(void) {
        return var_type(this);
      }
    private:
      size_t max_size_;
    };


    template <>
    class synch_p<void, void> : public synch_port, private boost::noncopyable {
    public:
      typedef void argument_type;
      typedef void result_type;
      typedef synch_v<void, void> var_type;

      synch_p(size_t sz=0) : max_size_(sz){} 

      void operator()(void) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<void,void>::operator(void) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<void,void> wait");
          num_waiting_++;
          cond_.wait(lock);
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          chd = get_fired_chord();
        }
        actor_->log.msg("synch<void,void> fired");
        boost::function0<void> cb;
        dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        cb();  //invoke callback in my thread
        return;
      }

      void put(void) {
        operator()();
      }
      void PUT(void) {
        operator()();
      }

      void operator()(const boost::xtime& timeout) {
        if(actor_ == 0) 
          throw not_in_chord_exception();
        boost::mutex::scoped_lock lock(actor_->mutex_);
        if(actor_ == 0) 
          throw not_in_chord_exception();
        if (max_size_> 0 && num_pending_ >= max_size_)
          throw queue_overflow_exception();

        actor_->log.msg("synch_p<void,void>::operator(timeout) enter");

        chord_base *chd = test_chord_fire();
        while (chd == 0) {
          //no chord ready to fire 
          //block wait here till awaken by another msg
          actor_->log.msg("synch<void,void> wait");
          num_waiting_++;
          if(!cond_.timed_wait(lock, timeout)  && ((chd = get_fired_chord()) == 0)) {
            port_revoked();
            num_waiting_--;
            actor_->log.msg("synch<void,void> timeout");
            throw synch_time_out_exception();
          }
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          if (chd == 0)
            chd = get_fired_chord();
        }
        actor_->log.msg("synch<void,void> fired");
        boost::function0<void> cb;
        dynamic_cast<chord_oper<void> *>(chd)->capture_arguments(cb);
        actor_->check_heartbeat();
        //dont use actor after here
        lock.unlock();
        cb();  //invoke callback in my thread
        return;
      }

      void put(const boost::xtime& w) {
        operator()(w);
      }
  
      void pop_top(void) {
      }

      var_type top(void) {
        return var_type(this);
      }

    private:
      size_t max_size_;
    };


    template <typename ArgT> 
    class async_v {
    public:
      async_p<ArgT> *port_;
      ArgT arg1;
      async_v(async_p<ArgT> *p) :
        port_(p) {
        if (port_ != 0)
          arg1 = port_->get();
      }
      operator ArgT() {
        return arg1;
      }
      ArgT &arg(void) {
        return arg1;
      }
      void operator()(ArgT t) {
        port_->put(t);
      }
    };

    template <typename ResT, typename ArgT> 
    class synch_v {
    public:
      synch_p<ResT, ArgT> *port_;
      ArgT arg1;
      bool ret_flag_;
      synch_v(synch_p<ResT, ArgT> *p) :
        port_(p), ret_flag_(false) {
          arg1 = port_->get();
      }
      operator ArgT(void) {
        return arg1;
      }
      ArgT &arg(void) {
        return arg1;
      }
      ResT operator()(ArgT t) {
        return port_->put(t);
      }
    };

    template <typename ArgT> 
    class synch_v<void, ArgT> {
    public:
      synch_p<void, ArgT> *port_;
      ArgT arg1;
      bool ret_flag_;
      synch_v(synch_p<void, ArgT> *p) :
        port_(p), ret_flag_(false) {
          arg1 = port_->get();
      }
      operator ArgT(void) {
        return arg1;
      }
      ArgT &arg(void) {
        return arg1;
      }
      void operator()(ArgT t) {
        port_->put(t);
      }
    };

    template <typename ResT> 
    class synch_v<ResT, void> {
    public:
      synch_p<ResT, void> *port_;
      bool ret_flag_;
      synch_v(synch_p<ResT, void> *p) :
        port_(p), ret_flag_(false) {}
      ResT operator()(void) {
        return port_->put();
      }
    };

  }
}

#endif
