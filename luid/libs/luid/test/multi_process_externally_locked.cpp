/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#define BOOST_TEST_MODULE luidg_multi_process_externally_locked 
#include "test.hpp"

//____________________________________________________________________________//

struct LUIDG { 
    bip::interprocess_mutex            mutex;
    bip::scoped_lock<bip::interprocess_mutex>    lock;
    luid::make_luidg<bluid::dsl::multi_process<bluid::dsl::externally_locked<> > >::type luidg;
    
    LUIDG() 
    : mutex(), lock(mutex)
    , luidg( _high=10, _ext_mtx=mutex ) { 
//        BOOST_TEST_MESSAGE( "setup fixture" ); 
    }
    ~LUIDG()         { 
//        BOOST_TEST_MESSAGE( "teardown fixture" ); 
    }
   
};
BOOST_FIXTURE_TEST_SUITE( multi_process_externally_locked, LUIDG )

BOOST_AUTO_TEST_CASE( unique )
{
    test_unique(this->luidg.get(lock));
}
BOOST_AUTO_TEST_CASE( overflow )
{
    test_overflow(this->luidg.get(lock));    
}
BOOST_AUTO_TEST_CASE( resize_gt )
{
    test_resize(this->luidg.get(lock), 5, 20);    
}

BOOST_AUTO_TEST_CASE( resize_btw )
{
    test_resize(this->luidg.get(lock), 5, 8);    
}

BOOST_AUTO_TEST_CASE( resize_lt )
{
    test_resize(this->luidg.get(lock), 5, 3);    
}

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

// EOF
