//
// asio_stream_sync.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_STREAM_SYNC_HPP
#define ASIO_STREAM_SYNC_HPP

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/channel/peer.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/marshaler.hpp>

namespace boost {
  namespace channel {

    template<typename id_type, typename marshaler_registry>
    class asio_stream_sync: public peer_type<id_type> {
      typedef peer_type<id_type> peer_type;
      typedef message<id_type> msg_type;

      enum {
        wait_time = 2
      };

      enum thr_state {
        thr_running, thr_stopped
      };

    private:
      enum sz_len {
        size_length = 8
      };
      boost::asio::ip::tcp::socket socket_;
      boost::asio::io_service& io_service_;
      marshaler_registry &mar_reg_;
      boost::thread *thr_;
      thr_state thr_st_;

      //output data buffers
      std::string id_len_;
      std::string id_data_;
      std::string msg_len_;
      std::string msg_data_;

      //input data buffers
      char inbound_header_[size_length];
      std::vector<char> inbound_data_;
      msg_type read_msg_;

    public:
      asio_stream_sync(boost::asio::io_service& io, marshaler_registry &reg) :
        socket_(io), io_service_(io), mar_reg_(reg) {
      }

      boost::asio::ip::tcp::socket& socket() {
        return socket_;
      }

      //start reading from socket
      void start() {
        //spawn a thread reading msgs from socket and forward them to local channel
        thr_st_ = thr_running;
        thr_ = new thread(boost::bind(&asio_stream_sync::recv, this));
      }

      //send a msg to socket and to remote peer
      void send(id_type id, shared_ptr<void> msg) {
        try {
          //marshal id
          id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
          id_m->marshal(id, id_data_);
          //marshal msg payload
          msg_marshaler *m_m = mar_reg_.get_msg_marshaler(id);
          m_m->marshal(msg, msg_data_);
          // format id length.
          std::ostringstream id_len_stream;
          id_len_stream << std::setw(size_length) << std::hex
                        << id_data_.size();
          if (!id_len_stream || id_len_stream.str().size() != size_length) {
            // Something went wrong
            boost::system::error_code err(
                                          boost::asio::error::invalid_argument);
            boost::system::system_error e(err);
            boost::throw_exception(e);
          }
          id_len_ = id_len_stream.str();
          // format msg length.
          std::ostringstream msg_len_stream;
          msg_len_stream << std::setw(size_length) << std::hex
                         << msg_data_.size();
          if (!msg_len_stream || msg_len_stream.str().size() != size_length) {
            // Something went wrong
            boost::system::error_code err(
                                          boost::asio::error::invalid_argument);
            boost::system::system_error e(err);
            boost::throw_exception(e);
          }
          msg_len_ = msg_len_stream.str();

          std::vector<boost::asio::const_buffer> buffers;
          buffers.push_back(boost::asio::buffer(id_len_));
          buffers.push_back(boost::asio::buffer(id_data_));
          buffers.push_back(boost::asio::buffer(msg_len_));
          buffers.push_back(boost::asio::buffer(msg_data_));
          boost::asio::write(socket_, buffers);
        } catch (boost::system::error_code& e) {
          std::cerr << e << "\n";
          shutdown_wait();
        } catch (std::exception& e) {
          std::cerr << "Exception: " << e.what() << "\n";
          shutdown_wait();
        }
      }

      void recv(void) {
        std::cerr << "recver thread active...\n";
        try {
          while (thr_st_ == thr_running) {
            //--- read id ---
            //1. read id len
            boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
            std::istringstream
              is(std::string(inbound_header_, size_length));
            std::size_t inbound_data_size = 0;
            if (!(is >> std::hex >> inbound_data_size)) {
              // invalid id len
              boost::system::error_code err(
                                            boost::asio::error::invalid_argument);
              boost::system::system_error e(err);
              boost::throw_exception(e);
            }
            inbound_data_.resize(inbound_data_size);
            //2. read id data
            boost::asio::read(socket_, boost::asio::buffer(inbound_data_));
            std::string archive_data(&inbound_data_[0],
                                     inbound_data_.size());
            id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
            id_m->demarshal(archive_data, read_msg_.id_);
            //--- read msg ---
            //3. read msg len
            boost::asio::read(socket_, boost::asio::buffer(inbound_header_));
            std::istringstream
              is(std::string(inbound_header_, size_length));
            std::size_t inbound_data_size = 0;
            if (!(is >> std::hex >> inbound_data_size)) {
              // invalid msg len
              boost::system::error_code err(
                                            boost::asio::error::invalid_argument);
              boost::system::system_error e(err);
              boost::throw_exception(e);
            }
            inbound_data_.resize(inbound_data_size);
            //4. read msg data
            boost::asio::read(socket_, boost::asio::buffer(inbound_data_));
            std::string archive_data(&inbound_data_[0],
                                     inbound_data_.size());
            msg_marshaler *m_m = mar_reg_.get_msg_marshaler(t.id_);
            m_m->demarshal(archive_data, read_msg_.data_);

            //5. forward to local channel
            peer_send(read_msg_.id_, read_msg_.data_);
          }
        } catch (boost::system::error_code& e) {
          std::cerr << e << "\n";
          shutdown_wait();
        } catch (std::exception& e) {
          std::cerr << "Exception: " << e.what() << "\n";
          shutdown_wait();
        }

        std::cerr << "recver thread exit...\n";
      }

      void shutdown_wait() {
        if (thr_st_ == thr_running) {
          //close socket
          close();
        }
        thr_->join();
        peer_type::release();
      }

      std::pair<typename peer_type::type, std::string> get_info(void) {
        std::pair<typename peer_type::type, std::string> info;
        info.first = peer_type::remote_peer;
        info.second = ""; //should get address info from sock_
        return info;
      }

      void close(void) {
        //close socket
        socket_->close();
      }

    };

  }
}

#endif
