//
// join_many.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace boost::join;

logger log1("log");

// join multiple input streams into one output stream
template <typename T, size_t N>
class join_many : public actor {
public:
  array<async<void(T)>,N> inputs;
  synch<array<T,N>()> output;
  join_many() {
    chord(output, inputs, &join_many::chord_body);
  }
  array<T,N> chord_body(synch_o<array<T,N>()> out, array<async_o<void(T)>,N> in) {
    array<T,N> vt;
    for(size_t i=0; i<N; i++)
      vt[i] = in[i].arg1;
    return vt;
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
  enum {
    num_chan = 4,
    num_test = 5
  };
  join_many<int,num_chan> merger;
  async<void(int)> inputer;
  void wait(void) {
    array<int,num_chan> results;
    for(int i=0; i<num_test; i++) {
      results = merger.output();
      log1.stream() << "output" << i << " = " << logger::end;
      for(size_t j=0;j<num_chan;j++) 
        log1.stream() << results[j] << " " << logger::end;
      log1.stream() << logger::endl;
    }
  }
  Demo(executor *e) : actor(e) {
    chord(inputer, &Demo::chord_body);
    //spawn multiple concurrent test streams
    for(int i=0; i<num_chan; i++)
      inputer(i);
  }
  void chord_body(async_o<void(int)> p) {
    int start = p.arg1*num_test;
    int end = start+num_test;
    for(int i=start; i<end; i++) {
      log1.stream() << "inputer[" << p.arg1 << "] sends [" << i << "] and wait..." << logger::endl;
      merger.inputs[p.arg1](i);
      thread_sleep((p.arg1+1)%3);
    }
  }
};

int main(int argc, char **argv) {
  executor exec(4);  //spawn 4 threads for executor thread pool
  Demo demo(&exec.execute);
  log1.msg("main thread starts waiting...");
  demo.wait();
  log1.msg("main thread finish waiting...");
  exec.shutdown();
  return 0;
}
