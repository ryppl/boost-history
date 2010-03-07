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
#include <boost/exception/info.hpp>
#include <boost/exception/errinfo_errno.hpp>

using namespace std;
using namespace boost;

stm::tx::int_t counter(0);
stm::tx::int_t counter2(0);

void reset() {

    BOOST_STM_OUTER_TRANSACTION(_) {
        counter=0;
    } BOOST_STM_RETRY
}

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
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, (counter==val));
    } BOOST_STM_RETRY
    return false;
}

bool test_equal() {
    reset();
    BOOST_STM_TRANSACTION(_) {
        counter=1;
        counter2=2;
        BOOST_STM_TX_GOTO(_, label1);
        counter2=3;
    } BOOST_STM_RETRY
    label1:
    BOOST_STM_TRANSACTION(_) {
        counter=2;
        BOOST_STM_TX_RETURN(_, (counter==counter2));
    } BOOST_STM_RETRY
    return false;
}

bool test_diff() {
    reset();
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
    reset();
    for(int i=0; i<2;++i) {
    BOOST_STM_TRANSACTION_IN_LOOP(_) {
        counter=1;
        counter2=counter;
        BOOST_STM_CONTINUE(_);
        counter2=3;
    } BOOST_STM_END_TRANSACTION_IN_LOOP(_)
    }
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, (counter==1) && (counter2==1) && (counter==counter2)) ;
    } BOOST_STM_RETRY
    return false;
}

bool test_less() {
    reset();
    for(;;) {
    BOOST_STM_TRANSACTION_IN_LOOP(_) {
        counter=1;
        counter2=2;
        BOOST_STM_BREAK(_);
        counter2=0;
    } BOOST_STM_END_TRANSACTION_IN_LOOP(_)
    }
    BOOST_STM_E_TRANSACTION
        BOOST_STM_E_RETURN(counter<counter2) ;
    BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool test_outer_throw() {
    reset();
    BOOST_STM_TRANSACTION(_) {
        counter=0;
    } BOOST_STM_END_TRANSACTION(_)

    try{
        BOOST_STM_TRANSACTION(_) {
            counter=1;
            throw "1";
        } BOOST_STM_END_TRANSACTION(_)
    } catch (...) {}

    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_,  (counter==0));
    } BOOST_STM_END_TRANSACTION(_)

    return true;
}
bool test_outer_throw_e() {
    reset();

    try{
        BOOST_STM_E_TRANSACTION {
            counter=1;
            throw "1";
        } BOOST_STM_E_END_TRANSACTION;
    } catch (...) {}

    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(counter==1);
    } BOOST_STM_E_END_TRANSACTION;

    return true;
}

bool test_outer_abort_throw_e() {
    reset();
    BOOST_STM_E_TRANSACTION {
        counter=0;
    } BOOST_STM_E_END_TRANSACTION;

    try{
        BOOST_STM_E_TRANSACTION {
            counter=1;
            BOOST_STM_E_ABORT_AND_THROW("1");
        } BOOST_STM_E_END_TRANSACTION;
    } catch (...) {}

    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(counter==0);
    } BOOST_STM_E_END_TRANSACTION;

    return true;
}


bool test_inner_throw() {
    reset();
    BOOST_STM_E_TRANSACTION {
        counter=0; counter2=0;
    } BOOST_STM_E_END_TRANSACTION;

    try{
        BOOST_STM_TRANSACTION(_) {
            counter=1;
            BOOST_STM_TRANSACTION(__) {
                throw "1";
            } BOOST_STM_END_TRANSACTION(_)
            catch (...){throw;}
            counter=3;
        } BOOST_STM_END_TRANSACTION(_)
    } catch (...) {}
    bool res=false;
    BOOST_STM_TRANSACTION(_) {
        //~ res=(counter==0) && (counter2==0);
        BOOST_STM_TX_RETURN(_,  (counter==0) && (counter2==0));
    } BOOST_STM_END_TRANSACTION(_)

    return res;
}

bool test_inner_throw_e() {
    reset();
    BOOST_STM_E_TRANSACTION {
        counter=0; counter2=0;
    } BOOST_STM_E_END_TRANSACTION;

    try{
        BOOST_STM_E_TRANSACTION {
            counter=1;
            BOOST_STM_E_TRANSACTION {
                counter=2;
                throw "1";
            } BOOST_STM_E_END_TRANSACTION;
            counter=3;
        } BOOST_STM_E_END_TRANSACTION;
    } catch (...) {
    }

    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(counter==2);
    } BOOST_STM_E_END_TRANSACTION;

    return false;
}

