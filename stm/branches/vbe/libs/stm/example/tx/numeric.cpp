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

#include <iostream>
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

    BOOST_STM_OUTER_TRANSACTION(_) {
        ++counter;
    } BOOST_STM_RETRY
}
void inc1() {
    stm::thread_initializer thi;

    BOOST_STM_OUTER_TRANSACTION(_) {
        counter+=1;
    } BOOST_STM_RETRY
}
void decr() {
    stm::thread_initializer thi;

    BOOST_STM_OUTER_TRANSACTION(_) {
        --counter;
    } BOOST_STM_RETRY
}
bool check(int val) {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, (counter==val));
    } BOOST_STM_RETRY
    return false;
}

bool test_equal() {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        counter=1;
        counter2=2;
        BOOST_STM_TX_GOTO(_, label1);
        counter2=3;
    } BOOST_STM_RETRY
    label1:
    BOOST_STM_TRANSACTION(_) {
        counter=2;
        //assert(counter==counter2);
        BOOST_STM_TX_RETURN(_, (counter==counter2));
    } BOOST_STM_RETRY
    return false;
}

bool test_diff() {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        counter=1;
        counter2=2;
        BOOST_STM_TX_GOTO(_, label1);
        counter2=3;
    } BOOST_STM_RETRY
    label1:
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, (counter!=counter2));
    } BOOST_STM_RETRY
    return false;
}

bool test_assign() {
    //thread_initializer thi;
    for(int i=0; i<2;++i)
    BOOST_STM_B_TRANSACTION_IN_LOOP(_) {
        counter=1;
        counter2=counter;
        continue;
        counter2=3;
    } BOOST_STM_RETRY_END(_)

    BOOST_STM_TRANSACTION(_) {
        //assert((counter==1) && (counter2==1) && (counter==counter2));
        BOOST_STM_TX_RETURN(_, (counter==1) && (counter2==1) && (counter==counter2)) ;
    } BOOST_STM_RETRY
    return false;
}

bool test_less() {
    //thread_initializer thi;
    for(;;) {
    BOOST_STM_B_TRANSACTION_IN_LOOP(_) {
        counter=1;
        counter2=2;
        break;
        counter2=0;
    } BOOST_STM_RETRY_END(_)
    }
    BOOST_STM_B_TRANSACTION(_) {
        return (counter<counter2) ;
    } BOOST_STM_RETRY_END(_)
    return false;
}

bool test_throw_e() {
    boost::stm::native_trans<int> x = 0;

    //thread_initializer thi;
    BOOST_STM_B_TRANSACTION(_) {
        counter=0; counter2=0;
    } BOOST_STM_RETRY_END(_)

    try{
    for(int i=0; i<2;++i) {
        BOOST_STM_B_TRANSACTION_IN_LOOP(_) {
            counter=1;
            throw 1;
            counter2=3;
        } BOOST_STM_RETRY_END_IN_LOOP(_)
    }
    } catch (...) {}

    BOOST_STM_B_TRANSACTION(_) {
        return (counter==0) && (counter2==0);
    } BOOST_STM_RETRY_END(_)

    return false;
}


bool test_assign_e() {
    //thread_initializer thi;
    for(int i=0; i<2;++i)
    BOOST_STM_B_TRANSACTION_IN_LOOP(_) {
        counter=1;
        counter2=counter;
        continue;
        counter2=3;
    } BOOST_STM_RETRY_END_IN_LOOP(_)

    BOOST_STM_B_TRANSACTION(_) {
        return (counter==1) && (counter2==1) && (counter==counter2) ;
    } BOOST_STM_RETRY_END(_)

    return false;
}

bool test_less_e() {
    //thread_initializer thi;
    for(;;) {
    BOOST_STM_B_TRANSACTION_IN_LOOP(_)
    {
        counter=1;
        counter2=2;
        break;
        counter2=0;
    } BOOST_STM_RETRY_END_IN_LOOP(_)
    }
    BOOST_STM_B_TRANSACTION(_)
    {
        return counter<counter2;
    } BOOST_STM_RETRY_END(_)
    return false;
}

bool test_le() {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        counter=1;
        counter2=1;
    } BOOST_STM_RETRY
    BOOST_STM_TRANSACTION(_) {
        //assert(counter<=counter2);
        BOOST_STM_TX_RETURN(_, (counter<=counter2)) ;
    } BOOST_STM_RETRY
    return false;
}

bool test_const(stm::tx::numeric<int> const& c) {
    //thread_initializer thi;
    BOOST_STM_TRANSACTION(_) {
        counter2=c;
    } BOOST_STM_RETRY
    BOOST_STM_TRANSACTION(_) {
        //assert(c==counter2);
        BOOST_STM_TX_RETURN(_, (c==counter2)) ;
    } BOOST_STM_RETRY
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
    fails += !test_diff();
    fails += !test_assign();
    //~ fails += !test_less();
    fails += !test_assign_e();
    fails += !test_less_e();
    fails += !test_throw_e();
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
