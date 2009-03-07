//
// boost/join/actor.hpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_JOIN_BASE_HPP
#define BOOST_JOIN_BASE_HPP

#include <string>
#include <vector>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/utility.hpp>
#include <boost/join/base/utils.hpp>
#include <boost/join/base/exceptions.hpp>
#include <boost/array.hpp>


namespace boost {
  namespace join {

    class chord_base;
    class port;

    class actor_base {
    public:
      typedef boost::function0<void> callable;
      const char * name_;
      logger log;
      boost::mutex mutex_; // protects actor global status including ports and chords
      actor_base(const char *n=0) : name_(n), log(n) {}
      virtual ~actor_base() {}
      virtual void reset() = 0;
      virtual bool check_heartbeat() = 0;
      virtual void spawn(callable c) = 0;
      virtual bool has_spawn(void) = 0;
      virtual boost::function1<void, callable> get_spawn() = 0;
      virtual chord_base * port_invoked(int ind) = 0;
      virtual void port_revoked(int ind) = 0;
    };

    class chord_base {
    public:
      virtual ~chord_base() {}
    };      

    template <typename R>
    class chord_oper {
    public:
      virtual void capture_arguments(boost::function0<R> &cb) = 0;
      virtual ~chord_oper() {}
    };      

    class port {
    public:
      enum port_type {
        async,
        synch
      } ;
      port_type type_;
      actor_base * actor_; //pointer to owning actor
      int index_;  //my index in actor
      unsigned int num_pending_; //pending calls/msgs at this port
 
      port(port_type t=async) : 
        type_(t), actor_(0), index_(-1), num_pending_(0) {
      }
      //ports should be only destroyed after all its chords have been destroyed
      //otherwise delete a port will delete the whole actor
      virtual ~port() {
        if (actor_ != 0)
          actor_->reset();
      }

      //when detached from actor, need reset
      //caled from actor destructor
      virtual void reset(void) {
        actor_ = 0;
        index_ = -1;
        num_pending_ = 0;
      }

      virtual void PUT(void) {}

      chord_base * test_chord_fire(void) {
        return actor_->port_invoked(index_);
      }

      void port_revoked(void) {
        actor_->port_revoked(index_);
      }
    };

  }
}

#endif
