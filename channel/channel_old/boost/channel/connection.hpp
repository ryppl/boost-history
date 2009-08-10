//
// connection.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <set>
#include <boost/channel/peer.hpp>

namespace boost {
  namespace channel {

    template<typename id_type>
    class connection {
    public:
      typedef peer_type<id_type> peer_type;
      enum type {
        local_connection = 0, remote_connection
      };
      type type_;
      peer_type *peer1_, *peer2_;
      connection(type t, peer_type *peer1, peer_type *peer2) :
        type_(t), peer1_(peer1), peer2_(peer2) {
      }

      ~connection() {
        peer1_->unbind();
        peer2_->unbind();
        delete peer1_;
        if (type_ == local_connection)
          delete peer2_; //should we delete stream for remote_conn?
      }
    };
  }
}

#endif
