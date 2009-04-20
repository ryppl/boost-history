//
// boost/join/join_base.hpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
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

namespace boost {
  namespace join {
    namespace detail {

      class port;

      class joint_base {
      public:
        typedef boost::function0<void> callable;
        const char * name_;
        logger log;
        boost::mutex mutex_; // protects joint global status including ports and chords
        joint_base(const char *n=0) : name_(n), log(n) {}
        virtual ~joint_base() {}
        virtual void reset() = 0;
        virtual bool check_heartbeat() = 0;
        virtual bool has_spawn(void) = 0;
        virtual void spawn(callable) = 0;
        virtual bool port_invoked(int ind) = 0;
        virtual void port_revoked(int ind) = 0;
        virtual void port_remove(int ind) = 0;
      };

      template <typename R>
      class chord_base {
      public:
        virtual void capture_arguments(boost::function0<R> &cb) = 0;
        virtual ~chord_base() {}
      };      

      class port {
      public:
        enum port_type {
          async,
          synch
        } ;
        port_type type_;
        boost::shared_ptr<joint_base> joint_; //pointer to owning joint
        int index_;  //my index in joint
        unsigned int num_pending_; //pending calls/msgs at this port
 
        port(port_type t, boost::shared_ptr<joint_base> j) : 
          type_(t), joint_(j), index_(-1), num_pending_(0) {
        }

        virtual ~port() {
          detach();
        }

        //called from client, detach port from joint
        void detach() {
          if (joint_ != 0) {
            joint_->log.stream() << "a port is being destructed..." << joint_.use_count() << logger::endl;
            joint_->port_remove(index_);
            joint_.reset();
          }
        }

        //when detached from joint, need reset
        //caled from joint destructor
        virtual void reset(void) {
          if (joint_ != 0)
            joint_->log.msg("a port is reset ...");
          index_ = -1;
          num_pending_ = 0;
        }

        //called from async/synch interface, need hold a shared_ptr to joint so its destructor will not be called right away
        void reset_joint() {
          boost::shared_ptr<joint_base> my_joint(joint_); 
          if (my_joint != 0)
            my_joint->reset();
        }

        //detach port from joint, only can be done by chord_remove
        //when all its chords are removed, a port is reset & detached
        //called from joint
        void detach_joint(void) {
          if (joint_ !=0) {
            joint_->log.stream() << "a port is detached ... joint remaining count: " << joint_.use_count()-1 << logger::endl;
            joint_.reset();
          }
        }

        bool test_chord_fire(void) {
          return joint_->port_invoked(index_);
        }

        void port_revoked(void) {
          joint_->port_revoked(index_);
        }
      };

    }
  }
}

#endif
