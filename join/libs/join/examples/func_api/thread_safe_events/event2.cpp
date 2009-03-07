//
// event2.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

using namespace boost::join;

logger log1("log");

template <typename V>
class event: public actor {
public:
  typedef boost::function<void(V)> subscriber;
  struct subscription_data {
    subscriber callback_;
    subscription_data(subscriber cb) : callback_(cb) {}
    };
  typedef boost::shared_ptr<subscription_data> subscription;
private:
  std::vector<subscription> subs_;
  async<void()> ready; 
  async<void(subscription)> subscribe_priv;
  async<void(V,int)> post_priv;
public:
  subscription subscribe(subscriber cb) {
    subscription sub(new subscription_data(cb));
    subscribe_priv(sub);
    return sub;
  }
  async<void(subscription)> unsubscribe;
  void operator()(V v) { post(v); }
  void post(V v) { post_priv(v, 0); }
  event(executor *e) : actor(e) {
    //post chord has the lowest priority so that sub/unsub msgs will be processed first
    chord(post_priv, ready, &event::post_cb);
    chord(subscribe_priv, ready, &event::sub_cb);
    //set unsubscribe to lower priority so subscribe call will be processed first
    chord(unsubscribe, ready, &event::unsub_cb, 1);
    ready();
  }
private:
  void post_cb(async_o<void(V,int)> post, async_o<void()> r) {
    subscriber cb;
    if ((size_t)post.arg2 < subs_.size()) {
      cb = subs_[post.arg2]->callback_;
      if((size_t)(post.arg2+1) < subs_.size()) {
	//start next callback in another task
	post_priv(post.arg1, (post.arg2+1));
      }
    }
    ready();
    if(cb) cb(post.arg1);
  }
  void sub_cb(async_o<void(subscription)> sub, async_o<void()> r) {
    subs_.push_back(sub.arg1);
    ready();
  }
  void unsub_cb(async_o<void(subscription)> unsub, async_o<void()> r) {
    typename std::vector<subscription>::iterator iter;
    if((iter = std::find(subs_.begin(), subs_.end(), unsub.arg1)) != subs_.end()) {
      subs_.erase(iter);
    }
    ready();
  }
};


void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

void evt_consumer2(std::string evt_data) {
  log1.stream() << "evt_consumer2 recv: " << evt_data << logger::endl;
}

class Demo : public actor {
public:
  event<std::string> evt;
  async<void()> producer;
  async<void(int)> consumer;
  Demo(executor *e) : actor(e), evt(e) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<10; i++) {
      ostr << i;
      log1.stream() << "producer asynchronously sends evt [" << i << "]" << logger::endl;
      evt(ostr.str());
      ostr.str("");
      thread_sleep(1);
    }
  }
  void consumer_cb(async_o<void(int)> c) {
    event<std::string>::subscription sub;
    int times = (c.arg1 == 1)?10:5;
    for(int i=0; i<times; i++) {
      if (c.arg1 == 1) {
	if(i % 2 == 0) {
	  log1.stream() << "consumer[" << c.arg1 << "] asynchronously subscribe to event " << logger::endl;  
	  sub = evt.subscribe(boost::bind(&Demo::evt_consumer1, this, _1));
	} 
	else {
	  log1.stream() << "consumer[" << c.arg1 << "] asynchronously un-subscribe to event " << logger::endl;  
	  evt.unsubscribe(sub);
	}
	thread_sleep(1);
      } else {
	if(i % 2 == 1) {
	  log1.stream() << "consumer[" << c.arg1 << "] asynchronously subscribe to event " << logger::endl;  
	  sub = evt.subscribe(&evt_consumer2);
	} 
	else {
	  log1.stream() << "consumer[" << c.arg1 << "] asynchronously un-subscribe to event " << logger::endl;  
	  evt.unsubscribe(sub);
	}
	thread_sleep(2);
      }
    }
  }

  void evt_consumer1(std::string evt_data) {
    log1.stream() << "evt_consumer1 recv: " << evt_data << logger::endl;
  }
};

int main(int argc, char **argv) {
  executor exec(5);  //3 threads for producer(1)/consumers(2) and 2 for other async tasks
  Demo demo(&exec.execute);
  demo.producer();
  demo.consumer(1);
  demo.consumer(2);
  //main thread have to wait about 10 secs here, other executor's threads will exit 
  //and Demo will fail
  thread_sleep(10);
  exec.shutdown();
  return 0;
}
