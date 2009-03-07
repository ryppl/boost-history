//
// async_call_ret.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/join/join.hpp>
#include <iostream>

using namespace std;
using namespace boost::join;

logger log1("log");

class IService {
public:
  async<void(string, async<void(string)>&)> Service;
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class MyService : public actor, public IService {
  int n;
public:
  MyService(int m, executor *e) : actor(e), n(m) {
    chord(Service, &MyService::service_fun);
  }
  void service_fun(async_o<void(string, async<void(string)>&)> req) {
    for(int i=0; i<n; i++) {
      log1.stream() << req.arg1 << " is " << i << logger::endl;
      thread_sleep((n%2)?1:2);
    }
    req.arg2(req.arg1+" is done");
  }
};

class Join2 : public actor {
public:
  async<void(string)> first;
  async<void(string)> second;
  synch<void(string&, string&)> Wait;
  Join2() {
    chord(Wait, first, second, &Join2::wait_body);
  }
  void wait_body(synch_o<void(string&,string&)> w, 
                 async_o<void(string)> f, 
                 async_o<void(string)> s) {
    w.arg1 = f.arg1;
    w.arg2 = s.arg1;
  }
};

int main(int argc, char **argv) {
  executor exec(2);
  MyService s1(5, &exec.execute);
  MyService s2(10, &exec.execute);
  Join2 j;
  s1.Service("Service 1", j.first);
  s2.Service("Service 2", j.second);
  for(int i=0; i<7; i++) {
    log1.stream() << "Main " << i << logger::endl;
    thread_sleep(1);
  }
  string x,y;
  j.Wait(x,y);
  log1.stream() << "first result = " << x << ", second result = " << y << logger::endl;
  exec.shutdown();
  return 0;
}

