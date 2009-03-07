//
// buffer.cpp
//
// Copyright (c) 2007 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <iostream>
#include <sstream>
#include <boost/join/join.hpp>

//using namespace boost;
using namespace boost::join;

logger log1("log");

template <typename V>
class buffer: public actor {
public:
  async_p<V> put;
  synch_p<V,void> get;
  buffer() {
    chord(get, put, &buffer::chord_body);
    log1.msg("buffer constructed");
  }
  V chord_body(synch_v<V,void> get, async_v<V> put) {
    return put.arg1;
  }
};

void hello_world(buffer<std::string> &b) {
  b.put("hello");
  b.put("world");
  log1.msg(b.get()+" ");
  log1.msg(b.get());
}

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class Demo : public actor {
public:
  buffer<std::string> &buf_;
  async_p<void> producer;
  async_p<void> consumer;
  Demo(buffer<std::string> &b, executor *e) : actor(e, 0, 0), buf_(b) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_v<void> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      ostr << i;
      buf_.put(ostr.str());
      log1.stream() << "producer sends [" << i << "]" << logger::endl;
      ostr.str("");
      thread_sleep(1);
    }
  }
  void consumer_cb(async_v<void> c) {
    for(int i=0; i<5; i++) {
      log1.stream() << "consumer recvs [" << buf_.get() << "]" << logger::endl;      
      thread_sleep(2);
    }
  }
};

int main(int argc, char **argv) {
  buffer<std::string> b;
  hello_world(b);

  executor exec(2);  //spawn 2 threads for executor thread pool
  Demo demo(b, &exec.execute);
  demo.producer();
  demo.consumer();
  exec.shutdown();
  return 0;
}
