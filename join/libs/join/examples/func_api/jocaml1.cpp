//
// jocaml1.cpp
//
// Copyright (c) 2007, 2008  Yigong Liu (yigongliu at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/join/join.hpp>
#include <boost/bind.hpp>
//#include <iostream>

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

//define 2 plain functions as chord-body or message handlers
void apple_pie(async_o<void(string)> a, async_o<void()> p) {
  log1.stream() << a.arg1 << " apple pie" << logger::endl;
}

void raspberry_pie(async_o<void(string)> r, async_o<void()> p) {
  log1.stream() << r.arg1 << " raspberry pie" << logger::endl;
}

//define a class using its object's methods as chord-body or message handlers
class fruit_pie {
  int count;
public:
  fruit_pie(int c) : count(c) {}
  void apple(async_o<void(string)> a, async_o<void()> p) {
    log1.stream()  << a.arg1 << " apple pie " << count << logger::endl;
  }
  void raspberry(async_o<void(string)> r, async_o<void()> p) {
    log1.stream()  << r.arg1 << " raspberry pie " << count << logger::endl;
  }
  void blueberry(async_o<void(string)> r, async_o<void()> p) {
    log1.stream()  << r.arg1 << " blueberry pie " << count << logger::endl;
  }
  void strange(async_o<void(string)> a, async_o<void()> p) {
    log1.stream()  << "STRANGE " << a.arg1 << " apple pie " << count << logger::endl;
  }
};


int main(int argc, char **argv) {
  //define a few asynchronous message channels
  async<void(string)> apple;
  async<void(string)> raspberry;
  async<void(string)> blueberry;
  async<void()> pie;

  //create an executor with 2 threads in pool
  executor exec(2);  

  //test1: a new joins obj is created, used, reused and destroyed at end of scope
  try 
  {
    //define an actor/joint object with a few chords/join-patterns to specify 
    //what to do with messages at channels : apple(), raspberry() and pie()
    actor joins(&exec.execute);
    joins.chord(apple, pie, apple_pie)
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

    //here when going out of scope, "joins" is destroyed with its coordination/join patterns
    //for channels pie()/apple()/raspberry(); while the channels are left unattached.
  }
  catch (join_exception &e) {
    log1.stream()  << "Caught exception: " << e.what() << logger::endl;
  }

  //test2: another joins object is used
  try
  {
    //define an actor/joint object with a few chords
    fruit_pie fp2(2);
    actor joins(&exec.execute);
    joins.chord(apple, pie, bind(&fruit_pie::apple, fp2, _1, _2))
         .chord(raspberry, pie, bind(&fruit_pie::raspberry, fp2, _1, _2))
         .chord(blueberry, pie, bind(&fruit_pie::blueberry, fp2, _1, _2));

    //send some test messages
    pie(); pie();
    raspberry("sour");
    apple("dull");
    blueberry("sweet"); pie();

    //test delete a chord
    joins.chord_remove(raspberry, pie);
    //since channel raspberry is not associated with joins/actor anymore,
    //calling it will throw exception
    raspberry("bitter");
  }
  catch (join_exception &e) {
    log1.stream()  << "Caught bitter-raspberry exception: " << e.what() << logger::endl;
  }

  try {
    //test unbound async methods/channels
    //an exception should be thrown here since 
    //all joiners/actors are gone and async channel "pie" 
    //is not associated with any actor
    pie(); 
  }
  catch (join_exception &e) {
    log1.stream()  << "Caught unbound-pie exception: " << e.what() << logger::endl;
  }

  try {
    exec.shutdown();
  }
  catch(join_exception &e) {
    log1.stream()  << "Caught exception: " << e.what() << logger::endl;
  }

  return 0;
}
 
