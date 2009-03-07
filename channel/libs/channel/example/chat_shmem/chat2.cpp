//
// chat2.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "./chat_defs.hpp"
#include <boost/channel/streams/shmem_stream.hpp>

//chat msg handler; print out data
void msg_handler(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "chan1: " << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:" << endl;
  cout << msg->data_  << endl;
}

void msg_handler2(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "chan2: " << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:" << endl;
  cout << msg->data_  << endl;
}

/**
 * here we are going to use the publish/subscribe API built on top of
 * named_in/named_out
 */

int main(int, char **) {
  char* my_name = "chatter 2";
  //create local channels and connect them
  chat_chan chan;
  chat_chan chan2;
  connect(chan, chan2);

  //create/bind-to subjects i am going to speak about
  chat_chan::publisher pub(chan);
  pub.publish(basketball);
  pub.publish(baseball);
  pub.publish(tax);
  pub.publish(investment);

  //subscribe to subjects i am interested in listening
  chat_chan::subscriber sub(chan, msg_handler);
  //i am a sports fan
  sub.subscribe(all_sports);
  sub.subscribe(stock);
  sub.subscribe(tax);

  //subscribe at chan2, using named_in/out api
  chat_chan::in basketball_i(chan2, basketball, msg_handler2);
  chat_chan::in all_financial_i(chan2, all_financial, msg_handler2);

  //register chat msg type for marshaling/demarshaling
  chat_chan::text_marshaler_registry mar_reg;
  mar_reg.register_default_msg_marshaler<chat_msg>();

  //create shmem stream and connect to channel
  //we'll create shmem msg_que and allow chat1 to connect
  shmem_stream<
  id_type,
    chat_chan::text_marshaler_registry
    >
    shmstream(true, que_name, que_max_num, que_max_size, mar_reg);

  //as passive conn side, we wait for chat1 to start connection handshaking
  //inside shmem_stream, an internal thread will pump messages
  connect(chan, &shmstream, false);

  //in the following code, main thread will drive the interactive loop
  char msg[1024];
  bool cont = true;
  while (cont) {
    //speak
    cout << "which subject : 1-basket_ball, 2-base_ball, 3-tax, 4-investment, 5-exit:\n";
    int subject;
    cin.getline(msg, 1024);
    subject = atoi(msg);
    switch(subject) {
    case 1:
    case 2:
    case 3:
    case 4:
      cout << "enter your message: ";
      cin.getline(msg,1024);
      switch (subject) {
      case 1:
	pub.send(basketball, new chat_msg(my_name, msg));
	break;
      case 2:
	pub.send(baseball, new chat_msg(my_name, msg));
	break;
      case 3:
	pub.send(tax, new chat_msg(my_name, msg));
	break;
      case 4:
	pub.send(investment, new chat_msg(my_name, msg));
	break;
      default:
	break;
      }
      break;
    case 5:
      cont = false;
      break;
    default:
      break;
    }
  }

  shmstream.shutdown_wait();

  cout << "... exit ...\n";
  return 0;
}

