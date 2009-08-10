//
// interface.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/channel/binder.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/dispatchers/broadcast_dispatcher.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/peer.hpp>
#include <boost/channel/name.hpp>

namespace boost {
  namespace channel {

    //interface to other channels or streams
    template<typename channel>
    class interface: public peer_type<typename channel::id_type> {
    public:
      enum state_type {
        intf_init = 0, intf_active
      };

      typedef   typename channel::platform platform;
      typedef typename channel::name_space_type name_space;
      typedef typename name_space::synch_policy synch_policy;
      typedef typename channel::id_type id_type;
      typedef typename channel::id_trait id_trait;
      typedef typename name_space::name name;
      typedef named_out<name_space, typename broadcast_dispatcher<name_space,platform>::sender > ns_named_out;
      typedef typename channel::out named_out;
      typedef typename channel::in named_in;
      typedef peer_type<id_type> peer_type;
      typedef message<id_type> message_type;
      //      typedef connection<id_type> connection_type;
      typedef typename channel::connection connection_type;
      typedef typename channel::executor executor;
      typedef named_out_bundle<name_space, typename broadcast_dispatcher<name_space,platform>::sender> ns_named_out_bundle;
      typedef named_in_bundle<name_space, typename broadcast_dispatcher<name_space,platform>::recver> ns_named_in_bundle;
      typedef named_out_bundle<name_space, typename channel::dispatch_policy::sender> named_out_bundle;
      typedef named_in_bundle<name_space, typename channel::dispatch_policy::recver> named_in_bundle;
      typedef binder_type<id_type> binder_type;
      typedef translator_type<id_type> translator_type;
      typedef filter_type<id_type> filter_type;
      typedef pubsub_info_msg_t<id_type> pubsub_info_msg_t;

      typedef interface<channel> interface_type;

    private:
      ///
      channel &ch_;
      state_type state_;

      //ids bound at local channel
      //for namespace change, using broadcast dispatching
      ns_named_in_bundle ns_named_ins_;
      ns_named_out_bundle ns_named_outs_;
      //for application messages, using channel specific dispatching
      named_in_bundle named_ins_;
      named_out_bundle named_outs_;

      ///outgoing-msg buffering when connection to remote peer is not ready yet
      std::vector<boost::shared_ptr<message_type> > pending_msgs_;
      typename synch_policy::mutex lock_;///lock to maintain interface internal state

      ///filter & translators: policies for remote connections
      filter_type *filter_;
      translator_type *translator_;

    public:
      interface (channel &chan, binder_type *binder) :
        ch_(chan),
        ns_named_ins_(chan, boost::bind(&interface::recv,this, _1,_2), name_base::member_remote),
        ns_named_outs_(chan, name_base::member_remote),
        named_ins_(chan, boost::bind(&interface::recv,this, _1,_2),name_base::member_remote),
        named_outs_(chan, name_base::member_remote)
      {
        state_ = intf_init;
        ch_.add_intf(this);
        if (binder == NULL) {
          filter_ = NULL;
          translator_ = NULL;
        } else {
          filter_ = binder->filter;
          translator_ = binder->translator;
        }
        //subscribe to system msgs from local channel on behalf of the remote side
        //since channel_(DIS)CONN_msg and init_SUB/PUB_info_msg
        //are generated inside interface, no need to subscribe to them in channels
        ns_named_ins_.bind(channel::subscription_info_msg, name_base::scope_local);
        ns_named_ins_.bind(channel::unsubscription_info_msg, name_base::scope_local);
        ns_named_ins_.bind(channel::publication_info_msg, name_base::scope_local);
        ns_named_ins_.bind(channel::unpublication_info_msg, name_base::scope_local);
        //publish system msgs at local channel on behalf of the remote side
        ns_named_outs_.bind(channel::channel_conn_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::channel_disconn_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::init_subscription_info_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::connection_ready_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::subscription_info_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::unsubscription_info_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::publication_info_msg, name_base::scope_local);
        ns_named_outs_.bind(channel::unpublication_info_msg, name_base::scope_local);
      }

