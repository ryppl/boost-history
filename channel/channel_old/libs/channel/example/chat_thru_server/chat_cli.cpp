//
// chat_cli.cpp
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

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << "Usage: chat_cli chatter_name srv_name srv_port\n";
    exit(0);
  }
  const char *my_name = argv[1];
  const char *srv_name = argv[2];
  const char *srv_port = argv[3];
  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create asio_executor and run callbacks in asio main thread
    asio_executor asio_exec(io_service);
    //create local channel
    chat_chan chan(&asio_exec);

    //register chat msg type for marshaling/demarshaling
    chat_chan::text_marshaler_registry mar_reg;
    mar_reg.register_default_msg_marshaler<chat_msg>();

    //create asio connectors and connect to chat server
    asio_connector_async connector(io_service);
    connector.async_connect(srv_name, srv_port,
	boost::bind(asio_bind_sock_chan<chat_chan, chat_chan::text_marshaler_registry>(),
		    boost::ref(chan), boost::ref(mar_reg), _1, _2, _3));

    //a separate thread to run io_service
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    //in main thread, we pub/sub subjects and send/recv msgs
    chat_chan::publisher pub(chan);
    chat_chan::subscriber sub(chan, msg_handler);

    bool cont = true;
    char buf[1024];
    while (cont) {
      cout << "action : 1-join, 2-leave, 3-send, 4-exit:\n";
      int action;
      cin.getline(buf, 1024);
      action = atoi(buf);
      switch (action) {
      case 1:
      case 2:
      case 3:
	{
	cout << "subject: ";
	cin.getline(buf, 1024);
	id_type id(buf);
	switch (action) {
	case 1:
	  pub.publish(id);
	  sub.subscribe(id);
	  break;
	case 2:
	  pub.unpublish(id);
	  sub.unsubscribe(id);
	  break;
	case 3:
	  cout << "message: ";
	  cin.getline(buf, 1024);
	  pub.send(id, new chat_msg(my_name,buf));
	  break;
	default:
	  break;
	}
	break;
	}
      case 4:
	cont = false;
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

