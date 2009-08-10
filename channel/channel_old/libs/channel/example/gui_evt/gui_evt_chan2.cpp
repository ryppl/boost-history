//
// gui_evt_chan2.cpp
//
// Copyright (c) 2005-2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/channel/channel.hpp>

using namespace std;
using namespace boost::channel;

//instantiate event channel type, using struct_id as id_type
typedef channel<struct_id> evt_chan;

//event ids in name_space
//sample events for a gui window
struct_id win_but_down_id = {application_message, 1};
struct_id win_but_up_id = {application_message, 2};
struct_id win_close_id = {application_message, 3};

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
};

class gui_window {
  evt_chan & chan_;
  evt_chan::out down_out_;
  evt_chan::out up_out_;
  evt_chan::out close_out_;
public:
  gui_window(evt_chan & c): chan_(c),
			    down_out_(chan_, win_but_down_id),
			    up_out_(chan_, win_but_up_id),
			    close_out_(chan_, win_close_id) {}
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
};

class window_handler {
  evt_chan & ch_;
  evt_chan::in *down_in_;
  evt_chan::in *up_in_;
  evt_chan::in *close_in_;
public:
  window_handler(evt_chan & c): ch_(c) {
    down_in_ = ch_.bind_name_in(win_but_down_id, boost::bind(&window_handler::down, this, _1, _2));
    up_in_ = ch_.bind_name_in(win_but_up_id, boost::bind(&window_handler::up, this, _1, _2));
    close_in_ = ch_.bind_name_in(win_close_id, boost::bind(&window_handler::close, this, _1, _2));
  }
  ~window_handler() {
    delete down_in_;
    delete up_in_;
    delete close_in_;
  }
  //methods to handle events
  void down(struct_id id, boost::shared_ptr<void> p) {
    cout << "window_handler::DOWN ... recv event [id=" << evt_chan::id_trait::id_to_string(id) << "], [data=" << ((evt_data*)p.get())->data_ << "]\n";
  }
  void up(struct_id id, boost::shared_ptr<void> p) {
    cout << "window_handler::UP ... recv event [id=" << evt_chan::id_trait::id_to_string(id) << "], [data=" << ((evt_data*)p.get())->data_ << "]\n";
  }
  void close(struct_id id, boost::shared_ptr<void> p) {
    cout << "window_handler::CLOSE ... recv event [id=" << evt_chan::id_trait::id_to_string(id) << "], [data=" << ((evt_data*)p.get())->data_ << "]\n";
  }
};


//free event handler; print out data
void evt_handler(struct_id id, boost::shared_ptr<void> p)
{
  cout << "free handler ... recv event [id=" << evt_chan::id_trait::id_to_string(id) << "], [data=" << ((evt_data*)p.get())->data_ << "]\n";
}

int main(int, char **) {
  //create 2 event channels
  evt_chan chan1;
  evt_chan chan2;

  //connect 2 channels
  connect(chan1, chan2);

  //bind event source (window) to channel1
  gui_window window(chan1);

  //bind event sink (handler) to channel1
  window_handler hdlr(chan1);

  //bind more event sinks to channel2
  evt_chan::in window_down_in(chan2, win_but_down_id, evt_handler);
  evt_chan::in window_up_in(chan2, win_but_up_id, evt_handler);
  evt_chan::in shut_down_in(chan2, win_close_id, evt_handler);

  //fire events
  window.up("..Hi there [mouse-left-up] ..");
  window.down(".. a simple window test [mouse-right-down] ..");
  window.close(".. simple window wiered [window-closed] ..");

  cout << "... exit ...\n";
  return 0;
}

