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
#include <boost/regex.hpp>
#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using regex_id as id_type
typedef regex_id id_type;
typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  abstract_executor, //force in place execution
  linear_name_space<id_type, abstract_executor, mt_synch<boost_platform>, false> ///false - assoc matching
///broadcast dispatcher by default
  > chat_chan;

//define sys ids
DEFINE_REGEX_SYS_IDS

//sample subject ids in chat_chan
//sports
id_type basketball = "[sports]: basketball";
id_type tennis = "[sports]: tennis";
id_type baseball = "[sports]: baseball";
id_type all_balls(new boost::regex(".*ball.*"));
//financial
id_type tax = "[financial]: tax";
id_type stock = "[financial]: stock";
id_type investment = "[financial]: investment";
id_type all_financial(new boost::regex(".*financial.*"));

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
