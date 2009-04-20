//
// producer_consumer.cpp
//
// Copyright (c) 2007 - 2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <iostream>
#include <sstream>
#include <boost/function.hpp>
#include <boost/join/join.hpp>
#include <boost/spirit/include/phoenix1.hpp>
#include "boost/tuple/tuple.hpp"

using namespace boost;
using namespace boost::join;
using namespace phoenix;

logger log1("log");

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

template <typename V>
boost::tuple<async<V>,synch<V,void> > create_msg_que() {
  async<V> send;
  synch<V,void> recv;
  joins().chord(recv, send, arg2);
  return boost::make_tuple(send, recv);
}

void producer(async<int> send) {
  for(int i=0;i<10;i++) {
    send(i);
    log1.stream() << "producer sends: " << i << logger::endl;
    thread_sleep(1);
  }
}

void consumer(synch<int,void> recv) {
  for(int i=0; i<10; i++) {
    log1.stream() << "consumer recvs: " << recv() << logger::endl;
  }
}

int main(int argc, char **argv) {
  executor exec(2);  //spawn 2 threads for executor thread pool

  //create msg que
  async<int> send;
  synch<int, void> recv;
  boost::tie(send, recv) = create_msg_que<int>();

  //spawn prod/consum tasks
  exec.spawn(boost::bind(consumer, recv));
  exec.spawn(boost::bind(producer, send));

  exec.shutdown();
  return 0;
}
