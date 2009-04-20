//
// flows.cpp
//
// Copyright (c) 2007 - 2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <boost/spirit/include/phoenix1.hpp>
#include "boost/tuple/tuple.hpp"

using namespace boost;
using namespace boost::join;
using namespace phoenix;

logger log1("log");

typedef boost::tuple<async<int>, async<int>, async<int> > flows_bundle;

flows_bundle make_data_channels(joint::spawn_type e) {
  async<int> flow1;
  async<int> flow2;
  async<int> flow3;
  joins_t<sched_first_match,32>(e)
    .chord(flow1, flow2, std::cout << val('(') << arg1 << " + " << arg2 << ") = " << (arg1 + arg2) << "\n")
    .chord(flow2, flow3, std::cout << val('(') << arg1 << " - " << arg2 << ") = " << (arg1 - arg2) << "\n")
    .chord(flow1, flow3, std::cout << val('(') << arg1 << " * " << arg2 << ") = " << (arg1 * arg2) << "\n");
  return boost::make_tuple(flow1, flow2, flow3);
}

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

void producer1(async<int> chan1) {
  for(int i=0; i<12; i++) {
    chan1(i);
    log1.stream() << "producer1 sends [" << i << "]" << logger::endl;
    thread_sleep(1);
  }
}
void producer2(async<int> chan2) {
  for(int i=0; i<10; i++) {
    chan2(i);
    log1.stream() << "producer2 sends [" << i << "]" << logger::endl;      
    thread_sleep(2);
  }
}
void producer3(async<int> chan3) {
  for(int i=0; i<12; i++) {
    chan3(i);
    log1.stream() << "producer3 sends [" << i << "]" << logger::endl;      
    thread_sleep(1);
  }
}

int main(int argc, char **argv) {
  executor exec(5);

  //create data channels
  flows_bundle flows = make_data_channels(exec.execute);

  //spawn test tasks
  exec.spawn(boost::bind(producer1, tuples::get<0>(flows)));
  exec.spawn(boost::bind(producer2, tuples::get<1>(flows)));
  exec.spawn(boost::bind(producer3, tuples::get<2>(flows)));

  thread_sleep(10);
  exec.shutdown();
  return 0;
}

