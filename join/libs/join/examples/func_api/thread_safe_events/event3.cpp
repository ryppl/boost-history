//
// event1.cpp
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
  async<void()> subscription_empty; 
  synch<void()> process_subscription;
  async<void(subscription)> subscribe_priv;
public:
  synch<void(V)> post;
  subscription subscribe(subscriber cb) {
    subscription sub(new subscription_data(cb));
    subscribe_priv(sub);
    return sub;
  }
  async<void(subscription)> unsubscribe;
  void operator()(V v) { post(v); }
  event() {
    chord(post, ready, &event::post_cb);
    chord(process_subscription, subscribe_priv, &event::sub_cb);
    //set unsubscribe to lower priority so subscribe call will be processed first
    chord(process_subscription, unsubscribe, &event::unsub_cb,1);
    //create a lowest priority chord for stopping process_subscription recursion when all subs are processed
    chord(process_subscription, subscription_empty, &event::sub_empty_cb,2);
    ready();
    subscription_empty();
  }
private:
  void post_cb(synch_o<void(V)> post, async_o<void()> r) {
    process_subscription();
    for(size_t i=0; i<subs_.size(); i++)
      subs_[i]->callback_(post.arg1);
    ready();
  }
  void sub_cb(synch_o<void()> proc, async_o<void(subscription)> sub) {
    subs_.push_back(sub.arg1);
    process_subscription();
  }
  void unsub_cb(synch_o<void()> proc, async_o<void(subscription)> unsub) {
    typename std::vector<subscription>::iterator iter;
    if((iter = std::find(subs_.begin(), subs_.end(), unsub.arg1)) != subs_.end()) {
      subs_.erase(iter);
    }
    process_subscription();
  }
  void sub_empty_cb(synch_o<void()> proc, async_o<void()> empty) {
    subscription_empty();
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
  Demo(executor *e) : actor(e) {
    chord(producer, &Demo::producer_cb);
    chord(consumer, &Demo::consumer_cb);
  }
  void producer_cb(async_o<void()> p) {
    std::ostringstream ostr;
    for(int i=0; i<10; i++) {
      ostr << i;
      log1.stream() << "producer synchronously sends evt [" << i << "]" << logger::endl;
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
