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
#include <boost/thread.hpp>
#include <boost/channel/streams/asio_stream_async.hpp>
#include "./chat_defs.hpp"

//chat msg handler; print out data
void msg_handler(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:\n";
  cout << msg->data_  << endl;
}

int main(int, char **) {
  const char *my_name = "chatter 1";
  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create local channel and bind local event source/sink
    chat_chan chan;

    //create/bind-to subjects i am going to speak about
    chat_chan::out robin_o(chan, robin_s_id);
    chat_chan::out adam_o(chan, adam_s_id);
    chat_chan::out tina_o(chan, tina_s_id);

    //subscribe to subjects i am interested in listening
    chat_chan::in smith_i(chan, smith_r_id, msg_handler);
    chat_chan::in math_i(chan, math_r_id, msg_handler);

    //register chat msg type for marshaling/demarshaling
    chat_chan::text_marshaler_registry mar_reg;
    mar_reg.register_default_msg_marshaler<chat_msg>();

    //create asio connectors and connect to remote channel
    asio_connector_async connector(io_service);
    connector.async_connect("localhost", "8888", //remote channel address
	boost::bind(asio_bind_sock_chan<chat_chan, chat_chan::text_marshaler_registry>(),
		    boost::ref(chan), boost::ref(mar_reg), _1, _2, _3));
    //a separate thread to run io_service
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    char msg[1024];
    bool cont = true;
    while (cont) {
      //speak
      cout << "which id : 1-(robin,smith,M,art), 2-(adam,smith,M,math), 3-(tina,su,F,physics), 4-exit:\n";
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
	  robin_o.send(new chat_msg(my_name, msg));
	  break;
	case 2:
	  adam_o.send(new chat_msg(my_name, msg));
	  break;
	case 3:
	  tina_o.send(new chat_msg(my_name, msg));
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

    connector.shutdown();
    t.join();
  }
  catch (boost::system::error_code& e)
    {
      std::cerr << e << "\n";
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

  cout << "... exit ...\n";
  return 0;
}