      ~interface ()
      {
        //destructors of named_in/out_bundles will unbind and destroy
        //all named_in_out objects; so we dont need to explicitly unbind/delete
        //named_ins_,named_outs_,ns_named_ins_,ns_named_outs_
        ch_.del_intf(this);
        platform::log("... interface destructor called ...");
      }

      /*
        void unbind() {
        //unbind app ids at remote end
        platform::log("... interface unbind enter ...");
        std::vector<id_type> ids;
        typename std::vector<id_type>::iterator iter;
        if (named_ins_.get_ids(ids)) {
        for(iter = ids.begin(); iter != ids.end(); iter++)
        send2remote_pubsub_msg (name_base::unbind_in_ev, *iter);
        }
        ids.clear();
        if (named_outs_.get_ids(ids)) {
        for(iter = ids.begin(); iter != ids.end(); iter++)
        send2remote_pubsub_msg (name_base::unbind_out_ev, *iter);
        }
        peer_type::unbind();
        platform::log("... interface unbind exit ...");
        }
      */

      bool sys_msg(id_type id) {
        if(id == channel::channel_conn_msg ||
           id == channel::channel_disconn_msg ||
           id == channel::init_subscription_info_msg ||
           id == channel::connection_ready_msg ||
           id == channel::subscription_info_msg ||
           id == channel::unsubscription_info_msg ||
           id == channel::publication_info_msg ||
           id == channel::unpublication_info_msg)
          return true;
        return false;
      }

      ///called from inside channel, forward msgs from inside channel to peers
      void recv(id_type id, boost::shared_ptr<void> msg)
      {
        platform::log("interface::recv ["+id_trait::id_to_string(id)+"]");
        if(state_ != intf_active) {
          boost::shared_ptr<message_type> m(new message_type(id,msg));
          add_pending_msg(m);
          platform::log("... msgs["+id_trait::id_to_string(id)+"] buffered ");
        } else {
          bool need_send = false;

          if (sys_msg(id)) {
            //check pub/unpub, sub/unsub msgs
            pubsub_info_msg_t *info = (pubsub_info_msg_t *)msg.get();
            if (id == channel::subscription_info_msg) {
              for(size_t i=0; i<info->msg_types.size() && !need_send; i++) {
                if (filter_ != NULL && filter_->block_inward(info->msg_types[i]))
                  continue;
                if (!sys_msg(info->msg_types[i]) && named_outs_.find(info->msg_types[i]) == NULL) {
                  if (translator_ != NULL)
                    translator_->translate_outward(info->msg_types[i]);
                  need_send = true;
                }
              }
            }
            else if (id == channel::unsubscription_info_msg) {
              for(size_t i=0; i<info->msg_types.size() && !need_send; i++) {
                named_out *no;
                if (!sys_msg(info->msg_types[i]) && (no = named_outs_.find(info->msg_types[i])) != NULL) {
                  if (no->num_bindings() == 0) {
                    if (translator_ != NULL)
                      translator_->translate_outward(info->msg_types[i]);
                    need_send = true;
                  }
                }
              }
            }
            else if (id == channel::publication_info_msg) {
              for(size_t i=0; i<info->msg_types.size() && !need_send; i++) {
                if (filter_ != NULL && filter_->block_outward(info->msg_types[i]))
                  continue;
                if (!sys_msg(info->msg_types[i]) && named_ins_.find(info->msg_types[i]) == NULL) {
                  if (translator_ != NULL)
                    translator_->translate_outward(info->msg_types[i]);
                  need_send = true;
                }
              }
            }
            else if (id == channel::unpublication_info_msg) {
              for(size_t i=0; i<info->msg_types.size() && !need_send; i++) {
                named_in *ni;
                if (!sys_msg(info->msg_types[i]) && (ni = named_ins_.find(info->msg_types[i])) != NULL) {
                  if (ni->num_bindings() == 0) {
                    if (translator_ != NULL)
                      translator_->translate_outward(info->msg_types[i]);
                    need_send = true;
                  }
                }
              }
            }
          } else { //applicaion msgs
            //translate application msgs if set
            if (translator_ != NULL) {
              translator_->translate_outward(id);
            }
            need_send = true;
          }

          if(need_send)
            peer_send(id, msg);
        }
      }

