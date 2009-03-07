//
// spawner2.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <string>
#include <iostream>

using namespace boost::join;

logger log1("log");

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

void run(std::string str) {
  for(int i=0; i<6; i++) {
    log1.msg(str);
    thread_sleep(1);
  }
}

int main(int argc, char **argv) {
  executor exec(2);
  async_p<executor::task> &spawn(exec.execute);
  spawn(boost::bind(run, "thread 1"));
  spawn(boost::bind(run, "thread 2"));
  thread_sleep(5);
  exec.shutdown();
  log1.msg("...main thread exits...");
  return 0;
}
