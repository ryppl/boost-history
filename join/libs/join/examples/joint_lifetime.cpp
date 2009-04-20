//
// jocaml1.cpp
//
// Copyright (c) 2007 - 2009  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/bind.hpp>

using namespace boost::join;
using namespace boost;
using namespace std;

logger log1("log");

void thread_sleep(int sec) {
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += sec;
    boost::thread::sleep(xt);
}

//define 2 plain functions as message handlers
void apple_pie(string a, void_t) {
  log1.stream() << a << " apple pie" << logger::endl;
}

void raspberry_pie(string r, void_t) {
  log1.stream() << r << " raspberry pie" << logger::endl;
}

//define a class using its object's methods as message handlers
class fruit_pie {
  int count;
public:
  fruit_pie(int c) : count(c) {}
  void apple(string a, void_t) {
    log1.stream()  << a << " apple pie " << count << logger::endl;
  }
  void raspberry(string r, void_t) {
    log1.stream()  << r << " raspberry pie " << count << logger::endl;
  }
  void blueberry(string r, void_t) {
    log1.stream()  << r << " blueberry pie " << count << logger::endl;
  }
  void strange(string a, void_t) {
    log1.stream()  << "STRANGE " << a << " apple pie " << count << logger::endl;
  }
};


int main(int argc, char **argv) {
  //define a few asynchronous message ports
  async<string> apple;
  async<string> raspberry;
  async<string> blueberry;
  async<void> pie;

  //create an executor with 2 threads in pool
  executor exec(2);  

  //test1: a new joint obj is created, used, reused and destroyed at end of scope
  try 
  {
    //define an joint/joint object with a few chords/join-patterns to specify 
    //what to do with messages at ports : apple(), raspberry() and pie()
    joint joins(exec.execute);
    joins
      .chord(apple, pie, apple_pie)
      .chord(raspberry, pie, raspberry_pie);

    //send a few messages to test
    pie(); pie();
    raspberry("green");
    apple("red");

    //clear all chords (join patterns or coordination definitions) in "joins" to reuse it 
    joins.reset(); 

    //redefine 2 chords with different processing code
    fruit_pie fp1(1);
    joins.chord(apple, pie, bind(&fruit_pie::apple, fp1, _1, _2))
         .chord(raspberry, pie, bind(&fruit_pie::raspberry, fp1, _1, _2));
    
    //send more test messages
    pie(); pie();
    raspberry("purple");
    apple("yellow");

    //override chord <apple,pie> to print diff msg
    joins.chord_override(apple, pie, bind(&fruit_pie::strange, fp1, _1, _2));
    apple("white"); pie();

    //add a new chord into existing chords
    joins.chord(blueberry, pie, bind(&fruit_pie::blueberry, fp1, _1, _2));
    blueberry("dark"); pie();

    try {
      //test unbound async methods/ports
      //an exception should be thrown here since 
      //all joiners/joints are gone and async port "pie" 
      //is not associated with any joint
      pie.detach();
      pie(); 
    }
    catch (join_exception &e) {
      log1.stream()  << "Caught unbound-pie exception: " << e.what() << logger::endl;
    }

    //must reset this joint to reuse the ports with another joint
    joins.reset();
  }
  catch (join_exception &e) {
    log1.stream()  << "Caught exception: " << e.what() << logger::endl;
  }

  //test2: another "unamed" joint object is used
  try
  {
    //define an joint object with 3 heartbeats (its chords can fire 3 times, then it will self-destroy)
    fruit_pie fp2(2);
    joins(exec.execute, 3)
      .chord(apple, pie, bind(&fruit_pie::apple, fp2, _1, _2))
      .chord(raspberry, pie, bind(&fruit_pie::raspberry, fp2, _1, _2))
      .chord(blueberry, pie, bind(&fruit_pie::blueberry, fp2, _1, _2));

    //send some test messages
    pie(); pie();
    raspberry("sour");
    apple("dull");
    blueberry("sweet"); pie();

    //since joint heartbeat count expires here, it should be destroyed and all ports detached,
    //calling ports now will throw exception
    raspberry("bitter");
  }
  catch (join_exception &e) {
    log1.stream()  << "Caught bitter-raspberry exception: " << e.what() << logger::endl;
  }

  try {
    exec.shutdown();
  }
  catch(join_exception &e) {
    log1.stream()  << "Caught exception: " << e.what() << logger::endl;
  }

  return 0;
}
 
