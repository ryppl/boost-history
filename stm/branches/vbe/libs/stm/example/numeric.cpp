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

struct A {
    A() : i(0), ptr(&i) {
        
    }
    stm::tx::int_t i;
    stm::tx::pointer<stm::tx::int_t> const ptr;
};

struct B {
    B() : a_ptr() {}
    stm::tx::pointer<A> a_ptr;
};


A a;
B b;

bool test_ptr() {
    {
    const stm::tx::numeric<int> ci=10;
    const stm::tx::numeric<int> cj = 10;
    stm::tx::pointer<const stm::tx::numeric<int> > pc = &ci;
    stm::tx::pointer<const stm::tx::numeric<int> > pc2 = &cj;
    pc=&cj;
    stm::tx::pointer<const stm::tx::numeric<int> > const cpc = pc;
    //cpc=pc2; // this must not compile
    stm::tx::pointer<stm::tx::pointer<const stm::tx::numeric<int> > > ppc;
    }
    {
    const int ci=10;
    const int cj = 10;
    stm::tx::pointer<const int> pc = &ci;
    stm::tx::pointer<const int> pc2 = &cj;
    pc=&cj;
    stm::tx::pointer<const int> const cpc = pc;
    //cpc=pc2; // this must not compile
    stm::tx::pointer<stm::tx::pointer<const int> > ppc;
    }

    stm::tx::numeric<int> i;
    stm::tx::pointer<stm::tx::numeric<int> > p;
    stm::tx::pointer<stm::tx::numeric<int> > const cp = &i;

    atomic(_) {
        b.a_ptr=&a;
        b.a_ptr->i =1;
    } end_atom
    atomic(_) {
        BOOST_STM_TX_RETURN(_, (b.a_ptr->i==1)&&(*b.a_ptr->ptr==1));
    } end_atom
    return false;
}

bool test_array() {
    {
    int v[2];
    int * p;
    p = &v[0];
    p = v;
    ++p;
    }
    stm::tx::numeric<int> v[2];
    stm::tx::pointer<stm::tx::numeric<int> > p;
    p = &v[0];
    p = v;
    ++p;
    bool res=true;
    return res;
}

bool test_array_ptr() {
    {
        int * v= new int[2];
        int * p;
        p = &v[0];
        p = v;
        ++p;
    }
    atomic(_) {
        //stm::tx::pointer<stm::tx::numeric<int> > v= BOOST_STM_TX_NEW_ARRAY(_, 2, stm::tx::numeric<int>);
        stm::tx::pointer<stm::tx::numeric<int> > v= BOOST_STM_NEW_ARRAY(2, stm::tx::numeric<int>);

        stm::tx::pointer<stm::tx::numeric<int> > p;
        p = &v[0];
        p = v;
        ++p;
        bool res=true;
        BOOST_STM_RETURN(res);
    } end_atom
    #if 0
    {
        stm::tx::pointer<stm::tx::numeric<int> > v= BOOST_STM_NEW_ARRAY(2, stm::tx::numeric<int>);

        stm::tx::pointer<stm::tx::numeric<int> > p;
        p = &v[0];
        p = v;
        ++p;
    }
    #endif
        bool res=true;
        BOOST_STM_RETURN(res);
    return false;
}

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

int test_counter() {

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
    fails += !test_ptr();
    fails += !test_const(counter);
    return fails;
}

int main() {
    stm::transaction::enable_dynamic_priority_assignment();
    stm::transaction::do_deferred_updating();
    stm::transaction::initialize();
    stm::thread_initializer thi;

    return test_counter();

}
