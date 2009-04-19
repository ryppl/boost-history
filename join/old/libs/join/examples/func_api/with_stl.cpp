//
// with_stl.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/join/join.hpp>

using namespace boost::join;

logger log1("log");

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class workers : public actor {
public:
  async<void(int)> do_it;
  workers(executor *e) : actor(e) {
    chord(do_it, &workers::chord_body);
  }
private:
  void chord_body(async_o<void(int)> do_it) {
    for(int i=0; i<5; i++) {
      log1.stream() << "do_it: " << do_it.arg1 << logger::endl;
      thread_sleep(1);
    }
  }
};

int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int size = sizeof(data) / sizeof(int);

int main(int argc, char **argv) {
  executor exec(5);  
  workers w(&exec.execute);
  std::for_each(data, data+size, boost::bind(boost::ref(w.do_it), _1));
  //or
  //std::for_each(data, data+size, boost::function<void(int)>(boost::ref(w.do_it)));
  exec.shutdown();
  return 0;
}

