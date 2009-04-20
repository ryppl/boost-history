//
// Boost/join/port.hpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
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
#include <boost/thread/condition.hpp> 
#include <boost/join/base/join_base.hpp>
#include <boost/join/base/exceptions.hpp>

namespace boost {
  namespace join {

    struct void_t {};

    namespace detail {

      class joint_base;
      template <typename ResT> class chord_base;

      class async_port : public port {
      public:
        async_port(shared_ptr<joint_base> j) : port(port::async, j), chord_fired_(0) {}
        void add_chord_fired(chord_base<void> *c) { chord_fired_ = c; }
      protected:
        chord_base<void> *chord_fired_;
      };

      //QueT supports push_back()/pop_front()/front()
      template <typename ArgT, typename QueT> 
      class async_impl1 : public async_port, private boost::noncopyable {
      public:
        typedef ArgT argument_type;
        typedef void result_type;

        async_impl1(size_t sz, shared_ptr<joint_base> j) : async_port(j), max_size_(sz), arg1_(0) {//sz==0 means unlimited
        } 

        void operator()(ArgT t) {
          boost::shared_ptr<joint_base> my_joint(joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          my_joint->log.msg("async_impl1::operator(arg) enter");
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          if(test_chord_fire()) {  //notify joint about a new msg
            arg1_ = &t; //setup my arg
            boost::function0<void> cb;
            chord_fired_->capture_arguments(cb);
            chord_fired_ = 0;
            my_joint->log.msg("a new async task spawned");
            my_joint->check_heartbeat();
            lock.unlock();
            my_joint->spawn(cb); //"spawn" (from executor) has its own lock protection
            return;
          }
          else {
            my_joint->log.msg("async_impl1::operator(arg) arg queued");
            arg_.push_back(t);
          }
        }
        void put(ArgT t) {
          operator()(t);
        }
        ArgT &top(void) {
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
        void reset(void) {
          port::reset();
          arg_.clear();
          arg1_ = 0;
        }
      private:
        size_t max_size_;
        QueT arg_;
        ArgT *arg1_;
      };

      class async_impl0 : public async_port, private boost::noncopyable {
      public:
        typedef void_t argument_type;
        typedef void result_type;

        async_impl0(size_t sz, shared_ptr<joint_base> j) : async_port(j), max_size_(sz) {
        } 

        void operator()(void) {
          boost::shared_ptr<joint_base> my_joint(joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          my_joint->log.msg("async_impl0::operator(void) enter");
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();
        
          if(test_chord_fire()) {  
            boost::function0<void> cb;
            chord_fired_->capture_arguments(cb);
            chord_fired_ = 0;
            my_joint->log.msg("a new async task spawned");
            my_joint->check_heartbeat();
            lock.unlock();
            my_joint->spawn(cb);
            return;
          }
          my_joint->log.msg("async_impl0::operator(void) exit");
        }
        void put(void) {
          operator()();
        }
        void pop_top(void) {
        }
        void_t top(void) {
          return void_t();
        }

      private:
        size_t max_size_;
      };

      template <typename ResT>
      class synch_port : public port {
      public:
        synch_port(shared_ptr<joint_base> j) : port(port::synch, j), num_waiting_(0), reset_(false), chord_fired_(0) {}

        void add_chord_fired(chord_base<ResT>* c) {
          if (chord_fired_ == 0)
            chord_fired_ = c;
          else
            chords_fired_.push_back(c);
        }

        //async call will call this to transfer control
        void transfer(chord_base<ResT>* c) {
          add_chord_fired(c);
          if (num_waiting_ > 0)
            cond_.notify_one();     
          joint_->log.msg("transfer");
        }
        chord_base<ResT> * get_chord_fired(void) {
          chord_base<ResT> *cd = 0;
          if (chord_fired_ != 0) {
            cd = chord_fired_;
            chord_fired_ = 0;
          }
          else {
            if (!chords_fired_.empty()) { //assigned to complete chord callback
              cd = chords_fired_.front();
              chords_fired_.pop_front();
            } 
          }
          return cd;
        }

        void wait_fired(boost::mutex::scoped_lock &lock, chord_base<ResT> *&chd) {
          num_waiting_++;
          cond_.wait(lock);
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          chd = get_chord_fired();
        }

        void timed_wait_fired(boost::mutex::scoped_lock &lock, chord_base<ResT> *&chd, const boost::xtime& timeout) {
          num_waiting_++;
          if(!cond_.timed_wait(lock, timeout) && ((chd = get_chord_fired()) == 0)) {
            port_revoked();
            num_waiting_--;
            throw synch_time_out_exception();
          }
          num_waiting_--;
          if (reset_) {
            if (num_waiting_ == 0)
              reset_ = false;
            throw port_chord_reset_exception();
          }
          if (chd == 0)
            chd = get_chord_fired();
        }

        void reset(void) {
          port::reset();
          chord_fired_ = 0;
          chords_fired_.clear();
          reset_ = true;
          //tell all blocking threads to exit
          if (num_waiting_ > 0)
            cond_.notify_all();     
          //joint_->log.msg("reset");
        }

      protected:
        boost::condition cond_;
        size_t num_waiting_;
        bool reset_;
        chord_base<ResT> *chord_fired_;
        std::deque<chord_base<ResT>*> chords_fired_;
      };

      template <typename ResT, typename ArgT>
      class synch_impl : public synch_port<ResT>, private boost::noncopyable {
      public:
        typedef ArgT argument_type;
        typedef ResT result_type;

        synch_impl(size_t sz, shared_ptr<joint_base> j) :  synch_port<ResT>(j), max_size_(sz), arg_(0) {//sz==0 means unlimited
        } 

        ResT operator()(ArgT t) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<ResT, ArgT>::operator(t) enter");

          chord_base<ResT> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<resT,argT> wait");
            this->wait_fired(lock, chd);
          }
          my_joint->log.msg("synch<resT,argT> fired");
          arg_ = &t; //setup my arg
          boost::function0<ResT> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          return cb();  //invoke callback in my thread
        }

        ResT put(ArgT t) {
          return operator()(t);
        }

        ResT operator()(ArgT t, const boost::xtime& timeout) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<ResT, ArgT>::operator(t, timeout) enter");

          chord_base<ResT> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<resT,argT> timed wait");
            this->timed_wait_fired(lock, chd, timeout);
          }
          my_joint->log.msg("synch<resT,argT> fired");
          arg_ = &t; //setup my arg
          boost::function0<ResT> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          return cb();  //invoke callback in my thread
        }

