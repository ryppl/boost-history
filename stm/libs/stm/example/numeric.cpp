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
    A() : i(0), ptr(&i) {}
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

    use_atomic(_) {
        b.a_ptr=&a;
        b.a_ptr->i =1;
    }
    bool res;
    use_atomic(_) {
        res =(b.a_ptr->i==1)&&(*b.a_ptr->ptr==1);
    }
    return res;
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

void inc() {
    stm::thread_initializer thi;

    use_atomic(_) {
        ++counter;
    }
}
void inc1() {
    stm::thread_initializer thi;

    use_atomic(_) {
        counter+=1;
    }
}
void decr() {
    stm::thread_initializer thi;

    use_atomic(_) {
        --counter;
    }
}
bool check(int val) {
    //thread_initializer thi;
    bool res;
    use_atomic(_) {
        res =(counter==val);
    }
    return res;
}

bool test_equal() {
    //thread_initializer thi;
    use_atomic(_) {
        counter=1;
        counter2=2;
    }
    bool res;
    use_atomic(_) {
        counter=2;
        //assert(counter==counter2);
        res =(counter==counter2);
    }
    return res;
}

bool test_assign() {
    //thread_initializer thi;
    use_atomic(_) {
        counter=1;
        counter2=counter;
    }
    bool res;
    use_atomic(_) {
        //assert((counter==1) && (counter2==1) && (counter==counter2));
        res =(counter==1) && (counter2==1) && (counter==counter2) ;
    }
    return res;
}

bool test_less() {
    //thread_initializer thi;
    use_atomic(_) {
        counter=1;
        counter2=2;
    }
    bool res;
    use_atomic(_) {
        //assert(counter<counter2);
        res =(counter<counter2) ;
    }
    return res;
}

bool test_le() {
    //thread_initializer thi;
    use_atomic(_) {
        counter=1;
        counter2=1;
    }
    bool res;
    use_atomic(_) {
        //assert(counter<=counter2);
        res =(counter<=counter2) ;
    }
    return res;
}
bool test_const(stm::tx::numeric<int> const& c) {
    //thread_initializer thi;
    use_atomic(_) {
        counter2=c;
    }
    bool res;
    use_atomic(_) {
        //assert(c==counter2);
        res =(c==counter2) ;
    }
    return res;
}

int test_counter() {

    int fails=0;
    #if 1
    thread  th1(inc);
    thread  th2(decr);
    thread  th3(inc1);
    thread  th4(inc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    fails += !check(2);
    #endif
    fails += !test_equal();
    fails += !test_assign();
    fails += !test_less();
    fails += !test_le();
    fails += !test_ptr();
    fails += !test_const(counter);
    std::cout << "fails=" << fails << std::endl;
    return fails;
}

int main() {
    stm::transaction::enable_dynamic_priority_assignment();
    stm::transaction::do_deferred_updating();
    stm::transaction::initialize();
    stm::thread_initializer thi;

    return test_counter();

}
