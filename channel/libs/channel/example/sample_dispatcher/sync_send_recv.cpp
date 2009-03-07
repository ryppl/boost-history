//
// sync_send_recv.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// demo of sample buffered synchronous dispatcher: ie. recver is blocking waiting for data to arrive
//

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/channel/channel.hpp>
#include "./pull_dispatcher_sample.hpp"

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using string as id_type
typedef linear_name_space<string,
			  abstract_executor,
			  mt_synch<boost_platform> > ns_type;
typedef channel<
  string,
  boost_platform,
  mt_synch<boost_platform>,
  abstract_executor,
  ns_type,
  pull_dispatcher_sample<ns_type, boost_platform>
> evt_chan;

//event ids in name_space
//sample events for sending text
string evt_id = "_simple_event_to_send_text_";

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
};

//entry point function for recving thread; print out data
void evt_recv(evt_chan::in & recver)
{
  string id;
  boost::shared_ptr<void> p;
  while (1) {
    recver.recv(id, p);
    cout << "recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]\n";
  }
}

int main(int, char **) {
  //create event channel
  evt_chan chan;

  //bind event source to channel
  evt_chan::out sender(chan, evt_id);

  //bind event sink to channel
  evt_chan::in recver(chan, evt_id);

  //a separate thread to run recver
  boost::thread t(boost::bind(evt_recv, boost::ref(recver)));

  //main thread fire events
  char msg[1024];
  bool cont = true;
  while (cont) {
    cin.getline(msg, 1024);
    sender.send(new evt_data(msg));
  }

  cout << "... exit ...\n";
  return 0;
}

