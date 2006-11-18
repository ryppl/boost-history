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

    BOOST_CHECK( b.left[2] == 150 );

    b.left .modify( b.left.begin() , _second = 100 );

    BOOST_CHECK( b.left[2] == 100 );

    b.right.modify( b.right.begin(), ( _first = 200, _second = 1 ) );

    BOOST_CHECK( b.left[1] == 200 );
}

int test_main( int, char* [] )
{
    test_bimap_modify();

    return 0;
}

