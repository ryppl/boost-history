//
// message.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/array.hpp>

namespace boost {
  namespace channel {

    template<typename id_type>
    struct message {
      id_type id_;
      boost::shared_ptr<void> data_;
      message(id_type id, boost::shared_ptr<void> d) :
        id_(id), data_(d) {
      }
      message() {
      }
    };

    /**
     * block messages: for passing a continuous memory blocks as messages, such as
     *                 data classes marshaled as bindary objects (void *) and arrays
     */
    template<typename T>
    struct binary_message {
      size_t size_;
      boost::shared_ptr<void> data_;
      binary_message() :
        size_(0), data_() {
      }
      binary_message(T *d, size_t s) :
        size_(s), data_(d) {
      }
      binary_message(boost::shared_ptr<void> d, size_t s) :
        size_(s), data_(d) {
      }
      ~binary_message() {
      } //shared_ptr should clean up data_
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const {
        ar & size_;
        ar & boost::serialization::make_binary_object(data_.get(), size_);
      }
      template<class Archive>
      void load(Archive & ar, const unsigned int version) {
        ar & size_;
        data_ = new char[size_];
        ar & boost::serialization::make_binary_object(data_.get(), size_);
      }
    };

    template<typename T>
    struct array_message {
      size_t size_;
      boost::shared_array<T> data_;
      array_message() :
        size_(0), data_() {
      }
      array_message(T *d, size_t s) :
        size_(s), data_(d) {
      }
      array_message(boost::shared_array<T> d, size_t s) :
        size_(s), data_(d) {
      }
      ~array_message() {
      } //shared_ptr should clean up data_
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const {
        ar & size_;
        ar & boost::serialization::make_array<T>(data_.get(), size_);
      }
      template<class Archive>
      void load(Archive & ar, const unsigned int version) {
        ar & size_;
        data_ = new T[size_];
        ar & boost::serialization::make_array<T>(data_.get(), size_);
      }
    };

    /**
     * Channel Management Msg types
     */
    ///when 2 channels connects, channel_info_msg is sent to tell
    ///peers about the channel info
    struct channel_info_msg_t {
      std::string host_addr;
      void *intf; ///the interface connect to peer
      bool is_local;
      channel_info_msg_t() {
        intf = NULL;
        is_local = false;
      }

      template<class Archive>
      void serialize(Archive & ar, const unsigned int version) {
        ar & host_addr;
      }

    };

    ///pubsub_info_msg is used to notify peer channels about
    ///publish/subscribe events
    template<class id_type>
    struct pubsub_info_msg_t {
      std::vector<id_type> msg_types;
      pubsub_info_msg_t() {
      }

      template<class Archive>
      void serialize(Archive & ar, const unsigned int version) {
        ar & msg_types;
      }

    };
  }
}

#endif
