//
// channel.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
#  if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#  endif // !defined(WIN32_LEAN_AND_MEAN)
#endif

#include <set>
#include <boost/channel/binder.hpp>
#include <boost/channel/connection.hpp>
#include <boost/channel/peer.hpp>
#include <boost/channel/dispatchers/broadcast_dispatcher.hpp>
#include <boost/channel/dispatchers/round_robin_dispatcher.hpp>
#include <boost/channel/dispatchers/always_latest_dispatcher.hpp>
#include <boost/channel/dispatchers/pull_dispatcher.hpp>
#include <boost/channel/executors/executor_base.hpp>
#include <boost/channel/interface.hpp>
#include <boost/channel/name_spaces/linear_id_trait.hpp>
#include <boost/channel/name_spaces/assoc_id_trait.hpp>
#include <boost/channel/name_spaces/linear_name_space.hpp>
#include <boost/channel/name_spaces/hierarchical_id_trait.hpp>
#include <boost/channel/name_spaces/hierarchical_name_space.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/named_in_out.hpp>
#include <boost/channel/unnamed_in_out.hpp>
#include <boost/channel/platforms/synch_policy.hpp>
#include <boost/channel/platforms/boost_platform.hpp>
#include <boost/channel/marshaler.hpp>
#include <boost/channel/queues/queues.hpp>
#include <boost/channel/pub_sub.hpp>

namespace boost {
  namespace channel {

    template<
      typename idtype,
      typename platform_type = boost_platform,
      typename synchpolicy = mt_synch<platform_type> ,
      typename executor_type = abstract_executor, //force in-line execution
      typename name_space = linear_name_space<idtype, executor_type, synchpolicy>,
      typename dispatcher = broadcast_dispatcher<name_space, platform_type>
      >
    class channel: public id_trait<idtype> , public name_space {
    public:
      typedef idtype id_type;
      typedef id_trait<id_type> id_trait;
      typedef platform_type platform;
      typedef synchpolicy synch_policy;
      typedef name_space name_space_type;
      typedef executor_type executor;
      typedef dispatcher dispatch_policy;
      typedef channel<id_type, platform, synch_policy, executor, name_space, dispatcher> channel_type;
      typedef message<id_type> message_type;
      typedef named_in<name_space_type, typename dispatch_policy::recver> in;
      typedef named_out<name_space_type, typename dispatch_policy::sender> out;
      typedef subscriber<name_space_type, typename dispatch_policy::recver> subscriber;
      typedef publisher<name_space_type, typename dispatch_policy::sender> publisher;
      typedef port<executor, id_type, platform, synch_policy, name_space_type, dispatch_policy> port;
      typedef signal<executor, id_type, platform, synch_policy, name_space_type, dispatch_policy> signal;
      typedef peer_type<id_type> peer_type;
      typedef binder_type<id_type> binder_type;
      typedef interface<channel_type> interface;
      typedef connection<id_type> connection;
      typedef marshaler_registry<id_type, id_trait, synch_policy, text_archive> text_marshaler_registry;
      typedef marshaler_registry<id_type, id_trait, synch_policy, binary_archive> binary_marshaler_registry;
      typedef marshaler_registry<id_type, id_trait, synch_policy, xml_archive> xml_marshaler_registry;
      typedef named_in<name_space_type, typename broadcast_dispatcher<name_space, platform>::recver> sys_named_in;
      typedef named_out<name_space_type, typename broadcast_dispatcher<name_space, platform>::sender> sys_named_out;
      typedef named_out_bundle<name_space, typename broadcast_dispatcher<name_space, platform>::sender> sys_named_outs;
      typedef named_in_bundle<name_space, typename broadcast_dispatcher<name_space, platform>::recver> sys_named_ins;

      channel(executor *e = NULL) :
        name_space(e) {
      }

      ~channel() {
        platform::log("... channel destructor called ...");
        for (typename std::set<interface*>::iterator iter = intfs_.begin(); iter != intfs_.end();) {
          typename std::set<interface*>::iterator curr = iter++;
          //need to get to next first, release() will callback and remove iter from set
          //release interface and kill connection; interface obj will be
          //deleted in connection's destructor
          (*curr)->release();
        }
      }

