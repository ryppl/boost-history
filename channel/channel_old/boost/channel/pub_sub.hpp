//
// pub_sub.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PUB_SUB_HPP
#define PUB_SUB_HPP

#include <vector>
//#include <boost/channel/named_in_out.hpp>

namespace boost {
  namespace channel {

    /**
     * the following 2 classes are convenience high level API classes
     * for end uers to use; to support publish/subscribe model
     */
    template<typename name_space, typename dispatcher_sender_type>
    class publisher: public named_out_bundle<name_space, dispatcher_sender_type> {
      typedef named_out_bundle<name_space, dispatcher_sender_type> named_out_bundle;
      typedef typename name_space::id_type id_type;
      typedef typename named_out_bundle::named_out named_out;
    public:
      publisher(name_space & ch,
                typename named_out::member_type type = named_out::member_local) :
        named_out_bundle(ch, type) {
      }
      void publish(std::vector<std::pair<id_type,typename named_out::scope_type> > & ids) {
        this->bind(ids);
      }
      bool publish(id_type & id, typename named_out::scope_type scope = named_out::scope_global) {
        return this->bind(id, scope);
      }
      bool unpublish(id_type & id) {
        return this->unbind(id);
      }

      //-- the following are API borrowed from dispatcher --
      template <typename user_msg_type>
      void send(id_type & id, user_msg_type *msg) {
        named_out *no = find(id);
        if (no != NULL)
          no->send(msg);
      }
      template <typename user_msg_type, typename deleter>
      void send(id_type & id, user_msg_type *msg, deleter deler) {
        named_out *no = find(id);
        if (no != NULL)
          no->send(msg,deler);
      }
      template <typename user_msg_type>
      void send(id_type & id, boost::shared_ptr<user_msg_type> msg) {
        named_out *no = find(id);
        if (no != NULL)
          no->send(msg);
      }
    };

    template <
      typename name_space,
      typename dispatcher_recver_type
      >
    class subscriber: public named_in_bundle<name_space,dispatcher_recver_type> {
      typedef typename name_space::id_type id_type;
      typedef named_in_bundle<name_space,dispatcher_recver_type> named_in_bundle;
      typedef typename named_in_bundle::named_in named_in;
    public:
      template <typename recver_type>
      subscriber(name_space & ch, recver_type rc,
                 typename named_in::member_type type = named_in::member_local) :
        named_in_bundle(ch, rc, type) {
      }
      void subscribe(std::vector<std::pair<id_type,typename named_in::scope_type> > & ids) {
        this->bind(ids);
      }
      bool subscribe(id_type & id, typename named_in::scope_type scope = named_in::scope_global) {
        return this->bind(id,scope);
      }
      bool unsubscribe(id_type & id) {
        return this->unbind(id);
      }
    };
  }
}

#endif

