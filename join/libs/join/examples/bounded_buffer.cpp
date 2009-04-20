//
// bounded_buffer.cpp
//
// Copyright (c) 2007 - 2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
 
using namespace boost::join;

logger log1("log");

template <typename T>
class BufferN : public joint {
private:
  async<void> token;
  async<T> value;

public:
  synch<void,T> put;
  synch<T,void> get;

  BufferN(int sz) {
    chord(put, token, &BufferN::put_cb);
    chord(get, value, &BufferN::get_cb);
    for(int i=0; i<sz; i++) 
      token();  
  }
  void put_cb(T put, void_t) {
    value(put);
  }
  T get_cb(void_t, T val) {
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

void producer(synch<void,int> put) {
  for(int i=0; i<2000000; i++) {
    put(i);
    log1.stream() << "producer sends [" << i << "]" << logger::endl;
    //thread_sleep(2);
  }
}
void consumer1(synch<int,void> get) {
  for(int i=0; i<1000000; i++) {
    log1.stream() << "consumer1 recvs [" << get() << "]" << logger::endl;      
    //thread_sleep(3);
  }
}
void consumer2(synch<int,void> get) {
  for(int i=0; i<1000000; i++) {
    log1.stream() << "consumer2 recvs [" << get() << "]" << logger::endl;      
    //thread_sleep(1);
  }
}

int main(int argc, char **argv) {
  executor exec(3);

  //create bounded buffer
  BufferN<int> buf(5);
  
  //create test tasks
  exec.spawn(boost::bind(consumer1, buf.get));
  exec.spawn(boost::bind(consumer2, buf.get));
  exec.spawn(boost::bind(producer, buf.put));

  exec.shutdown();
  return 0;
}