      ///called by peers, forward msgs from outside peer to channel
      void send(id_type id, boost::shared_ptr<void> msg)
      {
        platform::log("interface::send_msg: recv MSG ["+id_trait::id_to_string(id)+"]...");
        pubsub_info_msg_t *subinfo;

        /// --- connection setup hand-shaking ----
        if (id == channel::channel_conn_msg) {
          //connector or stream should already intercept this msg for its own internal
          //processing, such as connection management; here we forward it to subscribed
          //user code
          switch(this->role_) {
          case peer_type::active_role:
            platform::log("active end recv channel_info_msg ...");
            send2remote_init_subscribe_msg();
            break;
          case peer_type::passive_role:
            platform::log("passive end recv channel_info_msg ...");
            ///send my chan info
            send2remote_channel_info_msg();
            break;
          default:
            break;
          }
        }
        else if(id == channel::channel_disconn_msg) {
          //connector or stream should already intercept this msg for its own internal
          //processing, such as connection management; here we forward it to subscribed
          //user code
        }
        else if (id == channel::init_subscription_info_msg) {
          platform::log("recv init_subscription_info_msg...");
          subinfo = (pubsub_info_msg_t *)msg.get();
          std::vector<id_type> pub_msgs;
          ch_.bound_ids_for_out(name::exported_name, pub_msgs);
          for(size_t i=0; i<subinfo->msg_types.size(); i++) {
            if (translator_ != NULL)
              translator_->translate_inward(subinfo->msg_types[i]);
            if (filter_ != NULL && filter_->block_outward(subinfo->msg_types[i]))
              continue;
            if(std::find_if(pub_msgs.begin(), pub_msgs.end(),
                            boost::bind(&id_trait::match, _1, subinfo->msg_types[i])) !=
               pub_msgs.end()) {
              platform::log("remote subsc to ["+id_trait::id_to_string(subinfo->msg_types[i])+"]");
              named_ins_.bind(subinfo->msg_types[i], name_base::scope_local);
              send2remote_pubsub_msg (name_base::bind_out_ev, subinfo->msg_types[i]);
            }
          }
          if(this->role_ == peer_type::passive_role) {
            ///send my subscription info
            send2remote_init_subscribe_msg();
          } else if(this->role_ == peer_type::active_role) {
            send2remote_conn_ready_msg();
          }
        } else if (id == channel::connection_ready_msg) {
          platform::log("recv conn_ready...");
          platform::log((this->role_ == peer_type::active_role)?"i am active":"i am passive");
          state_ = intf_active;
          platform::log("conn active now...");
          resend_pending_msgs();
          if (this->role_ == peer_type::passive_role) {
            send2remote_conn_ready_msg();
          }
        }
        /// --- name_space change msgs during normal operations ---
        ///init_sub_msgs
        else if (id == channel::subscription_info_msg) {
          platform::log("recv subscription_info_msg ...");
          subinfo = (pubsub_info_msg_t *)msg.get();
          std::vector<id_type> global_msgs;
          ch_.bound_ids_for_out(name::exported_name, global_msgs);
          for(size_t i=0; i<subinfo->msg_types.size(); i++) {
            if (translator_ != NULL)
              translator_->translate_inward(subinfo->msg_types[i]);
            if (filter_ != NULL && filter_->block_outward(subinfo->msg_types[i]))
              continue;
            if (named_ins_.find(subinfo->msg_types[i]) == NULL) {
              if(std::find_if(global_msgs.begin(), global_msgs.end(),
                              boost::bind(&id_trait::match, _1, subinfo->msg_types[i])) !=
                 global_msgs.end()) {
                platform::log("remote subsc to ["+id_trait::id_to_string(subinfo->msg_types[i])+"]");
                if(named_ins_.bind(subinfo->msg_types[i], name_base::scope_local))
                  send2remote_pubsub_msg (name_base::bind_out_ev, subinfo->msg_types[i]);
              }
            }
          }
        }
        else if (id == channel::unsubscription_info_msg) {
          platform::log("recv unsubscription_info_msg...");
          subinfo = (pubsub_info_msg_t *)msg.get();
          for(size_t i=0; i<subinfo->msg_types.size(); i++) {
            if (translator_ != NULL)
              translator_->translate_inward(subinfo->msg_types[i]);
            if(named_ins_.unbind(subinfo->msg_types[i]))
              send2remote_pubsub_msg (name_base::unbind_out_ev, subinfo->msg_types[i]);
          }
        }
        else if (id == channel::publication_info_msg) {
          platform::log("recv publication_info_msg...");
          pubsub_info_msg_t *pubinfo = (pubsub_info_msg_t *)msg.get();
          std::vector<id_type> mtypes;
          ch_.bound_ids_for_in(name::exported_name, mtypes);
          for(size_t i=0; i<pubinfo->msg_types.size(); i++) {
            if (translator_ != NULL)
              translator_->translate_inward(pubinfo->msg_types[i]);
            if (filter_ != NULL && filter_->block_inward(pubinfo->msg_types[i]))
              continue;
            if (named_outs_.find (pubinfo->msg_types[i]) == NULL) {
              //platform::log("remote pub msg, try to match ["+id_trait::id_to_string(pubinfo->msg_types[i])+"]");
              if(std::find_if(mtypes.begin(), mtypes.end(),
                              boost::bind(&id_trait::match, _1, pubinfo->msg_types[i])) !=
                 mtypes.end()) {
                platform::log("remote pub msg ["+id_trait::id_to_string(pubinfo->msg_types[i])+"]");
                ///remote msgs only for local destinations
                if(named_outs_.bind (pubinfo->msg_types[i], name_base::scope_local))
                  send2remote_pubsub_msg(name_base::bind_in_ev, pubinfo->msg_types[i]);
              }
            }
          }
        }
        else if (id == channel::unpublication_info_msg) {
          platform::log("recv unpublication_info_msg...");
          subinfo = (pubsub_info_msg_t *)msg.get();
          for(size_t i=0; i<subinfo->msg_types.size(); i++) {
            if (translator_ != NULL)
              translator_->translate_inward(subinfo->msg_types[i]);
            if(named_outs_.unbind(subinfo->msg_types[i]))
              send2remote_pubsub_msg (name_base::unbind_in_ev, subinfo->msg_types[i]);
          }
        }
        /// --- application msgs ----
        else {
          //application msgs go to channel
          //translate it if set
          if (translator_ != NULL) {
            translator_->translate_inward(id);
          }
          //platform::log("recv an application msg ["+id_trait::id_to_string(id)+"]");
        }

        ///forward all msgs to local channel
        if(sys_msg(id)) { //forward system msgs
          ns_named_out *no = ns_named_outs_.find(id);
          if (no != NULL)
            no->send (msg);
        }
        else { //forward application msgs
          typename channel::out *no = named_outs_.find(id); //find exact match
          if (no != NULL) { //exact match found
            no->send (msg);
          } else {
            //platform::log("interface::send try to match a local wildcard with name: "+id_trait::id_to_string(id));
            no = named_outs_.find_match(id);
            if (no != NULL) { //assoc/fuzzy match found
              if (id_trait::wildcard_name(no->id_)) {
                platform::log("interface::send match a local wildcard name: "+id_trait::id_to_string(no->id_));
                no->send (id,msg);
              } else
                no->send (msg);
            }
          }
        }
      }

