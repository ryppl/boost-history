/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */
 
#define BOOST_TEST_MODULE luidg_recover_fifo_throw_on_error
#include "test.hpp"

//____________________________________________________________________________//

struct LUIDG { 
    int i;
    typedef bluid::dsl::luidg<bluid::dsl::recover<bluid::dsl::fifo<> >
    , bluid::dsl::throw_on_error<> > type;
    BOOST_STATIC_ASSERT((boost::luid::dsl::is_on_overflow<bluid::dsl::throw_on_error<> >::value)); 
    BOOST_STATIC_ASSERT((boost::luid::dsl::is_throw_on_error<type::on_overflow>::value)); 
    BOOST_STATIC_ASSERT((!boost::luid::dsl::is_discard<type::on_release>::value)); 
    
    bluid::luidg<bluid::dsl::recover<bluid::dsl::fifo<> >
            , bluid::dsl::throw_on_error<> >  luidg;
    
    LUIDG() 
    : i(10)
    , luidg( _high=i ) { 
//        BOOST_TEST_MESSAGE( "setup fixture" ); 
    }
    ~LUIDG()         { 
//        BOOST_TEST_MESSAGE( "teardown fixture" ); 
    }
   
};

BOOST_FIXTURE_TEST_SUITE( luidg_recover_fifo_throw_on_error, LUIDG )

BOOST_AUTO_TEST_CASE( unique )
{
    test_unique(this->luidg);
}
BOOST_AUTO_TEST_CASE( overflow )
{
    test_overflow_throw(this->luidg);    
}

BOOST_AUTO_TEST_CASE( fifo )
{
    test_fifo(this->luidg);    
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
