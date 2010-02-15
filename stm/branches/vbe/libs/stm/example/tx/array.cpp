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


bool test_array() {
    {
        int v[2];
        int * p;
        p = &v[0];
        *p=1;
        p = v;
        ++p;
        *p=2;
    }
    stm::tx::int_t v[2];
    stm::tx::pointer<stm::tx::int_t > p;
    BOOST_STM_OUTER_TRANSACTION(_) {
        p = &v[0];
        *p=1;
        p = v;
        ++p;
        *p=2;
    } BOOST_STM_RETRY
    return (v[0]==1) && (v[1]==2);
}

bool test_array_ptr() {
    {
        int * v= new int[2];
        int * p;
        p = &v[0];
        p = v;
        ++p;
    }
    stm::tx::pointer<stm::tx::int_t > v;
    BOOST_STM_OUTER_TRANSACTION(_) {
        v= BOOST_STM_TX_NEW_ARRAY(_,2, stm::tx::int_t);

        stm::tx::pointer<stm::tx::int_t > p;
        p = &v[0];
        p = v;
        //++p;
    } BOOST_STM_RETRY

    #if 0
    {
        stm::tx::pointer<stm::tx::int_t > v= BOOST_STM_NEW_ARRAY(2, stm::tx::int_t);

        stm::tx::pointer<stm::tx::int_t > p;
        p = &v[0];
        p = v;
        ++p;
    }
    #endif
    bool res=true;
    BOOST_STM_RETURN(res);
    return false;
}


int test_all() {

    int fails=0;
    //fails += !test_array();
    fails += !test_array_ptr();
    return fails;
}

int main() {
    stm::transaction::enable_dynamic_priority_assignment();
    stm::transaction::do_deferred_updating();
    stm::transaction::initialize();
    stm::thread_initializer thi;
    return test_all();
}
