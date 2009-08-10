//
// pull_dispatcher_base.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


//// push dispatchers:
//// define push-style message passing process:
//// including:
////   . provide both sender/recver sides

#ifndef PUSH_DISPATCHER_BASE_HPP
#define PUSH_DISPATCHER_BASE_HPP

#include <map>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/name.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/dispatchers/dispatcher_base.hpp>

namespace boost {
  namespace channel {

    namespace detail {

      //base sender class, shared by most push dispatchers
      template<typename name_space, typename platform,
               template<typename , typename > class sending_algo>
      struct push_sender_base {
	typedef	typename name_space::id_type id_type;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef name<id_type,executor,synch_policy> name_type;
	typedef message<id_type> msg_type;
	typedef typename name_type::binding_set_type binding_set_type;

	name_type * name_;
	executor *exec_;
	sending_algo<name_space, platform> algo;

	push_sender_base(name_type * n, executor *e) : name_(n), exec_(e) {}

	//assuming msgs contained inside shared_ptr
	void push(boost::shared_ptr<msg_type> msg) {
          typename synch_policy::scoped_lock lock(name_->bind_lock_);
          typename name_type::binding_set_type &bindings = name_->bindings_;
          algo(bindings, msg);
	}

	//after sending, channel becomes owner of msgs
	template <typename user_msg_type>
	void send(user_msg_type *msg) {
          boost::shared_ptr<void> m0(msg);
          boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
          push (m);
	}

	//after sending: 1> channel becomes owner, if deleter does real deletion
	// 2> sender still owns msg, if deleter does nothing
	template <typename user_msg_type, typename deleter>
	void send(user_msg_type *msg, deleter deler) {
          boost::shared_ptr<void> m0(msg, deler);
          boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
          push (m);
	}

	//user_msg is already smarter pointer, channel becomes owner
	template <typename user_msg_type>
	void send(boost::shared_ptr<user_msg_type> msg) {
          boost::shared_ptr<void> m0(msg);
          boost::shared_ptr<msg_type> m(new msg_type(name_->id_, m0));
          push (m);
	}

	//for channel internal use on wildcard named_out
	template <typename user_msg_type>
	void send(id_type id, boost::shared_ptr<user_msg_type> msg) {
          boost::shared_ptr<void> m0(msg);
          boost::shared_ptr<msg_type> m(new msg_type(id, m0));
          push (m);
	}

      };

      //base recver class, shared by most push dispatchers
      template <typename name_space, typename platform>
      struct push_recver_base {
	typedef typename name_space::id_type id_type;
	typedef typename name_space::synch_policy synch_policy;
	typedef typename name_space::executor executor;
	typedef message<id_type> msg_type;
	typedef name<id_type,executor,synch_policy> name_type;

	name_type * name_;
	executor *exec_;
	msg_handler_base<id_type> *recver_;

	template<typename recver_type>
	push_recver_base(name_type * n, recver_type rc, executor *e) :
          name_(n), exec_(e)
	{
          recver_ = new msg_handler<id_type, recver_type>(rc);
	}

	~push_recver_base() {
          if (recver_ != NULL)
            recver_->destroy();
	}

	void set_exe(executor *e) {exec_ = e;}

	template<typename recver_type>
	void set_recver(recver_type rc) {
          if (recver_ != NULL) recver_->destroy();
          recver_ = new msg_handler<id_type, recver_type>(rc);
	}

	//senders call this to push into here
	void push(boost::shared_ptr<msg_type> msg)
	{
          if (exec_ != NULL) //run recv_handler in executor
            exec_->execute(boost::bind(&msg_handler_base<id_type>::invoke,
                                       recver_,msg->id_, msg->data_));
          else //run recv_handler in place
            recver_->invoke(msg->id_, msg->data_);
	}

      };

    }

  }
}

#endif
