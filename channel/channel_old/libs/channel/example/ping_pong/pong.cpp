//
// pong.cpp
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
#include "./pingpong_defs.hpp"

//msg handler; print out data
void msg_handler(msg_chan::out & pong_o, int id, boost::shared_ptr<void> p)
{
  pingpong_msg *msg = (pingpong_msg *) p.get();
  cout << "Pong recv count = " << msg->count_  << endl;
  pong_o.send(new pingpong_msg(msg->count_+1));
}

void binding_callback(msg_chan::out ** pong_o, msg_chan::name *n, msg_chan::name::binding_event e) {
  if (n->type_ == msg_chan::name::member_remote && e == msg_chan::name::bind_ev) {
    (*pong_o)->send(new pingpong_msg(0));
  }
}

int main(int, char **) {
  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create local channel and bind local event source/sink
    msg_chan chan;

    //publish pong msg
    msg_chan::out *po = NULL;
    msg_chan::out pong_o(chan,
			 msg_id_pong,
			 boost::bind(binding_callback, &po, _1,_2));
    po = &pong_o;

    //subscribe ping msg
    msg_chan::in ping_i(chan, msg_id_ping, boost::bind(msg_handler,
				     boost::ref(pong_o), _1,_2));

    //register msg type for marshaling/demarshaling
    msg_chan::text_marshaler_registry mar_reg;
    mar_reg.register_default_msg_marshaler<pingpong_msg>();

    //create asio connectors and connect to remote channel
    asio_connector_async connector(io_service);
    connector.async_accept(9999, // channel address
			   boost::bind(asio_bind_sock_chan<msg_chan, msg_chan::text_marshaler_registry>(),
				       boost::ref(chan), boost::ref(mar_reg), _1, _2, _3));
    /*
    connector.async_connect("localhost", "8888", //remote channel address
			    boost::bind(asio_bind_sock_chan<msg_chan, msg_chan::text_marshaler_registry>(),
					boost::ref(chan), boost::ref(mar_reg), _1, _2, _3));
    */
    io_service.run();
  }
  catch (boost::system::error_code& e)
    {
      std::cerr << e << "\n";
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }

  cout << "... pong exit ...\n";
  return 0;
}

