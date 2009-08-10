//
// evt_chan2.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/channel/streams/asio_stream_async.hpp>
#include <boost/channel/executors/asio_executor.hpp>
#include "./evt_chan_defs.hpp"

//free event handler; print out data
void evt_handler(string id, boost::shared_ptr<void> p)
{
  cout << "free handler ... recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
}

int main(int, char **) {
  //create local event channel
  evt_chan chan;
  //bind event sinks to channel
  evt_chan::in window_down_in(chan, down_id, evt_handler);
  evt_chan::in window_up_in(chan, up_id, evt_handler);
  evt_chan::in shut_down_in(chan, close_id, evt_handler);

  //register event msg type for marshaling/demarshaling
  evt_chan::text_marshaler_registry mar_reg;
  std::vector<evt_chan::id_type> ids;
  ids.push_back(down_id);
  ids.push_back(up_id);
  ids.push_back(close_id);
  mar_reg.register_msg_marshaler<evt_data>(ids);

  //create asio connectors and wait for remote conn to local channel
  try {
    boost::asio::io_service io_service;
    asio_connector_async connector(io_service);
    connector.async_accept(6666, //channel published at port 6666
	boost::bind(asio_bind_sock_chan<evt_chan, evt_chan::text_marshaler_registry>(),
		    boost::ref(chan), boost::ref(mar_reg), _1, _2,_3));

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

  cout << "... exit ...\n";
  return 0;
}

