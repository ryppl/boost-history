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

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using string as id_type
typedef str_path_id<'/'> id_type;
typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  abstract_executor, //force in place execution
  hierarchical_name_space<id_type, abstract_executor, mt_synch<boost_platform> >
///broadcast dispatcher by default
  > chat_chan;

//sample subject ids in chat_chan
//sports
id_type basketball = "/sports/basketball";
id_type tennis = "/sports/tennis";
id_type baseball = "/sports/baseball";
id_type all_sports = "/sports/*";
//financial
id_type tax = "/financial/tax";
id_type stock = "/financial/stock";
id_type investment = "/financial/investment/";
id_type all_financial = "/financial/*";

//a simple struct for chat msg
struct chat_msg {
  string source_;
  string data_;
  chat_msg(char *s, char *d) : source_(s), data_(d) {}
  chat_msg() {} //have to define this for marshaling to work
  //serialization function for chat_msg
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar &source_ & data_;
  }
};

#endif
