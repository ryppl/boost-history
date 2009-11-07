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

int counter;
int counter2;

void inc() {
    thread_initializer thi;

    BOOST_STM_ATOMIC(_) {
        non_tx::wr_ptr<int> tx_counter(t, counter);
        ++(*tx_counter);
    } BOOST_STM_END_ATOMIC
}
void decr() {
    thread_initializer thi;

    BOOST_STM_ATOMIC(_) {
        non_tx::wr_ptr<int> tx_counter(_, counter);
        --(*tx_counter);
    } BOOST_STM_END_ATOMIC
}
bool check(int val) {
    //thread_initializer thi;
    bool res;
    BOOST_STM_ATOMIC(_) {
        non_tx::rd_ptr<int> tx_counter(_, counter);
        res =(*tx_counter==val);
    } BOOST_STM_END_ATOMIC
    return res;
}

bool assign() {
    //thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        non_tx::wr_ptr<int> tx_counter(_, counter);
        non_tx::wr_ptr<int> tx_counter2(_, counter2);
        *tx_counter=1;
        *tx_counter2=*tx_counter;
    } BOOST_STM_END_ATOMIC
    bool res;
    BOOST_STM_ATOMIC(_) {
        non_tx::rd_ptr<int> tx_counter(_, counter);
        non_tx::rd_ptr<int> tx_counter2(_, counter2);
        res =(*tx_counter==1) && (*tx_counter2==1) && (tx_counter==tx_counter2) ;
    } BOOST_STM_END_ATOMIC
    return res;
}

bool test_const(int const& c) {
    //thread_initializer thi;
    BOOST_STM_ATOMIC(_) {
        non_tx::rd_ptr<int> tx_c(_, c);
        non_tx::wr_ptr<int> tx_counter2(_, counter2);
        *tx_counter2=*tx_c;
    } BOOST_STM_END_ATOMIC
    bool res;
    BOOST_STM_ATOMIC(_) {
        non_tx::rd_ptr<int> tx_c(_, c);
        non_tx::wr_ptr<int> tx_counter2(_, counter2);
        res =(*tx_c==*tx_counter2) ;
    } BOOST_STM_END_ATOMIC
    return res;
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

    bool fails=false;
    fails = fails || !check(2);
    fails = fails || !assign();
    fails = fails || !test_const(counter);
    return fails;
}

int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    return test_counter();

}
