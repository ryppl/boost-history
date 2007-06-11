// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
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

    typedef bimap<int,double> bm;

    bm b;
    b.insert( bm::relation(1,0.1) );

    BOOST_CHECK( b.left[1] == 0.1 );

    b.left.replace( b.left.begin(), bm::relation::left_pair(1,0.15) );

    BOOST_CHECK( b.left[1] == 0.15 );

    b.left .modify( b.left.begin() , _first = 2 );
    b.left .modify( b.left.begin() , _second = 0.1 );
    b.right.modify( b.right.begin(), ( _first = 0.1, _second = 1 ) );

    BOOST_CHECK( b.left[1] == 0.1 );

    b.left [  1] = 0.2;
    b.right[0.2] =   2;

    BOOST_CHECK( b.left[2] == 0.2 );

    BOOST_CHECK( b.size() == 1 );

    b.replace( b.begin(), bm::relation( 3, 0.3 ) );

    BOOST_CHECK( b.left[3] == 0.3 );

    b.modify( b.begin(), ( _left = 2, _right = 0.2 ) );

    BOOST_CHECK( b.left[2] == 0.2 );

    b.left [  1] = 0.1;

    BOOST_CHECK( b.right[0.1] == 1 );
    BOOST_CHECK( b.size() == 2 );

    // Duplicated value test
    {
        bool duplicate_value_test_passed = false;

        b.clear();
        b.left[1] = 0.1;

        try
        {
            b.left[2] = 0.1;
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
        b.left[1] = 0.1;

        try
        {
            bool comp;
            comp = b.left[2] < 0.2;
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

