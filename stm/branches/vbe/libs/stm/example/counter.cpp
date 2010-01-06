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
#include <list>
#include <stdlib.h>

using namespace std;
using namespace boost;
using namespace boost::stm;

stm::tx_obj<int> counter(0);
stm::tx_obj<int> counter2(0);

void inc() {
    thread_initializer thi;

    BOOST_STM_ATOMIC(_) {
        ++counter;
    } BOOST_STM_END_ATOMIC
}
void decr() {
    thread_initializer thi;

    BOOST_STM_ATOMIC(_) {
        --counter;
    } BOOST_STM_END_ATOMIC
}
bool check(int val) {
    //thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        BOOST_STM_TX_RETURN(_, *counter==val);
    } BOOST_STM_END_ATOMIC
}

bool assign() {
    //thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        counter=1;
        counter2=counter;
    } BOOST_STM_END_ATOMIC
    bool res;
    BOOST_STM_ATOMIC(_) {
        BOOST_STM_TX_RETURN(_, (*counter==1) && (*counter2==1) && (counter==counter2)) ;
    } BOOST_STM_END_ATOMIC
    return res;
}

bool test_const(stm::tx_obj<int> const& c) {
    //thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        counter2=c;
    } BOOST_STM_END_ATOMIC
    BOOST_STM_ATOMIC(_) {
        BOOST_STM_TX_RETURN(_, c==counter2) ;
    } BOOST_STM_END_ATOMIC
}

int test_counter() {
    //counter=make_tx_ptr<int>(0);

    thread  th1(inc);
    thread  th2(decr);
    thread  th3(inc);
    thread  th4(inc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    int fails=0;
    fails += !check(2);
    fails += !assign();
    fails += !test_const(counter);
    return fails;
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;
    srand(time(0));

    test_counter();

    return 0;

}