bool test_inner_abort_throw_e() {
    reset();
    BOOST_STM_E_TRANSACTION {
        counter=0; counter2=0;
    } BOOST_STM_E_END_TRANSACTION;

    try{
        BOOST_STM_E_TRANSACTION {
            counter=1;
            BOOST_STM_E_TRANSACTION {
                counter=2;
                BOOST_STM_E_ABORT_AND_THROW( "1");
            } BOOST_STM_E_END_TRANSACTION;
            counter=3;
        } BOOST_STM_E_END_TRANSACTION;
    } catch (...) {}

    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(counter==0);
    } BOOST_STM_E_END_TRANSACTION;

    return false;
}

bool test_nested_e() {
    reset();
    BOOST_STM_E_TRANSACTION {
        counter=1;
        BOOST_STM_E_TRANSACTION {
            counter2=2;
        } BOOST_STM_E_END_TRANSACTION;
    } BOOST_STM_E_END_TRANSACTION;

    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN((counter==1) && (counter2==2));
    } BOOST_STM_E_END_TRANSACTION;

    return false;
}

bool test_assign_e() {
    reset();
    for(int i=0; i<2;++i) {
        BOOST_STM_E_TRANSACTION_IN_LOOP {
            counter=1;
            counter2=counter;
            continue;
            counter2=3;
        } BOOST_STM_E_END_TRANSACTION_IN_LOOP;
    }
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN((counter==1) && (counter2==1) && (counter==counter2)) ;
    } BOOST_STM_E_END_TRANSACTION;

    return false;
}

bool test_less_e() {
    reset();
    for(;;) {
        BOOST_STM_E_TRANSACTION_IN_LOOP {
            counter=1;
            counter2=2;
            break;
            counter2=0;
        } BOOST_STM_E_END_TRANSACTION_IN_LOOP;
    }
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(counter<counter2);
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}

bool test_le() {
    reset();
    BOOST_STM_TRANSACTION(_) {
        counter=1;
        counter2=1;
    } BOOST_STM_RETRY
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TX_RETURN(_, (counter<=counter2)) ;
    } BOOST_STM_RETRY
    return false;
}

bool test_const(stm::tx::numeric<int> const& c) {
    reset();
    BOOST_STM_TRANSACTION(_) {
        counter2=c;
    } BOOST_STM_RETRY
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_RETURN(c==counter2) ;
    } BOOST_STM_E_END_TRANSACTION;
    return false;
}
bool test_par() {
    reset();
    thread  th1(inc);
    thread  th2(decr);
    thread  th3(inc1);
    thread  th4(inc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    return check(2);
}

bool test_twice() {
    BOOST_STM_TRANSACTION(_) {
        BOOST_STM_TRANSACTION(_) {
        } BOOST_STM_END_TRANSACTION(_)
    } BOOST_STM_END_TRANSACTION(_)

    return true;
}

bool test_twice_e() {
    BOOST_STM_E_TRANSACTION {
        BOOST_STM_E_TRANSACTION {
        } BOOST_STM_E_END_TRANSACTION;
    } BOOST_STM_E_END_TRANSACTION;

    return true;
}
bool test_output() {
    std::stringstream strstr;
    BOOST_STM_E_TRANSACTION {
        strstr << counter;
    } BOOST_STM_E_END_TRANSACTION;

    return (strstr.str()=="0");
}
bool test_input() {
    std::stringstream strstr("2\n");
    BOOST_STM_E_TRANSACTION {
        strstr >> counter;
        BOOST_STM_E_RETURN(2==counter) ;
    } BOOST_STM_E_END_TRANSACTION;

    return false;
}

int test_all() {
    int fails=0;

    //~ fails += !test_twice();
    //~ fails += !test_twice();
    //~ fails += !test_twice_e();
    //~ fails += !test_twice_e();
    fails += !test_inner_throw_e();
    fails += !test_outer_throw();
    fails += !test_outer_throw_e();
    fails += !test_inner_abort_throw_e();
    fails += !test_outer_abort_throw_e();

    fails += !test_equal();
    fails += !test_diff();
    fails += !test_assign();
    fails += !test_assign_e();
    fails += !test_less();
    fails += !test_less_e();
    fails += !test_nested_e();
    fails += !test_le();
    fails += !test_const(counter);
    fails += !test_output();
    fails += !test_input();

    fails += !test_par();

    return fails;
}
void term_hd() {
    std::cout << "****************** ERROR: "<< __FILE__ << "["<<__LINE__<<"] term_hd"<< std::endl;
    std::abort();
}
int main() {
    try {
    stm::transaction::enable_dynamic_priority_assignment();
    stm::transaction::do_deferred_updating();
    stm::transaction::initialize();
    stm::thread_initializer thi;

    return test_all();
    } 
    CATCH_AND_PRINT_ALL

}
