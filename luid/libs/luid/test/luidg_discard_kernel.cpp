/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#define BOOST_TEST_MODULE luidg_discard_kernel
#include "test.hpp"

//____________________________________________________________________________//

struct LUIDG { 
    int i;
    luid::generator<luid::dsl::luidg<bluid::dsl::discard<>, bluid::dsl::kernel<> > >::type luidg;
    
    LUIDG() 
    : i(10)
    , luidg( _high=i, _shm_name="ab", _shm_size=10000 ) { 
//        BOOST_TEST_MESSAGE( "setup fixture" ); 
    }
    ~LUIDG()         { 
//        BOOST_TEST_MESSAGE( "teardown fixture" ); 
    }
   
};
BOOST_FIXTURE_TEST_SUITE( luidg_discard_kernel, LUIDG )

BOOST_AUTO_TEST_CASE( unique )
{
    test_unique(this->luidg);
}
BOOST_AUTO_TEST_CASE( overflow )
{
    test_overflow(this->luidg);    
}
BOOST_AUTO_TEST_CASE( discard )
{
    test_discard(this->luidg);    
}

BOOST_AUTO_TEST_CASE( resize_gt )
{
    test_resize(this->luidg, 5, 20);    
}

BOOST_AUTO_TEST_CASE( resize_btw )
{
    test_resize(this->luidg, 5, 8);    
}

BOOST_AUTO_TEST_CASE( resize_lt )
{
    test_resize(this->luidg, 5, 3);    
}



BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

// EOF
