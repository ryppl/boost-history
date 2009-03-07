//
// one_place_buffer.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <string>
#include <iostream>
#include <sstream>

using namespace boost::join;

logger log1("log");

template <typename V>
class one_place_buffer: public actor {
private: 
  //private async methods/msgs to represent buffer states
  async<void()> empty;
  async<void(V)> contains;
public:
  synch<void(V)> put;
  synch<V()> get;

  void put_cb(synch_o<void(V)> put, async_o<void()> empty) {
    contains(put.arg1);
  }

  V get_cb(synch_o<V()> get, async_o<void(V)> contains) {
    empty();
    return contains.arg1;
  }

  one_place_buffer() {
    chord(put, empty, &one_place_buffer::put_cb);
    chord(get, contains, &one_place_buffer::get_cb);
    empty();  //init state
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class Demo : public actor {
public:
  one_place_buffer<std::string> &buf_;
  async<void()> producer;
  async<void()> consumer;
  Demo(one_place_buffer<std::string> &b, executor *e) : actor(e), buf_(b) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      ostr << i;
      buf_.put(ostr.str());
      log1.stream() << "producer sends [" << i << "]" << logger::endl;
      ostr.str("");
      thread_sleep(1);
    }
  }
  void consumer_cb(async_o<void()> c) {
    for(int i=0; i<5; i++) {
      log1.stream() << "consumer recvs [" << buf_.get() << "]" << logger::endl;      
      thread_sleep(2);
    }
  }
};
 
int main(int argc, char **argv) {
  one_place_buffer<std::string> b;
  executor exec(2);  //spawn 2 threads for executor thread pool
  Demo demo(b, &exec.execute);
  demo.producer();
  demo.consumer();
  return 0;
}
 
