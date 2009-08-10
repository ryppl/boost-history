//
// evt_chan1.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/thread.hpp>
#include <boost/channel/streams/asio_stream_async.hpp>
#include <boost/channel/executors/asio_executor.hpp>
#include "./evt_chan_defs.hpp"

class gui_window {
  evt_chan & chan_;
  //a few event src
  evt_chan::out down_out_;
  evt_chan::out up_out_;
  evt_chan::out close_out_;
  ///should wait for remote channel conneted and ids/names bound and
  ///then fire some sample events
  bool rmt_down_bound;
  bool rmt_up_bound;
  bool rmt_close_bound;
public:
  gui_window(evt_chan & c):
    chan_(c),
    down_out_(chan_,
	      down_id,
	      boost::bind(&gui_window::binding_callback, this, _1, _2)),
    up_out_(chan_,
	    up_id,
	    boost::bind(&gui_window::binding_callback, this, _1, _2)),
    close_out_(chan_,
	       close_id,
	       boost::bind(&gui_window::binding_callback, this, _1, _2)),
    rmt_down_bound(false),rmt_up_bound(false),rmt_close_bound(false)
  {
  }

  //methods to fire some events
  void up(const char * msg) {
    up_out_.send(new evt_data(msg));
  }
  void down(const char * msg) {
    down_out_.send(new evt_data(msg));
  }
  void close(const char * msg) {
    close_out_.send(new evt_data(msg));
  }

  void binding_callback(evt_chan::name *n, evt_chan::name::binding_event e) {
    if (n->type_ == evt_chan::name::member_remote && e == evt_chan::name::bind_ev) {
      if (n->id_ == down_id) rmt_down_bound = true;
      if (n->id_ == up_id) rmt_up_bound = true;
      if (n->id_ == close_id) rmt_close_bound = true;
      ///fire sample events after remote peer connected and ids bound
      if (rmt_down_bound && rmt_up_bound && rmt_close_bound) {
	up("..Hi there [mouse-left-up] ..");
	down(".. a simple window test [mouse-right-down] ..");
	close(".. simple window wiered [window-closed] ..");
      }
    }
  }
};

class window_handler {
  evt_chan & ch_;
  //a few event sinks
  evt_chan::in *down_in_;
  evt_chan::in *up_in_;
  evt_chan::in *close_in_;
public:
  window_handler(evt_chan & c): ch_(c) {
    down_in_ = ch_.bind_name_in(down_id, boost::bind(&window_handler::down, this, _1, _2));
    up_in_ = ch_.bind_name_in(up_id, boost::bind(&window_handler::up, this, _1, _2));
    close_in_ = ch_.bind_name_in(close_id, boost::bind(&window_handler::close, this, _1, _2));
  }
  ~window_handler() {
    delete down_in_;
    delete up_in_;
    delete close_in_;
  }

  //methods to handle events
  void down(string id, boost::shared_ptr<void> p) {
    cout << "window_handler::DOWN ... recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
  void up(string id, boost::shared_ptr<void> p) {
    cout << "window_handler::UP ... recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
  void close(string id, boost::shared_ptr<void> p) {
    cout << "window_handler::CLOSE ... recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
};


int main(int, char **) {
  try {
    //create asio io_service
    boost::asio::io_service io_service;
    //create asio_executor
    asio_executor asio_exec(io_service);
    //create local channel and run all its async operations in asio_exec
    evt_chan chan(&asio_exec);

    //bind event source (window) to channel and register name_binding_callback
    //so that when remote channel connected and name bound, we can start firing sample events
    gui_window window(chan);
    //bind event sink (handler) to channel
    window_handler hdlr(chan);

    //register event msg type for marshaling/demarshaling
    evt_chan::text_marshaler_registry mar_reg;
    std::vector<evt_chan::id_type> ids;
    ids.push_back(down_id);
    ids.push_back(up_id);
    ids.push_back(close_id);
    mar_reg.register_msg_marshaler<evt_data>(ids);

    //create asio connectors and connect to remote channel
    asio_connector_async connector(io_service);
    connector.async_connect("localhost", "6666", //remote channel address
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

