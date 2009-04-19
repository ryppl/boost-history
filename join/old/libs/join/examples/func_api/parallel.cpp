//
// parallel.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::join;

logger log1("log");

template <typename V>
class future: public actor {
public:
  synch<V()> get;
  V wait() { return get(); }
  future(executor *e) : actor(e) {}
};

template <typename InputIterator, typename UnaryFunction>
class async_loop: public future<void> {
  InputIterator first_;
  InputIterator last_;
  UnaryFunction f_;
  async<void()> done;
  async<void(int)> total; //total of bodies running
  async<void()> one_finish;
  async<void(InputIterator, int)> run_priv;
public:
  void run() {
    if (first_ == last_)
      done();
    else
      run_priv(first_, 0);
  }
  async_loop(executor *e, InputIterator first, InputIterator last, UnaryFunction f) : 
    future<void>(e), first_(first), last_(last), f_(f) {
    chord(run_priv, &async_loop::run_cb);
    chord(get, done, &async_loop::get_cb);
    chord(one_finish, total, &async_loop::finish_cb);
  }
private:
  void run_cb(async_o<void(InputIterator, int)> r) {
    InputIterator next = r.arg1;
    next++;
    if (next == last_) {//all loop bodies started
      total(r.arg2+1);
    }
    else {
      run_priv(next, r.arg2+1); //fire the next iteration
    }
    f_(*r.arg1);
    one_finish();
  }
  void finish_cb(async_o<void()> one, async_o<void(int)> tot) {
    if (tot.arg1 > 1)
      tot(tot.arg1-1);
    else
      done();
  }
  void get_cb(synch_o<void()> get, async_o<void()> done) {}
};


template <typename InputIterator, 
          typename ResultType, 
          typename MapFunction, 
          typename ReduceFunction>
class map_reduce_async: public future<ResultType> {
  InputIterator first_;
  InputIterator last_;
  ResultType result_;
  MapFunction map_fun_;
  ReduceFunction reduce_fun_;
  async<void()> done;
  async<void(int)> total; //total of bodies running
  async<void(InputIterator, int)> map_m;
  async<void(ResultType)> reduce_m;

public:
  map_reduce_async(typename future<ResultType>::executor *e, InputIterator first, InputIterator last, 
             ResultType init_val, MapFunction mf, ReduceFunction rf) : 
    future<ResultType>(e), first_(first), last_(last), result_(init_val),
    map_fun_(mf), reduce_fun_(rf) {
    chord(map_m, &map_reduce_async::map_cb);
    chord(reduce_m, total, &map_reduce_async::reduce_cb);
    chord(future<ResultType>::get, done, &map_reduce_async::get_cb);
  }
  void run() {
    if (first_ == last_)
      done();
    else
      map_m(first_, 0);
  }
private:
  void map_cb(async_o<void(InputIterator, int)> r) {
    InputIterator next = r.arg1;
    next++;
    if (next == last_) {//all loop bodies started
      total(r.arg2+1);
    }
    else {
      map_m(next, r.arg2+1); //fire the next iteration
    }
    reduce_m(map_fun_(*r.arg1));
  }
  void reduce_cb(async_o<void(ResultType)> reduce, async_o<void(int)> tot) {
    result_ = reduce_fun_(result_, reduce.arg1);
    if (tot.arg1 > 1)
      tot(tot.arg1-1);
    else
      done();
  }
  ResultType get_cb(synch_o<ResultType()> get, async_o<void()> done) {
    return result_;
  }
};


class parallel {
  actor::executor *exec_;
public:
  parallel(actor::executor *e) : exec_(e) {}
  
  template <typename InputIterator, typename UnaryFunction>
  future<void> *for_each(InputIterator first, InputIterator last, UnaryFunction f) {
    async_loop<InputIterator, UnaryFunction> *aloop = 
      new async_loop<InputIterator, UnaryFunction>(exec_, first, last, f);
    aloop->run();
    return aloop;
  }
  
  template <typename InputIterator, 
            typename ResultType, 
            typename MapFunction, 
            typename ReduceFunction>
  future<ResultType> *map_reduce(InputIterator first, InputIterator last, ResultType init, MapFunction mf, ReduceFunction rf) {
    map_reduce_async<InputIterator,ResultType,MapFunction,ReduceFunction> *mapred = 
      new map_reduce_async<InputIterator,ResultType,MapFunction,ReduceFunction>(exec_, first, last, init, mf, rf);
    mapred->run();
    return mapred;
  }
};

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int size = sizeof(data) / sizeof(int);

int sqr(int val) {
  //do some work
  for(int i=0; i<3; i++) {
    log1.stream() << "one map task calculating: " << val << " * " << val << " ...[" << i << "]" << logger::endl;
    thread_sleep(2);
  }
  return val * val;
}

int plus(int total, int one_more) {
  log1.stream() << "reduce task accumulates: " << one_more << logger::endl;
  return total + one_more;
}

int main(int argc, char **argv) {
  executor exec(6);  
  parallel para(&exec.execute);
  future<void> *f1 = para.for_each(data, data+size, sqr);
  log1.msg("main thread waiting for parallel.for_each() ...");
  f1->wait();
  log1.msg("parallel.for_each() is done, start parallel.map_reduce() ...");
  future<int> *f2 = para.map_reduce(data, data+size, 0, sqr, plus);
  log1.msg("main thread waiting for parallel.map_reduce() ...");
  log1.stream() << "parallel.map_reduce() returns: " << f2->get() << logger::endl;
  exec.shutdown();
  delete f1;
  delete f2;
  return 0;
}
