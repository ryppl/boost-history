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

// std
#include <set>
#include <map>
#include <cstddef>
#include <cassert>
#include <algorithm>

// Boost.Test
#include <boost/test/minimal.hpp>

// std
#include <sstream>
#include <algorithm>
#include <set>

// Boost
#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>

// Boost.Bimap
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/bimap.hpp>

namespace ba =  boost::assign;

void test_bimap_assign()
{
    using namespace boost::bimap;

    typedef bimap<int, list_of<double> > bm;

    bm b = ba::list_of< bm::relation >(1,0.1)(2,0.2)(3,0.3);

    BOOST_CHECK( b.size() == 3 );

    // Since it is left_based the main view is a set, so we use insert

    ba::insert( b )
        (4,0.4)
        (5,0.5)
        (6,0.6)
        (7,0.7);

    BOOST_CHECK( b.size() == 7 );

    // The right map view is a list so we use push_back here

    ba::push_back( b.right )
        (0.8,8)
        (0.9,9);

    BOOST_CHECK( b.size() == 9 );
}


int test_main( int, char* [] )
{
    test_bimap_assign();
    return 0;
}

