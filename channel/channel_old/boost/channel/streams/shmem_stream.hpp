//
// shmem_stream.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SHMEM_STREAM_HPP
#define SHMEM_STREAM_HPP

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/channel/peer.hpp>
#include <boost/channel/message.hpp>
#include <boost/channel/marshaler.hpp>

namespace boost {
  namespace channel {

    /**
     * shmem_stream : a stream based on boost::interprocess shmem message queue
     * contains a internal thread reading from shmem msg_que and forward to peer
     */
    template<typename id_type, typename marshaler_registry>
    class shmem_stream: public peer_type<id_type> {
      typedef peer_type<id_type> peer_type;
      typedef message<id_type> msg_type;

      enum {
        wait_time = 2
      };

      enum q_f {
        quit_flag = 0
      };

      enum thr_state {
        thr_running, thr_stopped
      };

    public:
      shmem_stream(bool active, const char *name, std::size_t max_num_msg,
                   std::size_t max_msg_size, marshaler_registry &reg) :
        active_role_(active), q_name_(name), mar_reg_(reg) {
        if (active) {
          //active side create msg que in shared mem
          send_q_name_ = q_name_ + "_a_2_p"; //active_to_passive
          recv_q_name_ = q_name_ + "_p_2_a"; //passive_to_active
          boost::interprocess::message_queue::remove(send_q_name_.c_str());
          boost::interprocess::message_queue::remove(recv_q_name_.c_str());
          std::cout << "Create mq: " << send_q_name_.c_str() << "\n";
          std::cout << "Create mq: " << recv_q_name_.c_str() << "\n";
          send_mq_ = new boost::interprocess::message_queue(
                                                            boost::interprocess::create_only, send_q_name_.c_str(),
                                                            max_num_msg, max_msg_size);
          recv_mq_ = new boost::interprocess::message_queue(
                                                            boost::interprocess::create_only, recv_q_name_.c_str(),
                                                            max_num_msg, max_msg_size);
        } else {
          //passive side open only
          send_q_name_ = q_name_ + "_p_2_a"; //active_to_passive
          recv_q_name_ = q_name_ + "_a_2_p"; //passive_to_active
          send_mq_ = new boost::interprocess::message_queue(
                                                            boost::interprocess::open_only, send_q_name_.c_str());
          recv_mq_ = new boost::interprocess::message_queue(
                                                            boost::interprocess::open_only, recv_q_name_.c_str());
        }
        //spawn a thread reading msgs from msg_que and forward them to peer
        thr_st_ = thr_running;
        thr_ = new thread(boost::bind(&shmem_stream::recv, this));
      }

      ~shmem_stream() {
        //shutdown thread properly
        if (thr_st_ == thr_running)
          shutdown_wait();
        delete thr_;
        delete send_mq_;
        delete recv_mq_;
        if (active_role_) {
          boost::interprocess::message_queue::remove(send_q_name_.c_str());
          boost::interprocess::message_queue::remove(recv_q_name_.c_str());
        }
      }

      //send a msg to shmem msg_que and to remote peer
      void send(id_type id, shared_ptr<void> msg) {
        id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
        id_m->marshal(id, id_data_);
        msg_marshaler *m_m = mar_reg_.get_msg_marshaler(id);
        m_m->marshal(msg, out_msg_data_);
        int id_sz = id_data_.size();
        int msg_sz = out_msg_data_.size();
        //msg que internal already has lock protection
        send_mq_->send(id_data_.c_str(), id_sz, 0);
        send_mq_->send(out_msg_data_.c_str(), msg_sz, 0);
        std::cerr << "send one msg...\n";
      }

      void recv(void) {
        std::cerr << "shmem msg_que recver thread active...\n";
        size_t recv_sz;
        unsigned int pri;
        //have to be max_msg_sz of msg_que; otherwise exception is thrown
        size_t buf_sz = recv_mq_->get_max_msg_size();
        boost::shared_ptr<char> buf(new char[buf_sz]);
        while (thr_st_ == thr_running) {
          recv_mq_->receive(buf.get(), buf_sz, recv_sz, pri);
          if (recv_sz == sizeof(size_t) && *((size_t*) buf.get())
              == quit_flag) {
            thr_st_ = thr_stopped;
            break;
          }
          std::string id_data(buf.get(), recv_sz);
          id_marshaler<id_type> *id_m = mar_reg_.get_id_marshaler();
          id_m->demarshal(id_data, read_msg_.id_);
          recv_mq_->receive(buf.get(), buf_sz, recv_sz, pri);
          if (recv_sz == sizeof(size_t) && *((size_t*) buf.get())
              == quit_flag) {
            thr_st_ = thr_stopped;
            break;
          }
          std::string msg_data(buf.get(), recv_sz);
          msg_marshaler *m_m = mar_reg_.get_msg_marshaler(read_msg_.id_);
          m_m->demarshal(msg_data, read_msg_.data_);
          peer_send(read_msg_.id_, read_msg_.data_);
        }
        std::cerr << "shmem msg_que recver thread exit...\n";
      }

      std::pair<typename peer_type::type, std::string> get_info(void) {
        std::pair<typename peer_type::type, std::string> info;
        info.first = peer_type::remote_peer;
        info.second = q_name_; //should get address info from sock_
        return info;
      }

      void shutdown_wait() {
        size_t quit_msg = quit_flag;
        if (thr_st_ == thr_running) {
          //msg que internal already has lock protection
          recv_mq_->send(&quit_msg, sizeof(quit_msg), 0);
        }
        thr_->join();
        peer_type::release();
      }

    private:
      bool active_role_;
      std::string q_name_;
      std::string send_q_name_;
      std::string recv_q_name_;
      boost::interprocess::message_queue *send_mq_;
      boost::interprocess::message_queue *recv_mq_;
      marshaler_registry &mar_reg_;
      boost::thread *thr_;
      thr_state thr_st_;

      //output data buffers
      std::string id_data_;
      std::string out_msg_data_;

      //input data buffers
      std::vector<char> msg_data_;
      msg_type read_msg_;
    };

  }
}

#endif

