//
// ccr2.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

using namespace boost::join;
using namespace boost;
using namespace std;

logger log1("log");

//base interface for all service messages. define <result, failure> channels
class ServiceResponse {
public:
  async<void(string)> result;
  async<void(std::exception*)> failure;
};

class Stop : public ServiceResponse {};

class UpdateState : public ServiceResponse {
public:
  string state;
  UpdateState(string s): state(s) {}
  void set(string st) { state = st; }
};

class GetState : public ServiceResponse {};

//base interface of all services, defining what messages a service can serve
class Service {
public:
  async<void(Stop&)> stop;
  async<void(UpdateState&)> update;
  async<void(GetState&)> get;
};

//a simple service implemnt the above service base interface
//following the same pattern as CCR, using a static method to
//create the server obj and return the msg based interface
class SimpleService : public Service, public actor {
public:
  static shared_ptr<Service> Create(joint::executor *e) {
    shared_ptr<Service> srv(new SimpleService(e));
    return srv;
  }

private:
  string state_;

  SimpleService(joint::executor *e) : actor(e), state_("") {
    chord(update, &SimpleService::UpdateHandler);
    chord(get, &SimpleService::GetStateHandler);
    chord(stop, &SimpleService::StopHandler);
  }
  void GetStateHandler(async_o<void(GetState&)> get) {
    if (state_.length() == 0)
      get.arg1.failure(new std::exception());
    else
      get.arg1.result("from get: "+state_);
  }
  void UpdateHandler(async_o<void(UpdateState&)> update) {
    state_ = update.arg1.state;
    update.arg1.result("from update: "+state_);
  }
  void StopHandler(async_o<void(Stop&)> stop) {
    log1.stream() << "clients ask for stop"  << logger::endl;
    stop.arg1.result("i am going to stop...");
  }
};

void async_print_result(async_o<void(string)> a) {
  log1.stream() << "async result : " << a.arg1 << logger::endl;
}


void async_print_failure(async_o<void(std::exception *)> e) {
  delete e.arg1;
  log1.stream() << "async got std::exception" << logger::endl;
}

void synch_print_result(synch_o<void()> r, async_o<void(string)> a) {
  log1.stream() << "synch recv result : " << a.arg1 << logger::endl;
}


void synch_print_failure(synch_o<void()> r, async_o<void(std::exception *)> e) {
  delete e.arg1;
  log1.stream() << "synch got std::exception" << logger::endl;
}


void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

int main(int argc, char **argv) {
  
  //create an executor with 2 threads in pool
  executor exec(2);  

  shared_ptr<Service> ssrv(SimpleService::Create(&exec.execute));

  GetState g;
  UpdateState u("state1");

  //asynchronous recv and processing responses from server
  joint joins(&exec.execute);
  joins.chord(g.result, async_print_result)
       .chord(g.failure, async_print_failure)
       .chord(u.result, async_print_result)
       .chord(u.failure, async_print_failure);

  //send some requests
  ssrv->get(g); //should get std::exception here
  ssrv->update(u);
  ssrv->get(g); 

  //wait for async processing  responses
  log1.stream() << "wait 1 secs for async responses processing ..." << logger::endl;
  thread_sleep(1);

  //reset joins 
  joins.reset();

  //create synchronous recv channel
  synch<void()> recv;

  //and steup new chords with synchronous "recv"
  joins.chord(recv, g.result, synch_print_result)
       .chord(recv, g.failure, synch_print_failure)
       .chord(recv, u.result, synch_print_result)
       .chord(recv, u.failure, synch_print_failure);
  
  //send a few req 
  int runs=5;
  for(int i=0; i<runs; i++) 
    ssrv->update(u);

  //recv & process the responses. Note: recv() will block wait
  for(int i=0; i<runs; i++)
    recv();

  exec.shutdown();

  return 0;
}
 
