//
// chat_defs.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef CHAT_DEFS_HPP
#define CHAT_DEFS_HPP

#include <string>
#include <boost/channel/channel.hpp>
#include <boost/channel/executors/asio_executor.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type
typedef string id_type;
typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  asio_executor //asynchronous channel
  > chat_chan;

class filter_t : public filter_type<string> {
public:
  string prefix;
  filter_t(string pre) : prefix(pre) {}
  bool block_inward (string &id) {
    if (id.find(prefix) != 0) {
      cout << "id [" << id << "] blocked at inward<" << prefix << ">" << endl;
      return true;
    }
    return false;
  }
  bool block_outward (string &id) {
    if (id.find(prefix) != 0) {
      cout << "id [" << id << "] blocked at outward<" << prefix << ">" << endl;
      return true;
    }
    return false;
  }
};

class translator_t : public translator_type<string> {
public:
  string prefix;
  translator_t(string pre) : prefix(pre) {}
  void translate_inward (string & id) {
    cout << "translate_inward : id[" << id << "] into [" ;
    id = prefix + id;
    cout << id <<  "]\n";
  }
  void translate_outward (string & id) {
    cout << "translate_outward : id[" << id << "] into [" ;
    id.erase(0,prefix.size());
    cout << id << "]\n";
  }
};

class binder : public binder_type<string> {
public:
  binder(string prefix) {
    filter = new filter_t(prefix);
    translator = new translator_t(prefix);
  }
};

//a simple struct for chat msg
struct chat_msg {
  string source_;
  string data_;
  chat_msg(const char *s, const char *d) : source_(s), data_(d) {}
  chat_msg() {} //have to define this for marshaling to work
  //serialization function for chat_msg
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar &source_ & data_;
  }
};

#endif
