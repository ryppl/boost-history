//
// asio_stream_async.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_STREAM_ASYNC_HPP
#define ASIO_STREAM_ASYNC_HPP

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/channel/peer.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/marshaler.hpp>
#include <boost/channel/streams/asio_connector_async.hpp>

namespace boost {
  namespace channel {

    /**
     * asio_stream_async: a wrapper of socket, implement peer_type interface to be
     *                connected to channels
     * msg serialization/deserialization is also done here: there are 2 parts:
     * 1. serialization of ids: in id_trait files, for each id_type,
     *                          a serialization function is defined
     * 2. serialization of msg data: inside channel and among local peers, messages
     *        are passed by pointers; only when writing into and reading from
     *        external connections (such as sockets), serialization/deserialization
     *        is necessary. So all serialization is localized in "streams":
     *           diff ids could have diff msgs data structures, use
     *           marshaler_registry (a map or hash table to map
     *           ids to marshalers (the serialization functions)
     */

    void asio_stream_async_do_close(boost::shared_ptr<tcp::socket> sock) {
      //close socket; called from io_service main thread
      std::cout << "asio_stream_async_do_close enter" << std::endl;
      sock->close();
      std::cout << "asio_stream_async_do_close exit" << std::endl;
    }

    template<typename id_type, typename marshaler_registry>
    class asio_stream_async: public peer_type<id_type> {
      typedef peer_type<id_type> peer_type;
      typedef message<id_type> msg_type;
      typedef std::deque<msg_type> msg_que_type;

      enum stream_state {
        connecting, connected, disconnected
      };

    public:
      asio_stream_async(asio_connector_async *conn, boost::shared_ptr<
			boost::asio::ip::tcp::socket> sock, marshaler_registry &reg) :
        socket_(sock), io_service_(sock->io_service()), mar_reg_(reg), conn_(
                                                                             conn), state_(connected) {
      }

      ~asio_stream_async() {
        std::cout << "asio_stream_async destructor enter..." << std::endl;
        if (state_ == connected) {
          state_ = disconnected;
          peer_type::release();
          io_service_.post(boost::bind(&asio_stream_async_do_close, socket_));
        }
        std::cout << "asio_stream_async destructor exit..." << std::endl;
      }

      //start reading from socket
      void start() {
        async_read(read_msg_, boost::bind(&asio_stream_async::handle_read,
                                          this, boost::asio::placeholders::error));
      }

      //send a msg to socket and to remote peer
      void send(id_type id, shared_ptr<void> msg) {
        msg_type mdata(id, msg);
        io_service_.post(boost::bind(&asio_stream_async::do_write, this, mdata));
      }

      std::pair<typename peer_type::type, std::string> get_info(void) {
        std::pair<typename peer_type::type, std::string> info;
        info.first = peer_type::remote_peer;
        info.second = ""; //should get address info from sock_
        return info;
      }

      void close() {
        if (state_ == connected) {
          state_ = disconnected;
          peer_type::release();
          io_service_.post(boost::bind(&asio_stream_async_do_close, socket_));
        }
      }

    private:

      void do_write(msg_type msg) {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
          async_write(write_msgs_.front(), boost::bind(
                                                       &asio_stream_async::handle_write, this,
                                                       boost::asio::placeholders::error));
        }
      }

      template<typename handler_type>
      void async_write(msg_type &m, handler_type handler) {
        //marshal id
        id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
        id_m->marshal(m.id_, id_data_);
        //marshal msg payload
        msg_marshaler *m_m = mar_reg_.get_msg_marshaler(m.id_);
        m_m->marshal(m.data_, msg_data_);
        // format id length.
        std::ostringstream id_len_stream;
        id_len_stream << std::setw(size_length) << std::hex << id_data_.size();
        if (!id_len_stream || id_len_stream.str().size() != size_length) {
          // Something went wrong, inform the caller.
          boost::system::error_code error(
                                          boost::asio::error::invalid_argument);
          io_service_.post(boost::bind(handler, error));
          return;
        }
        id_len_ = id_len_stream.str();
        // format msg length.
        std::ostringstream msg_len_stream;
        msg_len_stream << std::setw(size_length) << std::hex
                       << msg_data_.size();
        if (!msg_len_stream || msg_len_stream.str().size() != size_length) {
          // Something went wrong, inform the caller.
          boost::system::error_code error(
                                          boost::asio::error::invalid_argument);
          io_service_.post(boost::bind(handler, error));
          return;
        }
        msg_len_ = msg_len_stream.str();

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(id_len_));
        buffers.push_back(boost::asio::buffer(id_data_));
        buffers.push_back(boost::asio::buffer(msg_len_));
        buffers.push_back(boost::asio::buffer(msg_data_));
        boost::asio::async_write(*socket_, buffers, handler);
      }

