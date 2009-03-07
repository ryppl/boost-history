//
// bounded_buffer.cpp
//
// Copyright (c) 2007 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>

using namespace boost::join;

logger log1("log");

template <typename T>
class BufferN : public actor {
private:
  async_p<void> token;
  async_p<T> value;

public:
  synch_p<void,T> put;
  synch_p<T,void> get;

  BufferN(int sz) {
    chord(put, token, &BufferN::put_cb);
    chord(get, value, &BufferN::get_cb);
    for(int i=0; i<sz; i++) 
      token();  
  }
  void put_cb(synch_v<void,T> put, async_v<void> tok) {
    value(put);
  }
  T get_cb(synch_v<T,void> get, async_v<T> val) {
    token();
    return val;
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class Test : public actor {
public:
  BufferN<int> buf_;
  async_p<void> producer;
  async_p<void> consumer1;
  async_p<void> consumer2;
  Test(executor *e) : actor(e), buf_(5) {
    chord(producer, &Test::producer_cb);
    chord(consumer1, &Test::consumer1_cb);
    chord(consumer2, &Test::consumer2_cb);
  }
  void producer_cb(async_v<void> p) {
    for(int i=0; i<2000000; i++) {
      buf_.put(i);
      log1.stream() << "producer sends [" << i << "]" << logger::endl;
      //thread_sleep(2);
    }
  }
  void consumer1_cb(async_v<void> c) {
    for(int i=0; i<1000000; i++) {
      log1.stream() << "consumer1 recvs [" << buf_.get() << "]" << logger::endl;      
      //thread_sleep(3);
    }
  }
  void consumer2_cb(async_v<void> c) {
    for(int i=0; i<1000000; i++) {
      log1.stream() << "consumer2 recvs [" << buf_.get() << "]" << logger::endl;      
      //thread_sleep(1);
    }
  }
};

int main(int argc, char **argv) {
  executor exec(3);
  Test test(&exec.execute);
  test.producer();
  test.consumer1();
  test.consumer2();
  exec.shutdown();
  return 0;
}

