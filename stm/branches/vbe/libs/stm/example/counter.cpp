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

    atomic(_) {
        ++counter;
    } end_atom
}
void decr() {
    thread_initializer thi;

    atomic(_) {
        --counter;
    } end_atom
}
bool check(int val) {
    //thread_initializer thi;
    bool res;
    atomic(_) {
        res =(*counter==val);
    } end_atom
    return res;
}

bool assign() {
    //thread_initializer thi;
    atomic(_) {
        counter=1;
        counter2=counter;
    } end_atom
    bool res;
    atomic(_) {
        res =(*counter==1) && (*counter2==1) && (counter==counter2) ;
    } end_atom
    return res;
}

bool test_const(stm::tx_obj<int> const& c) {
    //thread_initializer thi;
    atomic(_) {
        counter2=c;
    } end_atom
    bool res;
    atomic(_) {
        res =(c==counter2) ;
    } end_atom
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
