//
// sync_send_recv.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// demo of buffered synchronous dispatcher: ie. recver is blocking waiting for data to arrive
//

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using string as id_type
typedef pull_dispatcher<linear_name_space<string, abstract_executor, mt_synch<boost_platform> >,
			boost_platform> dispatcher_type;
typedef channel<
  string,
  boost_platform,
  mt_synch<boost_platform>,
  abstract_executor,
  linear_name_space<string, abstract_executor, mt_synch<boost_platform> >,
  dispatcher_type
> evt_chan;
typedef dispatcher_type::choice_sync choice_type;

//event ids in name_space
//sample events for sending text
string evt_id1 = "_1st_event_to_send_text_";
string evt_id2 = "_2nd_event_to_send_text_";

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
};

//entry point function for recving thread; print out data
void evt_recv(choice_type & recver)
{
  string id;
  boost::shared_ptr<void> p;
  while (1) {
    recver.recv(id, p);
    cout << "recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
}

int main(int, char **) {
  //create event channel
  evt_chan chan;

  //bind event source to channel
  evt_chan::out sender1(chan, evt_id1);
  evt_chan::out sender2(chan, evt_id2);

  //bind event sink to channel
  choice_type choice(chan);
  choice.bind(evt_id1);
  choice.bind(evt_id2);

  //a separate thread to run recver
  boost::thread t(boost::bind(evt_recv, boost::ref(choice)));

  //main thread fire events
  char msg[1024];
  for(int i=0;;i++) {
    cin.getline(msg, 1024);
    if(i % 2 == 0)
      sender1.send(new evt_data(msg));
    else
      sender2.send(new evt_data(msg));
  }

  cout << "... exit ...\n";
  return 0;
}

