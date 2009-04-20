//
// chain.cpp
//
// Copyright (c) 2007 - 2009 Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <boost/spirit/include/phoenix1.hpp>

using namespace boost;
using namespace boost::join;
using namespace std;
using namespace phoenix;

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

//the node in the chain:
//read data from input, transform its value, wait a sec 
//and send the new value to output
//all these are running in executor thread pool
template <typename T>
class node {
  int my_no;
  boost::function<T(T)> func_;
public:
  async<T> input;
  async<T> output;
  bool stop;

  node(joint::spawn_type e, int n, boost::function<T(T)> f) : 
    my_no(n), func_(f), stop(false) {
    joins(e).chord(input, bind(&node::proc, this, _1));
  }

private:
  void proc(T in) {
    try {
      if(!stop) {
        T o = func_(in);
        cout << "node [" << my_no << "] recv [" << in <<"] and send [" << o << "]" << endl;
        thread_sleep(1);
        output(o);
      }
    } catch (join_exception &e) {
      cout << "node [" << my_no << "] got exception: " << e.what() << "\n";
    }
  }
};

enum {
  num_nodes = 10,
  num_data = 5
};

int main(int, char**) {
  executor exec(3);
  vector<node<int>*> nodes;
  nodes.reserve(num_nodes);

  //create nodes
  for(int i=0; i<num_nodes; i++) {
    nodes.push_back(new node<int>(exec.execute, i, arg1+val(1)));
  }
  
  //setup chaining, please note we should connect output to input
  for(int j=0;j<num_nodes;j++) {
    if(j<(num_nodes-1)) {
      nodes[j]->output = nodes[j+1]->input;
    }
  }
  nodes[num_nodes-1]->output = nodes[0]->input;

  //pump some data into chain
  for(int i=0;i<num_data;i++) {
    nodes[0]->input(i);
    thread_sleep(1);
  }

  //wait for while to see the chain-reaction
  thread_sleep(20);

  //stop the chain
  for(int i=0; i<num_nodes; i++)
    nodes[i]->stop = true;
  exec.shutdown();
  return 0;
}
