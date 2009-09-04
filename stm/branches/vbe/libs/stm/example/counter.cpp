//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009. 
// (C) Copyright Vicente J. Botet Escriba 2009. 
// Distributed under the Boost
// Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/stm.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
//#include <string>
#include <vector>
#include <list>
//#include <iostream>
#include <stdlib.h>
#define foreach BOOST_FOREACH

using namespace std;
using namespace boost;
using namespace boost::stm;

stm::tx_ptr<int> counter;

void inc() {
    thread_initializer thi;
    
    use_atomic(_) {
        *counter+=*counter;
    }
}
void check() {
    //thread_initializer thi;
    
    use_atomic(_) {
        assert(*counter==4);
    }
}
int test_counter() {
    counter=make_tx_ptr<int>(0);
    
    thread  th1(inc);
    thread  th2(inc);
    thread  th3(inc);
    thread  th4(inc);

    th1.join(); 
    th2.join(); 
    th3.join(); 
    th4.join(); 
    
    check();
    boost::stm::delete_ptr(counter);
    return 0;
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
