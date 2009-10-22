//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <assert.h>
#include <list>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace boost;

stm::tx::int_t counter(0);
stm::tx::int_t counter2(0);

void inc() {
    stm::thread_initializer thi;

    atomic(_) {
        ++counter;
    } end_atom
}
void inc1() {
    stm::thread_initializer thi;

    atomic(_) {
        counter+=1;
    } end_atom
}
void decr() {
    stm::thread_initializer thi;

    atomic(_) {
        --counter;
    } end_atom
}
bool check(int val) {
    //thread_initializer thi;
    atomic(_) {
        BOOST_STM_TX_RETURN(_, (counter==val));
    } end_atom
    return false;
}

bool test_equal() {
    //thread_initializer thi;
    atomic(_) {
        counter=1;
        counter2=2;
        BOOST_STM_TX_GOTO(_, label1);
        counter2=3;
    } end_atom
    label1:
    atomic(_) {
        counter=2;
        //assert(counter==counter2);
        BOOST_STM_TX_RETURN(_, (counter==counter2));
    } end_atom
    return false;
}

bool test_assign() {
    //thread_initializer thi;
    atomic(_) {
        counter=1;
        counter2=counter;
        BOOST_STM_CONTINUE;
        counter2=3;
    } end_atom
    atomic(_) {
        //assert((counter==1) && (counter2==1) && (counter==counter2));
        BOOST_STM_TX_RETURN(_, (counter==1) && (counter2==1) && (counter==counter2)) ;
    } end_atom
    return false;
}

bool test_less() {
    //thread_initializer thi;
    atomic(_) {
        counter=1;
        counter2=2;
        BOOST_STM_BREAK;
        counter2=3;
    } end_atom
    atomic(_) {
        //assert(counter<counter2);
        BOOST_STM_TX_RETURN(_, (counter<counter2)) ;
    } end_atom
    return false;
}

bool test_le() {
    //thread_initializer thi;
    atomic(_) {
        counter=1;
        counter2=1;
    } end_atom
    atomic(_) {
        //assert(counter<=counter2);
        BOOST_STM_TX_RETURN(_, (counter<=counter2)) ;
    } end_atom
    return false;
}

bool test_const(stm::tx::numeric<int> const& c) {
    //thread_initializer thi;
    atomic(_) {
        counter2=c;
    } end_atom
    use_atomic(_) {
        //assert(c==counter2);
        BOOST_STM_TX_RETURN(_, (c==counter2)) ;
    }
    return false;
}

int test_all() {

    thread  th1(inc);
    thread  th2(decr);
    thread  th3(inc1);
    thread  th4(inc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    int fails=0;
    fails += !check(2);
    fails += !test_equal();
    fails += !test_assign();
    fails += !test_less();
    fails += !test_le();
    fails += !test_const(counter);
    return fails;
}

int main() {
    stm::transaction::enable_dynamic_priority_assignment();
    stm::transaction::do_deferred_updating();
    stm::transaction::initialize();
    stm::thread_initializer thi;

    return test_all();

}
