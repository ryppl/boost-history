//
// gui_evt3.cpp
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
typedef signal<> signal_type;
typedef signal_type::slot slot_type;

//a simple struct for event data
struct evt_data {
  string data_;
  evt_data(const char *d) : data_(d) {}
};

class gui_window {
public:
  //a few event src
  signal_type down_;
  signal_type up_;
  signal_type close_;

  gui_window(): down_(),
		up_(),
		close_() {}
  //methods to fire some events
  void up(const char * msg) {
    up_.send(new evt_data(msg));
  }
  void down(const char * msg) {
    down_.send(new evt_data(msg));
  }
  void close(const char * msg) {
    close_.send(new evt_data(msg));
  }
};

class window_handler {
  gui_window & win_;
  slot_type *down_slot, *up_slot, *close_slot;
public:
  window_handler(gui_window & w): win_(w) {
    down_slot = win_.down_.bind(boost::bind(&window_handler::down, this, _1));
    up_slot = win_.up_.bind(boost::bind(&window_handler::up, this, _1));
    close_slot = win_.close_.bind(boost::bind(&window_handler::close, this, _1));
  }
  ~window_handler() {
    delete down_slot;
    delete up_slot;
    delete close_slot;
  }
  //methods to handle events
  void down(boost::shared_ptr<void> p) {
    cout << "window_handler::DOWN ... recv [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
  void up(boost::shared_ptr<void> p) {
    cout << "window_handler::UP ... recv [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
  void close(boost::shared_ptr<void> p) {
    cout << "window_handler::CLOSE ... recv [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
  }
};


//free event handler; print out data
void evt_handler(boost::shared_ptr<void> p)
{
  cout << "free handler ... recv event [data=" << ((evt_data*)p.get())->data_ << "]" << endl;
}

int main(int, char **) {
  //bind event source (window) to channel
  gui_window window;

  //bind event sink (handler) to channel
  window_handler hdlr(window);

  //bind more event sinks
  window.down_.bind(evt_handler);
  window.up_.bind(evt_handler);
  window.close_.bind(evt_handler);

  //fire events
  window.up("..Hi there [mouse-left-up] ..");
  window.down(".. a simple window test [mouse-right-down] ..");
  window.close(".. simple window wiered [window-closed] ..");

  cout << "... exit ...\n";
  return 0;
}

