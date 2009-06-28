/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#define BOOST_TEST_MODULE luidg_multi_process
#include "test.hpp"

//____________________________________________________________________________//

struct LUIDG { 
    int i;
    luid::luidg<bluid::dsl::multi_process<> > luidg;
    
    LUIDG() 
    : i(10)
    , luidg( _high=i ) { 
//        BOOST_TEST_MESSAGE( "setup fixture" ); 
    }
    ~LUIDG()         { 
//        BOOST_TEST_MESSAGE( "teardown fixture" ); 
    }
   
};
BOOST_FIXTURE_TEST_SUITE( default_luidg, LUIDG )

BOOST_AUTO_TEST_CASE( unique )
{
    test_unique(this->luidg);
}
BOOST_AUTO_TEST_CASE( overflow )
{
    test_overflow(this->luidg);    
}

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

// EOF
