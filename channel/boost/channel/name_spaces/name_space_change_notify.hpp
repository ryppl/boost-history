//
// name_space_change_notify.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NAME_SPACE_CHANGE_NOTIFY_HPP
#define NAME_SPACE_CHANGE_NOTIFY_HPP

#include <boost/shared_ptr.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/dispatchers/broadcast_dispatcher.hpp>
#include <boost/channel/message.hpp>

namespace boost {
  namespace channel {
    namespace detail {

      template<typename name_space_type>
      class name_space_change_notify {
	typedef	typename name_space_type::id_type id_type;
	typedef typename name_space_type::id_trait id_trait;
	typedef typename name_space_type::name name;
	typedef named_out<name_space_type,
                          typename broadcast_dispatcher<name_space_type,
                                                        typename name_space_type::platform>::sender> named_out;
	struct binding_notify {
          named_out *bind_out, *unbind_out, *bind_in, *unbind_in;
	};
	binding_notify notify_[name_base::scope_number];
	bool silent;
      public:
	name_space_change_notify(name_space_type & ns) {
          silent = true;
          for(int i=0;i<name_base::scope_number;i++) {
            notify_[i].bind_out = new named_out(ns, (id_type)id_trait::publication_info_msg,(typename name::scope_type)i);
            notify_[i].unbind_out = new named_out(ns, (id_type)id_trait::unpublication_info_msg,(typename name::scope_type)i);
            notify_[i].bind_in = new named_out(ns, (id_type)id_trait::subscription_info_msg,(typename name::scope_type)i);
            notify_[i].unbind_in = new named_out(ns, (id_type)id_trait::unsubscription_info_msg,(typename name::scope_type)i);
          }
          silent = false;
	}

	~name_space_change_notify() {
          silent = true;
          for(int i=0;i<name::scope_number;i++) {
            delete notify_[i].bind_out;
            delete notify_[i].unbind_out;
            delete notify_[i].bind_in;
            delete notify_[i].unbind_in;
          }
          silent = false;
	}

	///notify all related (local/remote/global) named_ins/outs about changes
	void notify(typename name::binding_event op, name *n)
	{
          typename name_base::scope_type scope = n->scope_;
          if(n->id_ == id_trait::publication_info_msg ||
             n->id_ == id_trait::unpublication_info_msg ||
             n->id_ == id_trait::subscription_info_msg ||
             n->id_ == id_trait::unsubscription_info_msg) {
            if(silent)
              return;
            scope = name::scope_local;
          }
          boost::shared_ptr<pubsub_info_msg_t<id_type> > sub(new pubsub_info_msg_t<id_type>());
          sub->msg_types.push_back(n->id_);
          named_out *no;
          switch(op) {
          case name::bind_out_ev:
            no = notify_[scope].bind_out;
            break;
          case name::unbind_out_ev:
            no = notify_[scope].unbind_out;
            break;
          case name::bind_in_ev:
            no = notify_[scope].bind_in;
            break;
          case name::unbind_in_ev:
            no = notify_[scope].unbind_in;
            break;
          default:
            break;
          }
          no->send(sub);
	}

	///just notify external (remote) named_ins/outs (at interface) about changes
	void notify_external(typename name::binding_event op, name *n)
	{
          if (n->scope_ == name::scope_local) return;
          //dont need to send sys_msgs pub/subs, interface has done all
          if(n->id_ == id_trait::publication_info_msg ||
             n->id_ == id_trait::unpublication_info_msg ||
             n->id_ == id_trait::subscription_info_msg ||
             n->id_ == id_trait::unsubscription_info_msg)
            return;
          boost::shared_ptr<pubsub_info_msg_t<id_type> > sub(new pubsub_info_msg_t<id_type>());
          sub->msg_types.push_back(n->id_);
          named_out *no;
          switch(op) {
          case name::bind_out_ev:
            no = notify_[name::scope_remote].bind_out;
            break;
          case name::unbind_out_ev:
            no = notify_[name::scope_remote].unbind_out;
            break;
          case name::bind_in_ev:
            no = notify_[name::scope_remote].bind_in;
            break;
          case name::unbind_in_ev:
            no = notify_[name::scope_remote].unbind_in;
            break;
          default:
            break;
          }
          no->send(sub);
	}

	///just notify internal (local) named_ins/outs about changes
	void notify_internal(typename name::binding_event op, name *n)
	{
          if (n->scope_ == name::scope_remote) return;
          if(silent) {
            if(n->id_ == id_trait::publication_info_msg ||
               n->id_ == id_trait::unpublication_info_msg ||
               n->id_ == id_trait::subscription_info_msg ||
               n->id_ == id_trait::unsubscription_info_msg)
              return;
          }
          boost::shared_ptr<pubsub_info_msg_t<id_type> > sub(new pubsub_info_msg_t<id_type>());
          sub->msg_types.push_back(n->id_);
          named_out *no;
          switch(op) {
          case name::bind_out_ev:
            no = notify_[name::scope_local].bind_out;
            break;
          case name::unbind_out_ev:
            no = notify_[name::scope_local].unbind_out;
            break;
          case name::bind_in_ev:
            no = notify_[name::scope_local].bind_in;
            break;
          case name::unbind_in_ev:
            no = notify_[name::scope_local].unbind_in;
            break;
          default:
            break;
          }
          no->send(sub);
	}
      };
    }
  }
}

#endif
