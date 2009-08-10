//
// evt_chan_defs.hpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef EVT_CHAN_DEFS_HPP
#define EVT_CHAN_DEFS_HPP

#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using string as id_type
//and execute all async operations in asio executor
typedef channel<string,
		boost_platform,
		mt_synch<boost_platform>,
		asio_executor> evt_chan;

//event ids in name_space
//sample events for a gui window
string down_id = "_window_button_down_";
string up_id = "_window_button_up_";
string close_id = "_window_close_";

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
  evt_data() {} //have to define this for marshaling to work
  //serialization function for evt_data
  template <typename Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & data_;
  }
};

#endif
