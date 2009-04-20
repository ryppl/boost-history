//
// join_many.cpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace boost;
using namespace boost::join;

logger log1("log");

// join multiple input streams into one output stream
template <typename T>
class join_many : public joint {
public:
  vector<async<T> > inputs;
  synch<vector<T>, void> output;
  join_many(int num) : inputs(num) {
    chord(output, inputs, &join_many::chord_body);
  }
  vector<T> chord_body(void_t out, vector<T> in) {
    return in;
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

enum {
  num_chan = 4,
  num_test = 5
};

void collect(synch<vector<int>,void> output) {
  vector<int> results;
  for(int i=0; i<num_test; i++) {
    results = output();
    log1.stream() << "output" << i << " = " << logger::end;
    for(size_t j=0;j<num_chan;j++) 
      log1.stream() << results[j] << " " << logger::end;
    log1.stream() << logger::endl;
  }
}

void distributor(int p, async<int> input) {
  int start = p*num_test;
  int end = start+num_test;
  for(int i=start; i<end; i++) {
    log1.stream() << "inputer[" << p << "] sends [" << i << "] and wait..." << logger::endl;
    input(i);
    thread_sleep((p+1)%3);
  }
}

int main(int argc, char **argv) {
  executor exec(num_chan);  //spawn 4 threads for executor thread pool

  join_many<int> merger(num_chan);

  //spawn distributor tasks
  for(int i=0; i<num_chan; i++)
    exec.spawn(boost::bind(distributor, i, merger.inputs[i]));

  //main thread wait and collect results
  log1.msg("main thread starts waiting...");
  collect(merger.output);
  log1.msg("main thread finish waiting...");

  exec.shutdown();
  return 0;
}
