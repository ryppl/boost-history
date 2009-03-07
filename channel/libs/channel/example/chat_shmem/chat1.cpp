//
// chat1.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/channel/streams/shmem_stream.hpp>
#include "./chat_defs.hpp"

//chat msg handler; print out data
void msg_handler(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:" << endl;
  cout << msg->data_  << endl;
}

int main(int, char **) {
  char *my_name = "chatter 1";
  //create local channel and bind local event source/sink
  chat_chan chan;

  //create/bind-to subjects i am going to speak about
  chat_chan::out basketball_o(chan, basketball);
  chat_chan::out tennis_o(chan, tennis);
  chat_chan::out stock_o(chan, stock);

  //subscribe to subjects i am interested in listening
  chat_chan::in basketball_i(chan, basketball, msg_handler);
  //i want hear all about financials
  chat_chan::in all_financial_i(chan, all_financial, msg_handler);

  //register chat msg type for marshaling/demarshaling
  chat_chan::text_marshaler_registry mar_reg;
  mar_reg.register_default_msg_marshaler<chat_msg>();

  //create shmem stream and connect to channel
  //let the other side (chat2) create shmem, we jus open and use it
  shmem_stream<
  id_type,
    chat_chan::text_marshaler_registry
    >
    shmstream(false, que_name, que_max_num, que_max_size, mar_reg);

  //as active connection side, so we start connection handshaking
  //inside shmem_stream, an internal thread will pump messages
  connect(chan, &shmstream, true);

  //in the following code, main thread will drive the interactive loop
  char msg[1024];
  bool cont = true;
  while (cont) {
    //speak
    cout << "which subject : 1-basket_ball, 2-tennis, 3-stock, 4-exit:\n";
    int subject;
    cin.getline(msg, 1024);
    subject = atoi(msg);
    switch(subject) {
    case 1:
    case 2:
    case 3:
      cout << "enter your message: ";
      cin.getline(msg, 1024);
      switch (subject) {
      case 1:
	basketball_o.send(new chat_msg(my_name, msg));
	break;
      case 2:
	tennis_o.send(new chat_msg(my_name, msg));
	break;
      case 3:
	stock_o.send(new chat_msg(my_name, msg));
	break;
      default:
	break;
      }
      break;
    case 4:
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