      ///send owner channel info to remote
      void send2remote_channel_info_msg(void)
      {
        platform::log("send_chan_info_to_remote...");
        std::pair<typename peer_type::type, std::string> info = this->peer_get_info();
        boost::shared_ptr<channel_info_msg_t> msg(new channel_info_msg_t());
        msg->intf = this;
        msg->is_local = true;
        if (info.first == peer_type::remote_peer)
          {
            msg->is_local = false;
            msg->host_addr = info.second;
          }
        peer_send(channel::channel_conn_msg,msg);
      }

    private:

      /**
       * methods handling msg buffering before remote connection is ready
       */
      void add_pending_msg(boost::shared_ptr<message_type> m) {
        typename synch_policy::scoped_lock lock(lock_);
        pending_msgs_.push_back(m);
      }
      void resend_pending_msgs(void)
      {
        if(this->peer_ != NULL) {
          typename synch_policy::scoped_lock lock(lock_);
          for(size_t i=0; i<pending_msgs_.size();i++) {
            boost::shared_ptr<message_type> m = pending_msgs_[i];
            peer_send(m->id_,m->data_);
            platform::log("...resend 1 msg");
          }
          pending_msgs_.clear();
        }
      }

      /* ----- */

      ///send channel conn ready msg to remote
      void send2remote_conn_ready_msg(void)
      {
        platform::log("send_conn_ready_to_remote...");
        std::pair<typename peer_type::type, std::string> info = this->peer_get_info();
        boost::shared_ptr<channel_info_msg_t> msg(new channel_info_msg_t());
        msg->intf = this;
        msg->is_local = true;
        if (info.first == peer_type::remote_peer)
          {
            msg->is_local = false;
            msg->host_addr = info.second;
          }
        peer_send(channel::connection_ready_msg,msg);
      }

