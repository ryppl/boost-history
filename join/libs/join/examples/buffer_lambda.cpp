//
// buffer.cpp
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
#include <boost/lambda/lambda.hpp>

using namespace boost;
using namespace boost::join;
using namespace boost::lambda;

logger log1("log");

template <typename V>
class buffer {
public:
  async<V> put;
  synch<V,void> get;
  buffer() {
    joins().chord(get, put, lambda::_2);
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

void producer(async<std::string> put) {
  std::ostringstream ostr;
  for(int i=0; i<5; i++) {
    ostr << i;
    put(ostr.str());
    log1.stream() << "producer sends [" << i << "]" << logger::endl;
    ostr.str("");
    thread_sleep(1);
  }
}
void consumer(synch<std::string,void> get) {
  for(int i=0; i<5; i++) {
    log1.stream() << "consumer recvs [" << get() << "]" << logger::endl;      
    thread_sleep(2);
  }
}

int main(int argc, char **argv) {
  buffer<std::string> b;
  hello_world(b);

  executor exec(2);  //spawn 2 threads for executor thread pool

  //spawn test tasks
  exec.spawn(boost::bind(consumer, b.get));
  exec.spawn(boost::bind(producer, b.put));

  exec.shutdown();
  return 0;
}
