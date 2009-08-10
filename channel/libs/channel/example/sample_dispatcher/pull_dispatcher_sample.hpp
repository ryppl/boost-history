//
// pull_dispatcher_sample.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PULL_DISPATCHER_SAMPLE_HPP
#define PULL_DISPATCHER_SAMPLE_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/channel/channel.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      template<class,class,template<class,class,class>class> class pull_recver_sample;

      template <typename name_space, typename platform, template <class,class,class> class msg_queue_type>
      struct pull_sender_sample :
	public msg_queue_type<boost::shared_ptr<message<typename name_space::id_type> >,
			    typename name_space::synch_policy,
			    platform> {
	typedef typename name_space::id_type id_type;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef name<id_type,executor,synch_policy> name_type;
	typedef message<id_type> msg_type;
	typedef typename name_type::binding_set_type binding_set_type;
	typedef msg_queue_type<boost::shared_ptr<msg_type>, synch_policy,
			       platform> que_type;
	typedef pull_recver_sample<name_space, platform,msg_queue_type> recver_type;
	typedef named_in<name_space, recver_type> named_in_type;

	name_type * name_;

	pull_sender_sample(name_type *n, executor *) : que_type(), name_(n) { }
	~pull_sender_sample() {}

	//assuming msgs contained inside shared_ptr
	void notify(boost::shared_ptr<msg_type> msg) {
	  //first store msg in que
	  put(msg);
	  //notify recevers
	  typename synch_policy::scoped_lock lock(this->name_->bind_lock_);
	  binding_set_type &bindings = this->name_->bindings_;
	  if (!bindings.empty()) {
	    bool cont = true;
	    for(typename binding_set_type::iterator iter = bindings.begin();
		iter != bindings.end() && cont; iter++) {
	      named_in_type *named_in = (named_in_type *)(*iter);
	      recver_type *recver = (recver_type *)named_in;
	      //for pull dispatchers, only send msgs to local receviers
	      if (named_in->type_ == named_in_type::member_local)
		cont = recver->notify(msg->id_);
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

      //synchronous/blocking pull_recver
      template <typename name_space, typename platform, template <class,class,class> class msg_que_type>
      struct pull_recver_sample {
	typedef typename name_space::id_type id_type;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef name<id_type,executor,synch_policy> name_type;
	typedef message<id_type> msg_type;
	typedef pull_sender_sample<name_space, platform, msg_que_type> sender_type;
	typedef named_out<name_space, sender_type> named_out_type;
	typedef typename name_type::binding_set_type binding_set_type;

	name_type * name_;
	typename synch_policy::mutex mutex_;
	typename synch_policy::condition cond_;
	int num_waiting_;

	pull_recver_sample(name_type *n) :
	  name_(n), num_waiting_(0) {}

	~pull_recver_sample() {}

	//recving thread will block here and wait for notify from senders
	void recv(id_type & id, boost::shared_ptr<void> & msg) {
	  int st;
	  boost::shared_ptr<msg_type> mesg;
	  typename synch_policy::scoped_lock lock(mutex_);
	  while(1) {
	    st = pull(mesg);
	    if (st != 0) {
	      id = mesg->id_;
	      msg = mesg->data_;
	      return;
	    }
	    num_waiting_++;
	    cond_.wait(lock);
	    num_waiting_--;
	  }
	}

	//if any thread block waiting here
	bool notify(id_type id) {
	  typename synch_policy::scoped_lock lock(mutex_);
	  if (num_waiting_ > 0) {
	    cond_.notify_one();
	    return false;
	  }
	  else
	    return true;
	}

	//pull data from sender
	int pull(boost::shared_ptr<msg_type> & msg) {
	  int st = 0;
	  //go-thru binding_set to pull from senders
	  typename synch_policy::scoped_lock lock(name_->bind_lock_);
	  binding_set_type &bindings = name_->bindings_;
	  if (!bindings.empty()) {
	    for(typename binding_set_type::iterator iter = bindings.begin();
		iter != bindings.end() && st == 0; iter++) {
	      named_out_type *named_out = (named_out_type *)(*iter);
	      sender_type *sender = (sender_type *)(named_out);
	      st = sender->pull(msg);
	    }
	  }
	  return st;
	}
      };

    }

    template <typename name_space, typename platform, template <class,class,class> class queue_type = unbounded_que>
    struct pull_dispatcher_sample {
      typedef detail::pull_sender_sample<name_space,platform,queue_type> sender;
      typedef detail::pull_recver_sample<name_space,platform,queue_type> recver;
    };

  }
}

#endif