      ///when 2 channels just connected, send all current sub info
      ///to remote
      void send2remote_init_subscribe_msg(void)
      {
        ///send subscription msg
        boost::shared_ptr<pubsub_info_msg_t> sub(new pubsub_info_msg_t());
        std::vector<id_type> mtypes;
        ch_.bound_ids_for_in(name::exported_name, mtypes);
        for(size_t i=0; i<mtypes.size(); i++) {
          if (sys_msg(mtypes[i]) || (filter_ != NULL && filter_->block_inward(mtypes[i])))
            continue;
          if (translator_ != NULL)
            translator_->translate_outward(mtypes[i]);
          sub->msg_types.push_back(mtypes[i]);
        }
        platform::log("send init_subscription info...");
        peer_send(channel::init_subscription_info_msg, sub);
      }

      ///forward local/owner channel memebers' pub/sub operations to remote
      void send2remote_pubsub_msg(typename name_base::binding_event op, id_type t)
      {
        boost::shared_ptr<pubsub_info_msg_t> sub(new pubsub_info_msg_t());
        id_type mt;
        switch(op) {
        case name_base::bind_out_ev:
          if (filter_ != NULL && filter_->block_outward(t)) {
            return;
          }
          mt = channel::publication_info_msg;
          platform::log("interface::send2remote_pubsub_msg publish ");
          break;
        case name_base::unbind_out_ev:
          mt = channel::unpublication_info_msg;
          platform::log("interface::send2remote_pubsub_msg unpublish ");
          break;
        case name_base::bind_in_ev:
          if (filter_ != NULL && filter_->block_inward(t)) {
            return;
          }
          mt = channel::subscription_info_msg;
          platform::log("interface::send2remote_pubsub_msg subscribe ");
          break;
        case name_base::unbind_in_ev:
          mt = channel::unsubscription_info_msg;
          platform::log("interface::send2remote_pubsub_msg unsubscribe ");
          break;
        default:
          platform::log("interface::send2remote_pubsub_msg invalid id_type ");
          return;
        }
        if (translator_ != NULL)
          translator_->translate_outward(t);
        sub->msg_types.push_back(t);
        peer_send(mt, sub);
      }
    };

  }
}

#endif
