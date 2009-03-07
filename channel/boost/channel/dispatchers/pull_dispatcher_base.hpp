//
// pull_dispatcher_base.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef PULL_DISPATCHER_BASE_HPP
#define PULL_DISPATCHER_BASE_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/name.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/dispatchers/dispatcher_base.hpp>

namespace boost {
  namespace channel {
    namespace detail {

      template<class , class , template<class , class , class > class> class pull_recver_base;

      //base pull_sender class, shared by most pull dispatchers
      template<typename name_space, typename platform,
               template<class , class , class > class msg_queue_type>
      struct pull_sender_base: public msg_queue_type<boost::shared_ptr<message<
                                                                         typename name_space::id_type> >, typename name_space::synch_policy,
                                                     platform> {
        typedef	typename name_space::id_type id_type;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef name<id_type,executor,synch_policy> name_type;
	typedef message<id_type> msg_type;
	typedef msg_queue_type<boost::shared_ptr<msg_type>,
                               synch_policy,
                               platform> que_type;
    typedef typename name_type::binding_set_type binding_set_type;
    typedef pull_recver_base<name_space, platform,msg_queue_type> recver_type;
    typedef named_in<name_space, recver_type> named_in_type;

    name_type * name_;
    executor *exec_;
    typename synch_policy::mutex claim_lock_;
    int num_claimed_;

    pull_sender_base(name_type *n, executor *e) : que_type(), name_(n),
                                                  exec_(e), num_claimed_(0) {}
    ~pull_sender_base() {}

    void notify(boost::shared_ptr<msg_type> msg) {
      //first store msg in que, only buffered msgs destined for local
      //msgs destined for remote will not be buffered, sent using always_first?
      if (name_->scope_ != name_base::scope_remote) {
        this->put(msg);
      }
      //notify recevers
      typename synch_policy::scoped_lock lock(this->name_->bind_lock_);
      binding_set_type &bindings = this->name_->bindings_;
      if (!bindings.empty()) {
        ///let all interested recvers know that a message is available
        for(typename binding_set_type::iterator iter = bindings.begin();
            iter != bindings.end(); iter++) {
          named_in_type *named_in = (named_in_type *)(*iter);
          recver_type *recver = (recver_type *)named_in;

          if (name_->scope_ == name_base::scope_remote || //for msgs sent to remote, treat it as always_first?
              named_in->type_ == named_in_type::member_local) {//for pull dispatchers, only send msgs to local receviers
            recver->notify(msg);
            if (name_->scope_ == name_base::scope_remote)
              return;
          }
        }
      }
    }

    //recvers will call this to retrv/pull data
    int pull(boost::shared_ptr<msg_type> & msg) {
      if (!this->empty()) {
        this->get(msg);
        return 1;
      }
      return 0;
    }

    //--- for 2 phase msg consumption protocol ---

    //to support 2-phase protocol at recver side
    bool claim_one(void) {
      typename synch_policy::scoped_lock lock(this->claim_lock_);
      if ((int)this->size() > num_claimed_) {
        num_claimed_++;
        return true;
      }
      return false;
    }

    void unclaim_one(void) {
      typename synch_policy::scoped_lock lock(this->claim_lock_);
      num_claimed_--;
      if (num_claimed_ < 0) num_claimed_ = 0;
    }
    //--- api interface ---
    //after sending, channel becomes owner
    template <typename user_msg_type>
    void send(user_msg_type *msg) {
      boost::shared_ptr<void> m0(msg);
      boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
      notify (m);
    }

    //after sending: 1> channel becomes owner, if deleter does real deletion
    // 2> sender still owns msg, if deleter does nothing
    template <typename user_msg_type, typename deleter>
    void send(user_msg_type *msg, deleter deler) {
      boost::shared_ptr<void> m0(msg, deler);
      boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
      notify (m);
    }

    //user_msg is already smarter pointer, channel becomes owner
    template <typename user_msg_type>
    void send(boost::shared_ptr<user_msg_type> msg) {
      boost::shared_ptr<void> m0(msg);
      boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
      notify (m);
    }

    //for channel internal use on wildcard named_out
    template <typename user_msg_type>
    void send(id_type id, boost::shared_ptr<user_msg_type> msg) {
      boost::shared_ptr<void> m0(msg);
      boost::shared_ptr<msg_type> m(new msg_type(id, m0));
      notify (m);
    }

  };