      template<typename recver_type>
      in * bind_name_in(id_type id, recver_type r, typename in::scope_type scope = in::scope_global,
                        typename in::member_type type = in::member_local, executor *e = NULL) {
        return new in(*this, id, r, scope, type, e);
      }
      out * bind_name_out(id_type id, typename out::scope_type scope = out::scope_global,
                          typename out::member_type type = out::member_local, executor *e = NULL) {
        return new out(*this, id, scope, type, e);
      }

      void add_intf(interface *c) {
        typename synch_policy::scoped_lock lock(lock_);
        intfs_.insert(c);
      }

      void del_intf(interface *c) {
        typename synch_policy::scoped_lock lock(lock_);
        intfs_.erase(c);
      }

      int num_conn(void) {
        return intfs_.size();
      }

    private:
      typename synch_policy::mutex lock_;
      std::set<interface *> intfs_;
    };

  /**
   *  function to connect 2 local channels of the same type
   */
  template<typename channel>
  typename channel::connection* connect(channel &peer1, channel &peer2,
                                        typename channel::binder_type *binder1 = NULL,
                                        typename channel::binder_type *binder2 = NULL) {
    typedef typename channel::interface interface;
    typedef typename channel::peer_type peer_type;
    typedef typename channel::connection connection;
    interface *intf1 = new interface(peer1, binder1);
    interface *intf2 = new interface(peer2, binder2);
    connection *conn = new connection(connection::local_connection,
                                      intf1, intf2);
    intf1->bind_peer(intf2, peer_type::active_role, conn);
    intf2->bind_peer(intf1, peer_type::passive_role, conn);

    //start connection setup hand-shaking
    intf1->send2remote_channel_info_msg();

    return conn;
  }

  /**
   *  function to connect 2 local channels of different types
   */
  template <typename channel1, typename channel2>
  typename channel1::connection* connect(channel1 &peer1, channel2 &peer2,
                                         typename channel1::binder_type *binder1 = NULL,
                                         typename channel2::binder_type *binder2 = NULL) {
    typedef typename channel1::interface interface1;
    typedef typename channel2::interface interface2;
    typedef typename channel1::peer_type peer_type;
    typedef typename channel1::connection connection;
    interface1 *intf1 = new interface1(peer1, binder1);
    interface2 *intf2 = new interface2(peer2, binder2);
    connection *conn = new connection(connection::local_connection,
                                      intf1, intf2);
    intf1->bind_peer(intf2, peer_type::active_role, conn);
    intf2->bind_peer(intf1, peer_type::passive_role, conn);

    //start connection setup hand-shaking
    intf1->send2remote_channel_info_msg();

    return conn;
  }

  /**
   * function to connect local channel to remote peer thru a stream obj
   */
  template <typename channel, typename stream_t>
  typename channel::connection* connect(channel &peer,
                                        stream_t * stream,
                                        bool active,
                                        typename channel::binder_type *binder = NULL
                                        )
  {
    typedef typename channel::interface interface;
    typedef typename channel::peer_type peer_type;
    typedef typename channel::connection connection;
    interface *intf = new interface(peer, binder);
    connection *conn = new connection(connection::remote_connection,
                                      intf, stream);
    intf->bind_peer(stream, active?peer_type::active_role:peer_type::passive_role, conn);
    stream->bind_peer(intf, active?peer_type::active_role:peer_type::passive_role, conn);

    if (active) {
      //start connection setup hand-shaking
      intf->send2remote_channel_info_msg();
    }

    return conn;
  }

  /**
   * generic functions to directly bind/unbind any pair of named_out and named_in
   */
  template <typename name>
  void bind(name *named_out, name *named_in) {
    named_out->bind(named_in);
    named_in->bind(named_out);
  }
  template <typename name>
  void unbind(name *named_out, name *named_in) {
    named_out->unbind(named_in);
    named_in->unbind(named_out);
  }

}
}

#endif
