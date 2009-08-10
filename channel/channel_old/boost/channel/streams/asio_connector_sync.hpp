//
// asio_connector_sync.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_CONNECTOR_SYNC_HPP
#define ASIO_CONNECTOR_SYNC_HPP

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/channel/streams/asio_stream_sync.hpp>

using boost::asio::ip::tcp;

namespace boost {
  namespace channel {

    class asio_connector_sync {
    private:
      boost::asio::io_service& io_service_;
    public:
      asio_connector_sync(boost::asio::io_service& io_service) :
        io_service_(io_service) {
      }

      void shutdown(void) {
      }

      //synchronously connect a local channel to a remote channel (at <host, port>)
      asio_stream_sync* connect(std::string host, std::string port) {
        tcp::resolver resolver(io_service_);
        tcp::resolver::query query(host, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        asio_stream_sync *stream = new asio_stream_sync(io_service_, reg);
        boost::asio::error error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end) {
          stream->socket().close();
          stream->socket().connect(*endpoint_iterator++,
                                   boost::asio::assign_error(error));
        }
        if (error)
          throw error;
        stream->start();
        return stream;
      }

      //synchronously accept:
      //? how to continuously wait
      //? how to wait on multi ports for multi channels
      asio_stream_sync* accept(int port) {
        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
        asio_stream_sync *stream = new asio_stream_sync(io_service_, reg);
        a.accept(stream->socket());
        stream->start();
        return stream;
      }
    };

  }
}

#endif
