//
// gui_evt_chan1.cpp
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

//instantiate event channel type, using string as id_type
typedef channel<string> evt_chan;

//event ids in name_space
//sample events for a gui window
string down_id = "_window_button_down_";
string up_id = "_window_button_up_";
string close_id = "_window_close_";

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
};

class gui_window {
  evt_chan & chan_;
  //a few event src
  evt_chan::out down_out_;
  evt_chan::out up_out_;
  evt_chan::out close_out_;
public:
  gui_window(evt_chan & c): chan_(c),
			    down_out_(chan_, down_id),
			    up_out_(chan_, up_id),
			    close_out_(chan_, close_id) {}
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


//free event handler; print out data
void evt_handler(string id, boost::shared_ptr<void> p)
{
  cout << "free handler ... recv event [id=" << id << "], [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
}

int main(int, char **) {
  //create event channel
  evt_chan chan;

  //bind event source (window) to channel
  gui_window window(chan);

  //bind event sink (handler) to channel
  window_handler hdlr(chan);

  //bind more event sinks
  evt_chan::in window_down_in(chan, down_id, evt_handler);
  evt_chan::in window_up_in(chan, up_id, evt_handler);
  evt_chan::in shut_down_in(chan, close_id, evt_handler);

  //fire events
  window.up("..Hi there [mouse-left-up] ..");
  window.down(".. a simple window test [mouse-right-down] ..");
  window.close(".. simple window wiered [window-closed] ..");

  cout << "... exit ...\n";
  return 0;
}

