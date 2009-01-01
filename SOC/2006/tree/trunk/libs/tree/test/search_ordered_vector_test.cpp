//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/searcher.hpp>

#include <vector>

#define BOOST_TEST_MODULE search_ordered_vector test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( search_ordered_vector_test )
{
//    using boost::tree::searcher;
//    using std::vector;
//    
//    typedef searcher<false, vector<int> > searcher_t;
//    searcher_t my_searcher;
//
//    searcher_t::cursor c, c1, c2, c3, c4, c5;
//    
//    c = my_searcher.end();
//
//    c1 = my_searcher.insert(c, 18);
//    c1 = my_searcher.insert(c1, 7);        //FIXME: crash if pos hint == c
//    c1 = my_searcher.insert(c1, 6);
//    c1 = my_searcher.insert(c1, 8);
//
//    c1 = my_searcher.begin();
//    BOOST_CHECK_EQUAL(*c1++, 6);
//    BOOST_CHECK_EQUAL(*c1++, 7);
//    BOOST_CHECK_EQUAL(*c1++, 8);
//    BOOST_CHECK_EQUAL(*c1++, 18);
//    BOOST_CHECK_EQUAL(c1, my_searcher.end());
}

//boost::unit_test::test_suite*
//init_unit_test_suite( int argc, char* argv[] )
//{
//    boost::unit_test::test_suite* ordered_vector_test = 
//            BOOST_TEST_SUITE( "Ordered vector test" );
//
//    ordered_vector_test->add( BOOST_TEST_CASE( &search_ordered_vector_test ) );
//
//    return ordered_vector_test;
//}