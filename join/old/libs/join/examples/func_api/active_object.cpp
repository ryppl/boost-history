//
// active_object.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace boost::join;

logger log1("log");

class ActiveObject : public actor {
protected:
  bool done;
  synch<void()> ProcessMessage;
  async<void()> Start;
public:
  ActiveObject(executor *e) : actor(e), done(false) {
    chord(Start, &ActiveObject::main_loop);
    Start();
  }
  void main_loop(async_o<void()> s) {
    log1.msg("Active object thread starts");
    while(!done) ProcessMessage();
  }
}; 

class EventSink {
public:
  async<void(std::string)> Post;
};

class Distributor : public ActiveObject, public EventSink {
  std::vector<EventSink*> subscribers;
  std::string name;
public:
  async<void(EventSink*)> Subscribe;
  async<void()> Close;

  Distributor(std::string n, executor *e) : ActiveObject(e), name(n) {
    chord(ProcessMessage, Subscribe, &Distributor::sub_cb);
    chord(ProcessMessage, Post, &Distributor::post_cb);
    chord(ProcessMessage, Close, &Distributor::close_cb);
  }

  void sub_cb(synch_o<void()> P, async_o<void(EventSink*)> S) {
    subscribers.push_back(S.arg1);
  }
  void post_cb(synch_o<void()> P, async_o<void(std::string)> M) {
    for(size_t i=0; i<subscribers.size(); i++) 
      subscribers[i]->Post(name+" : "+M.arg1);
  }
  void close_cb(synch_o<void()> P, async_o<void()> C) {
    done = true;
  }

};

class Subscriber : public EventSink, public actor {
  std::string name;
public:
  Subscriber(std::string n, executor *e) : actor(e), name(n) {
    chord(Post, &Subscriber::post_cb);
  }
  void post_cb(async_o<void(std::string)> M) {
    log1.stream() << name << " got message " << M.arg1 << logger::endl;
  }
};

int main(int argc, char **argv) {
  executor exec(2);
  Distributor d("D", &exec.execute);
  Subscriber a("a", &exec.execute);
  d.Subscribe(&a);
  d.Post("First message");
  Subscriber b("b", &exec.execute);
  d.Subscribe(&b);
  d.Post("Second message");
  Subscriber c("c", &exec.execute);
  d.Subscribe(&c);
  d.Post("Third message");
  d.Close();
  log1.msg("Main thread waits for exit...");
  exec.shutdown();
  return 0;
}

