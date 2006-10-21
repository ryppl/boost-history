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


void test_bimap_unconstrained()
{
    using namespace boost::bimap;

    {
        typedef bimap<int,double,unconstrained_set_of_relation> bm;
        bm b;
        b.left[2] = 34.4;
        b.right[2.2] = 3;
    }

    {
        typedef bimap<int,unconstrained_set_of<double> > bm;
        bm b;
        b.left[2] = 34.4;
        BOOST_CHECK( b.size() == 1 );
    }

    {
        typedef bimap<unconstrained_set_of<int>, double > bm;
        bm b;
        b.right[2.4] = 34;
    }

    {
        typedef bimap<unconstrained_set_of<int>, double, right_based > bm;
        bm b;
        b.right[2.4] = 34;
        BOOST_CHECK( b.size() == 1 );
    }

    {
        typedef bimap<int,unconstrained_set_of<double>, unconstrained_set_of_relation > bm;
        bm b;
        b.left[2] = 34.4;
    }

    {
        typedef bimap<unconstrained_set_of<int>, double, unconstrained_set_of_relation > bm;
        bm b;
        b.right[2.4] = 34;
    }

    {
        typedef bimap<unconstrained_set_of<int>, unconstrained_set_of<double>, set_of_relation<> > bm;
        bm b;
        b.insert( bm::relation(1,2.3) );
        BOOST_CHECK( b.size() == 1 );
    }
}


int test_main( int, char* [] )
{
    test_bimap_unconstrained();
    return 0;
}

