//
// buffer.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
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
  async<void(V)> put;
  synch<V(void)> get;
  buffer() {
    chord(get, put, &buffer::chord_body);
    log1.msg("buffer constructed");
  }
  V chord_body(synch_o<V(void)> get, async_o<void(V)> put) {
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
  async<void()> producer;
  async<void()> consumer;
  Demo(buffer<std::string> &b, executor *e) : actor(e), buf_(b) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<5; i++) {
      thread_sleep(i+1);
      ostr << i;
      buf_.put(ostr.str());
      log1.stream() << "producer sends [" << i << "]" << logger::endl;
      ostr.str("");
    }
  }
  void consumer_cb(async_o<void()> c) {
    int i=0; 
    while(i<5) {
      try {
        boost::xtime xt;
        boost::xtime_get(&xt, boost::TIME_UTC);
        xt.sec += 1; //receiver timeout in 1 sec
        log1.stream() << "consumer recvs [" << buf_.get(xt) << "]" << logger::endl;      
        i++;
      }
      catch (synch_time_out_exception) {
        log1.msg("consumer timeout (1 sec)"); 
      }
      catch (...) {
        log1.msg("consumer unknown exception");  
      }
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
