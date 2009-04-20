//
// async_call_ret.cpp
//
// adapted from Cw sample
//
#include <boost/join/join.hpp>
#include "boost/tuple/tuple.hpp"
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::join;

logger log1("log");

//server interface: expose an async ports taking requests 
//in the form of a string name and a response ports
class IService {
public:
  async<tuple<string, async<string> > > Service;
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

//server class, implement the server interface:
//for each incoming request <name, respChan>, sleep a few seconds
//send "name is done" to respChan
class MyService : public joint, public IService {
  int n;
public:
  MyService(int m, joint::spawn_type e) : joint(e), n(m) {
    chord(Service, &MyService::service_fun);
  }
  void service_fun(tuple<string, async<string> > req) {
    string srvName = tuples::get<0>(req);
    async<string> replyChan = tuples::get<1>(req);
    for(int i=0; i<n; i++) {
      log1.stream() << srvName << " does " << i << logger::endl;
      thread_sleep((n%2)?1:2);
    }
    replyChan(srvName+" is done");
  }
};

//clients use Join2 to wait for responses from 2 servers
class Result2 : public joint {
public:
  async<string> first;
  async<string> second;
  synch<tuple<string,string>,void> Wait;
  Result2() {
    chord(Wait, first, second, &Result2::wait_body);
  }
  tuple<string,string> wait_body(void_t w, string f, string s) {
    return make_tuple(f,s);
  }
};

int main(int argc, char **argv) {
  executor exec(2);
  //create 2 server threads
  MyService s1(5, exec.execute);
  MyService s2(10, exec.execute);
  //main thread will play the role of clients
  //create a Result2 for waiting for results from 2 servers
  Result2 j;
  //send to 2 servers requests with response ports packed inside
  s1.Service(make_tuple("Service 1", j.first));
  s2.Service(make_tuple("Service 2", j.second));
  //client idle
  for(int i=0; i<7; i++) {
    log1.stream() << "client idle " << i << logger::endl;
    thread_sleep(1);
  }
  string x,y;
  //client blocking here waiting for responses from servers
  log1.stream() << "client block wait " << logger::endl;
  tie(x,y) = j.Wait();
  log1.stream() << "first result = " << x << ", second result = " << y << logger::endl;
  exec.shutdown();
  return 0;
}

