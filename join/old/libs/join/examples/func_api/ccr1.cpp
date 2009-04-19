//
// ccr1.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/bind.hpp>
#include <string>

using namespace boost::join;
using namespace boost;
using namespace std;

logger log1("log");


class ServiceResponse {
public:
  async<void(string)> result;
  async<void(std::exception*)> failure;
};

class Stop {
public:
  ServiceResponse *response;
  Stop(ServiceResponse *r) : response(r) {}
};

class UpdateState {
public:
  ServiceResponse *response;
  string state;
  UpdateState() {}
  UpdateState(string st, ServiceResponse *r) : response(r), state(st) {}
};

class GetState {
public:
  ServiceResponse *response;
  GetState() {}
  GetState(ServiceResponse *r) : response(r) {}
};

class SimpleService {
  joint joins;
  string state_;
public:
  async<void(Stop)> stop;
  async<void(UpdateState)> update;
  async<void(GetState)> get;

  SimpleService(joint::executor *e) : joins(e), state_("") {
    joins.chord(update, bind(&SimpleService::UpdateHandler, this, _1))
         .chord(get, bind(&SimpleService::GetStateHandler, this, _1));
  }
  void GetStateHandler(async_o<void(GetState)> get) {
    if (state_.length() == 0)
      get.arg1.response->failure(new std::exception());
    else
      get.arg1.response->result("from get: "+state_);
  }
  void UpdateHandler(async_o<void(UpdateState)> update) {
    state_ = update.arg1.state;
    update.arg1.response->result("from update: "+state_);
  }
};

void print_result(async_o<void(string)> a) {
  log1.stream() << "result : " << a.arg1 << logger::endl;
}


void print_failure(async_o<void(std::exception *)> e) {
  delete e.arg1;
  log1.stream() << "got std::exception" << logger::endl;
}


int main(int argc, char **argv) {
  
  //create an executor with 2 threads in pool
  executor exec(2);  

  SimpleService ssrv(&exec.execute);

  ServiceResponse resp;

  joint joins(&exec.execute);
  joins.chord(resp.result, print_result)
       .chord(resp.failure, print_failure);

  //send some requests
  GetState g(&resp);
  ssrv.get(g); //should get std::exception here
  UpdateState up(string("state1"), &resp);
  ssrv.update(up);
  ssrv.get(g); 

  exec.shutdown();

  return 0;
}
 