      void handle_write(const boost::system::error_code& error) {
        if (!error) {
          write_msgs_.pop_front();
          if (!write_msgs_.empty()) {
            async_write(write_msgs_.front(), boost::bind(
                                                         &asio_stream_async::handle_write, this,
                                                         boost::asio::placeholders::error));
          }
        } else {
          close();
          //conn_->close_stream(boost::shared_ptr<void>(this));
        }
      }

      template<typename handler_type>
      void async_read(msg_type &read_msg, handler_type handler) {
        // read id length header.
        void (asio_stream_async::*f)(const boost::system::error_code&,
                                     msg_type&, boost::tuple<handler_type>)
          = &asio_stream_async::handle_read_id_len;
        boost::asio::async_read(*socket_, boost::asio::buffer(inbound_header_),
				boost::bind(f, this, boost::asio::placeholders::error,
                                            boost::ref(read_msg), boost::make_tuple(handler)));
      }

      template<typename handler_type>
      void handle_read_id_len(const boost::system::error_code& e, msg_type& t,
                              boost::tuple<handler_type> handler) {
        if (e) {
          boost::get<0>(handler)(e);
        } else {
          // Determine id length
          std::istringstream is(std::string(inbound_header_, size_length));
          std::size_t inbound_data_size = 0;
          if (!(is >> std::hex >> inbound_data_size)) {
            // Header doesn't seem to be valid. Inform the caller.
            boost::system::error_code error(
                                            boost::asio::error::invalid_argument);
            boost::get<0>(handler)(error);
            return;
          }

          // Start an asynchronous call to read id data.
          inbound_data_.resize(inbound_data_size);
          void (asio_stream_async::*f)(const boost::system::error_code&,
                                       msg_type&, boost::tuple<handler_type>)
            = &asio_stream_async::handle_read_id_data;
          boost::asio::async_read(*socket_,
                                  boost::asio::buffer(inbound_data_), boost::bind(f, this,
                                                                                  boost::asio::placeholders::error, boost::ref(t),
                                                                                  handler));
        }
      }

      template<typename handler_type>
      void handle_read_id_data(const boost::system::error_code& e, msg_type& t,
                               boost::tuple<handler_type> handler) {
        if (e) {
          boost::get<0>(handler)(e);
        } else {
          // Extract the data structure from the data just received.
          try {
            std::string archive_data(&inbound_data_[0],
                                     inbound_data_.size());
            id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
            id_m->demarshal(archive_data, t.id_);
          } catch (std::exception& e) {
            // Unable to decode data.
            boost::system::error_code error(
                                            boost::asio::error::invalid_argument);
            boost::get<0>(handler)(error);
            return;
          }

          // read msg length header.
          void (asio_stream_async::*f)(const boost::system::error_code&,
                                       msg_type&, boost::tuple<handler_type>)
            = &asio_stream_async::handle_read_msg_len;
          boost::asio::async_read(*socket_, boost::asio::buffer(
                                                                inbound_header_), boost::bind(f, this,
                                                                                              boost::asio::placeholders::error, boost::ref(t), handler));

        }
      }

