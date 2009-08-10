//
// peer.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PEER_HPP
#define PEER_HPP

#include <string>
#include <boost/shared_ptr.hpp>

namespace boost {
  namespace channel {

    template<typename > class connection;

    template<typename id_type>
    class peer_type {
    public:
      typedef connection<id_type> connection;
      enum type {
        local_peer = 0, remote_peer
      };
      enum role {
        undefined_role, active_role, //i start connection setup process
        passive_role,
        //i wait for others to connect
      };

      void bind_peer(peer_type *p, role r, connection *c) {
        peer_ = p;
        role_ = r;
        conn_ = c;
      }
      virtual void unbind() {
        peer_ = 0;
        role_ = undefined_role;
        conn_ = 0;
      }
      //channel or stream should call release to disconn and cleanup
      void release(void) {
        if (conn_ != NULL)
          delete conn_; //connection destructor responsible for deleting all
      }

      peer_type() :
        peer_(0), role_(undefined_role), conn_(0) {
      }
      virtual ~peer_type() {
      }

      std::pair<type, std::string> peer_get_info(void) {
        return peer_->get_info();
      }
      void peer_send(id_type id, shared_ptr<void> msg) {
        peer_->send(id, msg);
      }

      //stream should override get_info to provide transport info (e.g. host/port..)
      virtual std::pair<type, std::string> get_info(void) {
        std::pair<type, std::string> info;
        info.first = local_peer;
        info.second = "";
        return info;
      }

      //concrete peer types (streams, interface) should override this
      virtual void send(id_type id, shared_ptr<void> msg) = 0;

      peer_type *peer_;
      role role_;
      connection *conn_;
    };

  }
}

#endif
