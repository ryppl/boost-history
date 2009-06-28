/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#define BOOST_TEST_MODULE luidg_multi_process_internally_locked 
#include "test.hpp"

//____________________________________________________________________________//

struct LUIDG { 
    luid::make_luidg<bluid::dsl::multi_process<bluid::dsl::internally_locked<> > >::type luidg;
    
    LUIDG() 
    : luidg( _high=10 ) { 
    }
    ~LUIDG()         { 
    }
   
};
BOOST_FIXTURE_TEST_SUITE( multi_process_externally_locked, LUIDG )

BOOST_AUTO_TEST_CASE( unique )
{
    test_unique(this->luidg);
}
BOOST_AUTO_TEST_CASE( overflow )
{
    test_overflow(this->luidg);    
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