      template<typename handler_type>
      void handle_read_msg_len(const boost::system::error_code& e, msg_type& t,
                               boost::tuple<handler_type> handler) {
        if (e) {
          boost::get<0>(handler)(e);
        } else {
          // Determine id length
          std::istringstream is(std::string(inbound_header_, size_length));
          std::size_t inbound_data_size = 0;
          if (!(is >> std::hex >> inbound_data_size)) {
            // Header doesn't seem to be valid. Inform the caller.
            boost::system::error_code error(
                                            boost::asio::error::invalid_argument);
            boost::get<0>(handler)(error);
            return;
          }

          // Start an asynchronous call to read id data.
          inbound_data_.resize(inbound_data_size);
          void (asio_stream_async::*f)(const boost::system::error_code&,
                                       msg_type&, boost::tuple<handler_type>)
            = &asio_stream_async::handle_read_msg_data;
          boost::asio::async_read(*socket_,
                                  boost::asio::buffer(inbound_data_), boost::bind(f, this,
                                                                                  boost::asio::placeholders::error, boost::ref(t),
                                                                                  handler));
        }
      }

      template<typename handler_type>
      void handle_read_msg_data(const boost::system::error_code& e, msg_type& t,
                                boost::tuple<handler_type> handler) {
        if (e) {
          boost::get<0>(handler)(e);
        } else {
          // Extract the data structure from the data just received.
          try {
            std::string archive_data(&inbound_data_[0],
                                     inbound_data_.size());
            msg_marshaler *m_m = mar_reg_.get_msg_marshaler(t.id_);
            m_m->demarshal(archive_data, t.data_);
          } catch (std::exception& e) {
            // Unable to decode data.
            boost::system::error_code error(
                                            boost::asio::error::invalid_argument);
            boost::get<0>(handler)(error);
            return;
          }

          // Inform caller that data has been received ok.
          boost::get<0>(handler)(e);
        }
      }

      //finish reading a complete msg, forward to peer and read next one
      void handle_read(const boost::system::error_code& error) {
        if (!error) {
          peer_send(read_msg_.id_, read_msg_.data_);
          async_read(read_msg_, boost::bind(&asio_stream_async::handle_read,
                                            this, boost::asio::placeholders::error));
        } else {
          close();
          //conn_->close_stream(boost::shared_ptr<void>(this));
        }
      }

      /*
        void do_close(boost::shared_ptr<tcp::socket> sock)
        {
        //close socket; called from io_service main thread
        sock->close();
        std::cout << "do_close called" << std::endl;
        }
      */

      boost::shared_ptr<boost::asio::ip::tcp::socket> socket(void) {
        return socket_;
      }

    private:
      enum sz_len {
        size_length = 8
      };
      boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
      boost::asio::io_service& io_service_;
      marshaler_registry &mar_reg_;
      asio_connector_async *conn_;
      stream_state state_;

      //output data buffers
      msg_que_type write_msgs_;
      std::string id_len_;
      std::string id_data_;
      std::string msg_len_;
      std::string msg_data_;

      //input data buffers
      char inbound_header_[size_length];
      std::vector<char> inbound_data_;
      msg_type read_msg_;
    };

    template<typename channel, typename marshaler_registry>
    struct asio_bind_sock_chan {
      typedef boost::shared_ptr<void> result_type;
      typename channel::binder_type *binder_;
      typedef boost::function1<typename channel::binder_type *,
                               boost::shared_ptr<tcp::socket> > call_back_t;
      call_back_t binder_gen_;

      asio_bind_sock_chan(call_back_t cb = 0) :
        binder_(NULL), binder_gen_(cb) {
      }
      boost::shared_ptr<void> operator()(channel &chan, marshaler_registry &reg,
                                         asio_connector_async *conn, boost::shared_ptr<tcp::socket> sock,
                                         bool active) {
        typedef	typename channel::id_type id_type;
        typedef asio_stream_async<id_type, marshaler_registry> asio_stream;
        asio_stream *stream = new asio_stream(conn, sock, reg);
        if (binder_gen_)
          binder_ = binder_gen_(sock);
        //should connect first, so that when stream starts(reading),
        //peer channel is already connected and ready to forward msgs
        connect(chan, stream, active,binder_);
        stream->start(); //start reading from sock
        return boost::shared_ptr<void>(stream);
      }
    };

  }
}

#endif

