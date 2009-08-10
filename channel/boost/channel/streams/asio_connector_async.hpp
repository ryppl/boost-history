//
// asio_connector_async.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_CONNECTOR_ASYNC_HPP
#define ASIO_CONNECTOR_ASYNC_HPP

#include <string>
#include <list>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace boost {
  namespace channel {
    class asio_connector_async;

    class conn_handler_base {
    public:
      virtual boost::shared_ptr<void> handle(asio_connector_async *conn,
                                             boost::shared_ptr<tcp::socket> sock, bool active) = 0;
      virtual ~conn_handler_base() {
      }
    };
    template<typename conn_handler_type>
    class conn_handler: public conn_handler_base {
    public:
      conn_handler(conn_handler_type h) :
        handler_(h) {
      }
      boost::shared_ptr<void> handle(asio_connector_async *conn,
                                     boost::shared_ptr<tcp::socket> sock_conn, bool active) {
        return handler_(conn, sock_conn, active);
      }
    private:
      conn_handler_type handler_;
    };

    class asio_connector_async {
    public:
      //need add locks to protect these, since they are touched by both
      //io_service thread (main) and thread calling shutdown
      std::list<boost::shared_ptr<void> > streams_;
      std::list<boost::shared_ptr<tcp::acceptor> > acceptors_;

      size_t num_streams(void) {
        return streams_.size();
      }

      void close_stream(boost::shared_ptr<void> s) {
        streams_.remove(s);
      }

      void shutdown(void) {
        std::cout << "close acceptors..." << std::endl;
        for (std::list<boost::shared_ptr<tcp::acceptor> >::iterator iter0 =
               acceptors_.begin(); iter0 != acceptors_.end(); iter0++) {
          (*iter0)->close(); //close acceptors
        }
        std::cout << "close streams..." << std::endl;
        for (std::list<boost::shared_ptr<void> >::iterator iter =
               streams_.begin(); iter != streams_.end(); iter++) {
          iter->reset(); //delete streams and connections & interfaces
        }
        std::cout << "finish shutdown..." << std::endl;
      }

      asio_connector_async(boost::asio::io_service& io_service) :
        io_service_(io_service) {
      }
      ~asio_connector_async() {
        std::cout << "~asio_connector_async..." << std::endl;
      }

      //publish a local channel at specific port and accept remote conn
      template<typename sock_conn_handler>
      void async_accept(int port, sock_conn_handler hndl) {
        boost::shared_ptr<conn_handler_base> handler(new conn_handler<
                                                     sock_conn_handler> (hndl));
        tcp::endpoint endpoint(tcp::v4(), port);
        boost::shared_ptr<tcp::acceptor> acceptor(new tcp::acceptor(
                                                                    io_service_, endpoint));
        acceptors_.push_back(acceptor);
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(io_service_));
        acceptor->async_accept(*sock, boost::bind(
                                                  &asio_connector_async::handle_accept, this, acceptor, handler,
                                                  sock, boost::asio::placeholders::error));
      }

      void handle_accept(boost::shared_ptr<tcp::acceptor> acceptor,
                         boost::shared_ptr<conn_handler_base> handler, boost::shared_ptr<
                         tcp::socket> sock, const boost::system::error_code& error) {
        if (!error) {
          boost::shared_ptr<void> stream = handler->handle(this, sock, false);
          streams_.push_back(stream);
          boost::shared_ptr<tcp::socket> new_sock(
                                                  new tcp::socket(io_service_));
          acceptor->async_accept(*new_sock, boost::bind(
                                                        &asio_connector_async::handle_accept, this, acceptor,
                                                        handler, new_sock, boost::asio::placeholders::error));
        }
      }

      //asynchronously connect a local channel to a remote channel (at <host, port>)
      template<typename sock_conn_handler>
      void async_connect(std::string host, std::string port,
                         sock_conn_handler hndl) {
        tcp::resolver resolver(io_service_);
        tcp::resolver::query query(host, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::endpoint endpoint = *endpoint_iterator;

        boost::shared_ptr<tcp::socket> sock(new tcp::socket(io_service_));
        boost::shared_ptr<conn_handler_base> handler(new conn_handler<
                                                     sock_conn_handler> (hndl));
        sock->async_connect(endpoint, boost::bind(
                                                  &asio_connector_async::handle_connect, this, sock, handler,
                                                  boost::asio::placeholders::error, ++endpoint_iterator));
      }

      void handle_connect(boost::shared_ptr<tcp::socket> sock, boost::shared_ptr<
                          conn_handler_base> handler, const boost::system::error_code& error,
                          tcp::resolver::iterator endpoint_iterator) {
        if (!error) {
          boost::shared_ptr<void> stream = handler->handle(this, sock, true);
          streams_.push_back(stream);
        } else if (endpoint_iterator != tcp::resolver::iterator()) {
          sock->close();
          tcp::endpoint endpoint = *endpoint_iterator;
          sock->async_connect(endpoint, boost::bind(
                                                    &asio_connector_async::handle_connect, this, sock, handler,
                                                    boost::asio::placeholders::error, ++endpoint_iterator));
        }
      }

      //synchronously connect a local channel to a remote channel (at <host, port>)
      template<typename sock_conn_handler>
      void sync_connect(std::string host, std::string port,
			sock_conn_handler hndl) {
        tcp::resolver resolver(io_service_);
        tcp::resolver::query query(host, port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        boost::shared_ptr<tcp::socket> sock(new tcp::socket(io_service_));
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end) {
          sock->close();
          sock->connect(*endpoint_iterator++, error);
        }
        if (error)
          throw error;

        boost::shared_ptr<void> stream = hndl(this, sock, true);
        streams_.push_back(stream);
      }

    private:
      boost::asio::io_service& io_service_;
    };

  }
}

#endif