  //pull_recver_base class, base for join/choice pattern
  template <typename name_space, typename platform, template <class,class,class> class msg_que_type>
  struct pull_recver_base {
    typedef typename name_space::id_type id_type;
    typedef typename name_space::synch_policy synch_policy;
    typedef typename name_space::executor executor;
    typedef name<id_type,executor,synch_policy> name_type;
    typedef message<id_type> msg_type;
    typedef typename name_type::binding_set_type binding_set_type;
    typedef pull_sender_base<name_space, platform, msg_que_type> sender_type;
    typedef named_out<name_space, sender_type> named_out_type;

    name_type * name_;
    //msg_handler_base2<id_type> * recver_;
    msg_handler_base<id_type> * recver_;
    executor *exec_;
    sender_type *claimed_sender_;

    template<typename recver_type>
    pull_recver_base(name_type *n, recver_type rc, executor *e) :
      name_(n), exec_(e), claimed_sender_(NULL)
    {
      //recver_ = new msg_handler2<id_type, recver_type>(rc);
      recver_ = new msg_handler<id_type, recver_type>(rc);
    }

    ~pull_recver_base() {
      if (recver_ != NULL)
        recver_->destroy();
      if (claimed_sender_ != NULL)
        claimed_sender_->unclaim_one();
    }

    void set_exe(executor *e) {exec_ = e;}

    //senders call this to notify about some data coming
    //put this recver into activation list
    bool notify(boost::shared_ptr<msg_type> msg) {
      /*
        if (exec_ != NULL) //run recv_handler in executor
        exec_->execute(boost::bind(&msg_handler_base2<id_type>::invoke, recver_,id));
        else //run recv_handler in place
        recver_->invoke(id);
      */
      if (exec_ != NULL) //run recv_handler in executor
        exec_->execute(boost::bind(&msg_handler_base<id_type>::invoke, recver_, msg->id_, msg->data_));
      else //run recv_handler in place
        recver_->invoke(msg->id_, msg->data_);

      return true; //always return true to force sender to notify others
    }

    //claim data at sender
    bool claim(void) {
      if (claimed_sender_ != NULL) { //should not happen
        claimed_sender_->unclaim_one();
        claimed_sender_ = NULL;
      }
      //go-thru binding_set to claim
      typename synch_policy::scoped_lock lock(name_->bind_lock_);
      binding_set_type &bindings = name_->bindings_;
      if (!bindings.empty()) {
        for(typename binding_set_type::iterator iter = bindings.begin();
            iter != bindings.end() && claimed_sender_ == NULL; iter++) {
          named_out_type *named_out = (named_out_type *)(*iter);
          sender_type *sender = (sender_type *)(named_out);
          if(sender->claim_one()) claimed_sender_ = sender;
        }
      }
      if (claimed_sender_ != NULL) return true;
      else return false;
    }

    //
    void unclaim(void) {
      if (claimed_sender_ != NULL) {
        claimed_sender_->unclaim_one();
        claimed_sender_ = NULL;
      }
    }

    //pull data from sender
    int pull(boost::shared_ptr<msg_type> & msg) {
      int sz = 0;
      if (claimed_sender_ != NULL) {
        sz = claimed_sender_->pull(msg);
        claimed_sender_->unclaim_one();
        claimed_sender_ = NULL;
      }
      return sz;
    }
  };

}

  }
}

#endif