        ResT put(ArgT t, const boost::xtime& w) {
          return operator()(t, w);
        }
  
        ArgT &top(void) {
          return *arg_;
        }
        void pop_top(void) {
        }
      private:
        size_t max_size_;
        ArgT *arg_;
      };


      template <typename ArgT>
      class synch_impl<void, ArgT> : public synch_port<void>, private boost::noncopyable {
      public:
        typedef ArgT argument_type;
        typedef void result_type;

        synch_impl(size_t sz, shared_ptr<joint_base> j) : synch_port<void>(j), max_size_(sz), arg_(0) {//sz==0 means unlimited
        } 

        void operator()(ArgT t) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<void, ArgT>::operator(t) enter");

          chord_base<void> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<void,argT> wait");
            this->wait_fired(lock, chd);
          }
          my_joint->log.msg("synch<void, argT> fired");
          arg_ = &t; //setup my arg
          boost::function0<void> cb;
          chd->capture_arguments(cb);
          my_joint->log.msg("synch<void, argT> fired-bef-callback");
          my_joint->check_heartbeat();
          lock.unlock();
          cb();  //invoke callback in my thread
        }

        void put(ArgT t) {
          operator()(t);
        }

        void operator()(ArgT t, const boost::xtime& timeout) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<void, ArgT>::operator(t, timeout) enter");

