//
// rwlock.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>

using namespace boost::join;

logger log1("log");

class ReaderWriter : public actor {
private:
  //async msgs describe rwlock status
  async<void()> idle;
  async<void(int)> s;

public:
  synch<void()> ReleaseShared;
  synch<void()> Shared;
  synch<void()> Exclusive;

  ReaderWriter() {
    chord(ReleaseShared, s, &ReaderWriter::release_cb);
    chord(Shared, idle, &ReaderWriter::shared_cb1);
    chord(Shared, s, &ReaderWriter::shared_cb2);
    chord(Exclusive, idle, &ReaderWriter::exclusive_cb);
    idle(); //init state
  }

  void release_cb(synch_o<void()> ReleaseShared, async_o<void(int)> sn) {
    if(sn == 1) idle();
    else s(sn-1);
  }
  void shared_cb1(synch_o<void()> Shared, async_o<void()> idl) {
    s(1);
  }
  void shared_cb2(synch_o<void()> Shared, async_o<void(int)> sn) {
    s(sn+1);
  }
  void exclusive_cb(synch_o<void()> Exclusive, async_o<void()> idl) {
  }
  void ReleaseExclusive() {
    idle();
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

class client : public actor {
  ReaderWriter &rwlock;
  int id;
  async<void()> run;
public:
  client(int i, ReaderWriter &rwl, executor *e) : actor(e), rwlock(rwl), id(i) {
    chord(run, &client::run_cb);
    run();
  }
  void run_cb(async_o<void()> r) {
    log1.msg("one client starts running...");
    while(true) {
      read(); delay();
      write(); delay();
    }
  }
  void delay() {
    thread_sleep(2);
  }
  void read() {
    log1.stream() << id << " waiting to read" << logger::endl;
    rwlock.Shared();
    log1.stream() << id << " reading" << logger::endl;
    delay();
    rwlock.ReleaseShared();
    log1.stream() << id << " finished reading" << logger::endl;
  }
  void write() {
    log1.stream() << id << " waiting to write" << logger::endl;
    rwlock.Exclusive();
    log1.stream() << id << " writing" << logger::endl;
    delay();
    rwlock.ReleaseExclusive();
    log1.stream() << id << " finished writing" << logger::endl;
  }
};

int main(int argc, char **argv) {
  ReaderWriter rwlock;
  int num_thr = 5;
  executor exec(num_thr);
  std::vector<boost::shared_ptr<client> > cls;
  for(int i=0; i<num_thr; i++) 
    cls.push_back(boost::shared_ptr<client>(new client(i, rwlock, &exec.execute)));
  exec.shutdown();
  return 0;
}

