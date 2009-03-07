//
// pingpong_defs.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PINGPONG_DEFS_HPP
#define PINGPONG_DEFS_HPP

#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type
typedef channel<int> msg_chan;

//sample msg ids
int msg_id_base = msg_chan::app_msg_begin;
int msg_id_ping = msg_id_base+1;
int msg_id_pong = msg_id_ping+1;

//a simple struct for ping-pong msg
struct pingpong_msg {
  int count_;
  char dd[1024*1024]; //for checking mem-leak during msg passing
  pingpong_msg(int cnt) : count_(cnt) {}
  pingpong_msg() {} //have to define this for marshaling to work
  //serialization function for pingpong_msg
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & count_;
  }
};

#endif