          chord_base<void> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<void,argT> timed wait");
            this->timed_wait_fired(lock, chd, timeout);
          }
          my_joint->log.msg("synch<void, argT> fired");
          arg_ = &t; //setup my arg
          boost::function0<void> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          cb();  //invoke callback in my thread
        }

        void put(ArgT t, const boost::xtime& w) {
          operator()(t, w);
        }
    
        ArgT &top(void) {
          return *arg_;
        }
        void pop_top(void) {
        }
      private:
        size_t max_size_;
        ArgT *arg_;
      };

      template <typename ResT>
      class synch_impl<ResT,void> : public synch_port<ResT>, private boost::noncopyable {
      public:
        typedef void_t argument_type;
        typedef ResT result_type;

        synch_impl(size_t sz, shared_ptr<joint_base> j) : synch_port<ResT>(j), max_size_(sz) {} 

        ResT operator()(void) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();
        
          my_joint->log.msg("synch<ResT,void>::operator(void) enter");

          chord_base<ResT> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<resT,void> wait");
            this->wait_fired(lock, chd);
          }
          my_joint->log.msg("synch<ResT,void> fired");
          boost::function0<ResT> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          return cb();  //invoke callback in my thread
        }

        ResT put(void) {
          return operator()();
        }

        ResT operator()(const boost::xtime& timeout) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<ResT,void>::operator(timeout) enter");

          chord_base<ResT> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<resT,void> timed wait");
            this->timed_wait_fired(lock, chd, timeout);
          }
          my_joint->log.msg("synch<ResT,void> fired");
          boost::function0<ResT> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          return cb();  //invoke callback in my thread
        }

        ResT put(const boost::xtime& w) {
          return operator()(w);
        }

        void pop_top(void) {
        }

        void_t top(void) {
          return void_t();
        }
      private:
        size_t max_size_;
      };


      template <>
      class synch_impl<void, void> : public synch_port<void>, private boost::noncopyable {
      public:
        typedef void_t argument_type;
        typedef void result_type;

        synch_impl(size_t sz, shared_ptr<joint_base> j) : synch_port<void>(j), max_size_(sz){} 

        void operator()(void) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<void,void>::operator(void) enter");

          chord_base<void> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<void,void> wait");
            this->wait_fired(lock, chd);
          }
          my_joint->log.msg("synch<void,void> fired");
          boost::function0<void> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          cb();  //invoke callback in my thread
        }

        void put(void) {
          operator()();
        }

        void operator()(const boost::xtime& timeout) {
          boost::shared_ptr<joint_base> my_joint(this->joint_); 
          if(my_joint == 0) 
            throw not_in_chord_exception();
          boost::mutex::scoped_lock lock(my_joint->mutex_);
          if (max_size_> 0 && this->num_pending_ >= max_size_)
            throw queue_overflow_exception();

          my_joint->log.msg("synch<void,void>::operator(timeout) enter");

          chord_base<void> *chd = 0;
          if(this->test_chord_fire()) chd = this->get_chord_fired();
          while (chd == 0) {
            //no chord ready to fire 
            //block wait here till awaken by another msg
            my_joint->log.msg("synch<void,void> timed wait");
            this->timed_wait_fired(lock, chd, timeout);
          }
          my_joint->log.msg("synch<void,void> fired");
          boost::function0<void> cb;
          chd->capture_arguments(cb);
          my_joint->check_heartbeat();
          lock.unlock();
          cb();  //invoke callback in my thread
        }

        void put(const boost::xtime& w) {
          operator()(w);
        }
  
        void pop_top(void) {
        }

        void_t top(void) {
          return void_t();
        }

      private:
        size_t max_size_;
      };

    }

    //------ Pimpl wrappers ------

    template <typename ArgT, typename QueT=std::deque<ArgT> > 
    class async {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::async_impl1<ArgT, QueT> ImplT;
      typedef typename ImplT::argument_type argument_type;
      typedef typename ImplT::result_type result_type;
      async(size_t sz=0) : max_size_(sz) {}
      void put(ArgT t) {
        if (pimpl_ == 0) throw not_in_chord_exception();
        pimpl_->put(t);
      }
      void operator()(ArgT t) { put(t); }
      void detach() { 
        if (!pimpl_)
          pimpl_->detach();
      }
      void reset_joint() {
        if (pimpl_ != 0)
          pimpl_->reset_joint();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };

    template <> 
    class async<void> {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::async_impl0 ImplT;
      typedef ImplT::argument_type argument_type;
      typedef ImplT::result_type result_type;
      async(size_t sz=0) : max_size_(sz) {}  
      void put(void) {
        if (!pimpl_) throw not_in_chord_exception();
        pimpl_->put();
      }
      void operator()(void) { put(); }
      void detach() { 
        if (pimpl_ != 0)
          pimpl_->detach();
      }
      void reset_joint() {
        if (pimpl_ != 0)
          pimpl_->reset_joint();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };

    template <typename ResT, typename ArgT>
    class synch {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::synch_impl<ResT, ArgT> ImplT;
      typedef typename ImplT::argument_type argument_type;
      typedef typename ImplT::result_type result_type;
      synch(size_t sz=0) : max_size_(sz) {}
      ResT put(ArgT t) {
        if (pimpl_ == 0) throw not_in_chord_exception();
        return pimpl_->put(t);
      }
      ResT operator()(ArgT t) { return put(t); }
      void detach() { 
        if (pimpl_ != 0)
          pimpl_->detach();
      }
      void reset_joint() {
        if (pimpl_ != 0)
          pimpl_->reset_joint();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };

    template <typename ResT>
    class synch<ResT,void> {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::synch_impl<ResT, void> ImplT;
      typedef typename ImplT::argument_type argument_type;
      typedef typename ImplT::result_type result_type;
      synch(size_t sz=0) : max_size_(sz) {}
      ResT put(void) {
        if (pimpl_ == 0) throw not_in_chord_exception();
        return pimpl_->put();
      }
      ResT operator()(void) { return put(); }
      void detach() { 
        if (pimpl_ != 0)
          pimpl_->detach();
      }
      void reset_joint() {
        if (pimpl_ != 0)
          pimpl_->reset_joint();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };
      
    template <typename ArgT>
    class synch<void,ArgT> {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::synch_impl<void, ArgT> ImplT;
      typedef typename ImplT::argument_type argument_type;
      typedef typename ImplT::result_type result_type;
      synch(size_t sz=0) : max_size_(sz) {}
      void put(ArgT t) {
        if (pimpl_ == 0) throw not_in_chord_exception();
        pimpl_->put(t);
      }
      void operator()(ArgT t) { put(t); }
      void detach() { 
        if (pimpl_ != 0)
          pimpl_->detach();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };

    template <>
    class synch<void,void> {
      template <template <size_t> class, size_t> friend class joint_t;
    public:
      typedef detail::synch_impl<void, void> ImplT;
      typedef ImplT::argument_type argument_type;
      typedef ImplT::result_type result_type;
      synch(size_t sz=0) : max_size_(sz) {}
      void put() {
        if (pimpl_ == 0) throw not_in_chord_exception();
        pimpl_->put();
      }
      void operator()() { put(); }
      void detach() { 
        if (pimpl_ != 0)
          pimpl_->detach();
      }
      void reset_joint() {
        if (pimpl_ != 0)
          pimpl_->reset_joint();
      }
    private:
      size_t max_size_;
      boost::shared_ptr<ImplT> pimpl_;
    };

  }
}

#endif
