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

typedef native_trans<int> tx_int;
typedef transaction_object_ptr<tx_int> tx_int_ptr;
typedef transaction_object_ptr<const tx_int> tx_int_const_ptr;

tx_int counter;
tx_int_ptr counter_ptr, counter2_ptr;
//tx_int_const_ptr counter_const_ptr;

void inc() {
    thread_initializer thi;

    use_atomic(_) {
        // ++(*counter_ptr)
        read_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        //tx_int_ptr tmp = *tx_counter_ptr_ptr;
        write_ptr<tx_int> tx_counter_ptr(_, **tx_counter_ptr_ptr);
        ++(*tx_counter_ptr);
    }
}
void decr() {
    thread_initializer thi;

    use_atomic(_) {
        // --(*counter_ptr)
        read_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        write_ptr<tx_int> tx_counter_ptr(_, **tx_counter_ptr_ptr);
        --(*tx_counter_ptr);
    }
}
bool check(int val) {
    //thread_initializer thi;
    bool res;
    use_atomic(_) {
        // *counter_ptr==val
        read_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        read_ptr<tx_int> tx_counter_ptr(_, **tx_counter_ptr_ptr);
        res =(*tx_counter_ptr==val);
    }
    return res;
}

bool assign() {
    //thread_initializer thi;
    use_atomic(_) {
        // *tx_counter2_ptr=*tx_counter_ptr;
        read_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        write_ptr<tx_int_ptr> tx_counter2_ptr_ptr(_, counter2_ptr);
        tx_counter2_ptr_ptr=tx_counter_ptr_ptr;
    }
    bool res=true;
    use_atomic(_) {
        //res = (counter2_ptr==counter_ptr) ;
        read_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        read_ptr<tx_int_ptr> tx_counter2_ptr_ptr(_, counter2_ptr);
        //res = (*tx_counter2_ptr_ptr==*tx_counter_ptr_ptr) ;
        //res= (_.read(counter_ptr)==_.read(counter2_ptr));
    }
    return res;
}
#if 0
bool test_const(tx_int_const_ptr& const  ptr) {
    //thread_initializer thi;
    use_atomic(_) {
        write_ptr<tx_int_const_ptr> tx_counter_const_ptr_ptr(_, counter_const_ptr);
        tx_counter_const_ptr_ptr=ptr;
    }
    bool res=true;
    use_atomic(_) {
        //res =(c==counter2) ;
    }
    return res;
}
#endif

int test_counter() {
    use_atomic(_) {
        write_ptr<tx_int_ptr> tx_counter_ptr_ptr(_, counter_ptr);
        *tx_counter_ptr_ptr=BOOST_STM_NEW(_, tx_int());
    }
    thread  th1(inc);
    thread  th2(decr);
    thread  th3(inc);
    thread  th4(inc);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    int fails=0;
    fails += check(2);
    fails += !assign();
    //fails += !test_const(counter);
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
