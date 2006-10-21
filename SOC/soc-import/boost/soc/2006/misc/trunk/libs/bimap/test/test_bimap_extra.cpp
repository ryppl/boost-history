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
#include <boost/bimap/list_of.hpp>

void test_bimap_extra()
{
    using namespace boost::bimap;

    typedef bimap<int, unconstrained_set_of<double> > bm;

    bm b;
    b.left.insert( bm::left_value_type(1,0.1) );

    bm::left_iterator iter = (b.left.begin());
    iter->second = 0.2;

    b.begin()->right = 0.1;

}


int test_main( int, char* [] )
{
    test_bimap_extra();
    return 0;
}

