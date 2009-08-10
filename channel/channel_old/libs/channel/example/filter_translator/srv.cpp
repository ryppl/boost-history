//
// srv.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/channel/streams/asio_stream_async.hpp>
#include "./chat_defs.hpp"

//chat srv handler: update name space based clients' publications and subscriptions
//and forward chat msgs
class msg_handler {
public:
  chat_chan::publisher pub;
  chat_chan::subscriber sub;
  msg_handler(chat_chan &chan) :
    pub(chan),
    sub(chan,boost::bind(&msg_handler::handling, this, _1,_2))
  {
    //subscribe to name space change events
    sub.subscribe(chat_chan::publication_info_msg, chat_chan::in::scope_remote);
  }
  void handling(id_type id, boost::shared_ptr<void> p)
  {
    typedef pubsub_info_msg_t<id_type> pubsub_info_msg_t;
    pubsub_info_msg_t *pubsub_info = NULL;
    if(id == chat_chan::publication_info_msg) {
      pubsub_info = (pubsub_info_msg_t *) p.get();
      for(std::vector<id_type>::iterator iter = pubsub_info->msg_types.begin();
	  iter != pubsub_info->msg_types.end(); iter++) {
	cout << "server pub/sub to " << (*iter) << endl;
	pub.publish((*iter), chat_chan::in::scope_remote);
	sub.subscribe((*iter), chat_chan::in::scope_remote);
      }
    }
    else if(id == chat_chan::unpublication_info_msg) {
      pubsub_info = (pubsub_info_msg_t *) p.get();
      for(std::vector<id_type>::iterator iter = pubsub_info->msg_types.begin();
	  iter != pubsub_info->msg_types.end(); iter++) {
	pub.unpublish((*iter));
	sub.unsubscribe((*iter));
      }
    }
    else { //chat subject ids
      chat_msg *msg = (chat_msg *) p.get();
      cout << msg->source_ << " speak on [" << chat_chan::id_trait::id_to_string(id) << "]:\n";
      cout << msg->data_  << endl;
    }
  }
};

/**
 *  binder_generator : based on security requirement and name_space management req
 *            we can use diff binders (filter & translator) for diff sock conn
 */
binder * binder_generator(boost::shared_ptr<tcp::socket> sock)
{
  //in real application we may create binders based on remote ip address
  //here we just do simple
  static int num = 0;
  string prefix = "/client";
  std::ostringstream os;
  os << num;
  num++;
  return new binder(prefix+os.str()+"/");
}

/**
 * here we are going to use the publish/subscribe API built on top of
 * named_in/named_out
 */

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: chat_srv srv_port\n";
    exit(0);
  }

  const char * my_name = "server";
  const char * srv_port = argv[1];

  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create asio_executor and run callbacks in asio main thread
    asio_executor asio_exec(io_service);
    //create local channel and run all its async operations in asio_exec
    chat_chan chan(&asio_exec);

    //pub/sub subjects and forard msgs
    msg_handler handler(chan);

    //register chat msg type for marshaling/demarshaling
    chat_chan::text_marshaler_registry mar_reg;
    mar_reg.register_default_msg_marshaler<chat_msg>();

    //create asio connectors and connect to remote channel
    asio_connector_async connector(io_service);
    connector.async_accept(atoi(srv_port), // channel address
	boost::bind(asio_bind_sock_chan<chat_chan, chat_chan::text_marshaler_registry>(&binder_generator),
		    boost::ref(chan), boost::ref(mar_reg), _1, _2,_3));


    //a separate thread to run io_service
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

    bool cont = true;
    char buf[1024];
    while (cont) {
      cout << "action : 1-send, 2-exit:\n";
      int action;
      cin.getline(buf, 1024);
      action = atoi(buf);
      switch (action) {
      case 1:
	{
	  cout << "subject: ";
	  cin.getline(buf, 1024);
	  id_type id(buf);
	  cout << "message: ";
	  cin.getline(buf, 1024);
	  handler.pub.send(id, new chat_msg(my_name,buf));
	}
	break;
      case 2:
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

