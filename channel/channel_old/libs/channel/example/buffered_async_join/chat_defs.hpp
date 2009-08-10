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
#include <boost/channel/executors/thread_pool_executor.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using string as id_type
typedef str_path_id<'/'> id_type;
//use channel internal simple thread_pool_executor
typedef thread_pool_executor<mt_synch<boost_platform> > exec_type;
//use Philipp Henkel's threadpool library
//#include <boost/channel/executors/threadpool_executor.hpp>
//typedef threadpool_executor<> exec_type;
typedef hierarchical_name_space<id_type,
				exec_type,
				mt_synch<boost_platform> > name_space_type;
typedef pull_dispatcher<name_space_type,boost_platform,unbounded_que> dispatcher_type;
typedef dispatcher_type::choice_async choice_arbiter_t;
typedef dispatcher_type::join_async join_arbiter_t;
//chat channel type to consume mesgs
typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  exec_type,
  name_space_type,
  dispatcher_type
  > chat_chan;
//distribution channel type to distribute mesgs
typedef channel<
  id_type,
  boost_platform,
  mt_synch<boost_platform>,
  exec_type,
  name_space_type,
  broadcast_dispatcher<name_space_type, boost_platform>
> dist_chan;

//sample subject ids in chat_chan
//sports
id_type basketball = "/sports/basketball";
id_type tennis = "/sports/tennis";
id_type baseball = "/sports/baseball";
//financial
id_type tax = "/financial/tax";
id_type stock = "/financial/stock";
id_type investment = "/financial/investment/";

//a simple struct for chat msg
struct chat_msg {
  string data_;
  chat_msg(char *d) : data_(d) {}
  chat_msg() {} //have to define this for marshaling to work
  //serialization function for chat_msg
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & data_;
  }
};

struct null_event {
};

#endif
