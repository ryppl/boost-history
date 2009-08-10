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
#include <boost/thread.hpp>
#include <boost/channel/streams/asio_stream_async.hpp>
#include "./chat_defs.hpp"

//chat msg handler; print out data
void msg_handler(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "chan1: " << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:\n";
  cout << msg->data_  << endl;
}

void msg_handler21(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "chan2.1: " << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:\n";
  cout << msg->data_  << endl;
}

void msg_handler22(id_type id, boost::shared_ptr<void> p)
{
  chat_msg *msg = (chat_msg *) p.get();
  cout << "chan2.2: " << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:\n";
  cout << msg->data_  << endl;
}

/**
 * here we are going to use the publish/subscribe API built on top of
 * named_in/named_out
 */

int main(int, char **) {
  const char* my_name = "chatter 2";
  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create local channels and connect them
    chat_chan chan;
    chat_chan chan2;
    connect(chan, chan2);

    //create/bind-to subjects i am going to speak about
    chat_chan::publisher pub(chan);
    pub.publish(adam_s_id);
    pub.publish(robin_s_id);
    pub.publish(tina_s_id);

    //subscribe to subjects i am interested in listening
    chat_chan::subscriber sub(chan, msg_handler);
    //i am a sports fan
    sub.subscribe(female_r_id);
    sub.subscribe(robin_s_id);

    //subscribe at chan2, using named_in/out api
    chat_chan::in smith_i(chan2, smith_r_id, msg_handler21);
    chat_chan::in math_i(chan2, math_r_id, msg_handler22);

    //register chat msg type for marshaling/demarshaling
    chat_chan::text_marshaler_registry mar_reg;
    mar_reg.register_default_msg_marshaler<chat_msg>();

    //create asio connectors and connect to remote channel
    asio_connector_async connector(io_service);
    connector.async_accept(8888, // channel address
	boost::bind(asio_bind_sock_chan<chat_chan, chat_chan::text_marshaler_registry>(),
		    boost::ref(chan), boost::ref(mar_reg), _1, _2, _3));
    //a separate thread to run io_service
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    //in the following code, main thread will drive the interactive loop
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
	cin.getline(msg,1024);
	switch (subject) {
	case 1:
	  pub.send(robin_s_id, new chat_msg(my_name, msg));
	  break;
	case 2:
	  pub.send(adam_s_id, new chat_msg(my_name, msg));
	  break;
	case 3:
	  pub.send(tina_s_id, new chat_msg(my_name, msg));
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

