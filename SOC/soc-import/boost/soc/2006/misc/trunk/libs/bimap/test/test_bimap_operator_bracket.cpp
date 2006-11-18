// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.Bimap
#include <boost/bimap/support/lambda.hpp>
#include <boost/bimap/bimap.hpp>

void test_bimap_modify()
{
    using namespace boost::bimap;

    typedef bimap<int,long> bm;

    bm b;
    b.insert( bm::relation(1,100) );

    BOOST_CHECK( b.left[1] == 100 );

    b.left.replace( b.left.begin(), bm::relation::left_pair(1,150) );

    BOOST_CHECK( b.left[1] == 150 );

    b.left .modify( b.left.begin() , _first = 2 );
    b.left .modify( b.left.begin() , _second = 100 );
    b.right.modify( b.right.begin(), ( _first = 100, _second = 1 ) );

    BOOST_CHECK( b.left[1] == 100 );

    b.left [  1] = 200;
    b.right[200] =   2;

    BOOST_CHECK( b.left[2] == 200 );

    BOOST_CHECK( b.size() == 1 );

    b.replace( b.begin(), bm::relation( 3, 300 ) );

    BOOST_CHECK( b.left[3] == 300 );

    b.modify( b.begin(), ( _left = 2, _right = 200 ) );

    BOOST_CHECK( b.left[2] == 200 );

    b.left [  1] = 100;

    BOOST_CHECK( b.right[100] == 1 );
    BOOST_CHECK( b.size() == 2 );

    // Duplicated value test
    {
        bool duplicate_value_test_passed = false;

        b.clear();
        b.left[1] = 100;

        try
        {
            b.left[2] = 100;
        }
        catch( duplicate_value & )
        {
            duplicate_value_test_passed = true;
        }

        BOOST_CHECK( duplicate_value_test_passed );
    }

    // Value not found test
    {
        bool value_not_found_test_passed = false;

        b.clear();
        b.left[1] = 100;

        try
        {
            bool comp;
            comp = b.left[2] < 200;
        }
        catch( value_not_found & )
        {
            value_not_found_test_passed = true;
        }

        BOOST_CHECK( value_not_found_test_passed );
    }
}


int test_main( int, char* [] )
{
    test_bimap_modify();
    return 0;
}

