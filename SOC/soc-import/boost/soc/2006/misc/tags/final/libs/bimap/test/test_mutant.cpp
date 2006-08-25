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

// Boost.MPL
#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>

// Boost.Bimap
#include <boost/bimap/relation/detail/mutant.hpp>

using namespace boost::bimap::relation::detail;

// The mutant idiom is standard if only POD types are used.

typedef double  type_a;
typedef int     type_b;

const type_a value_a = 1.4;
const type_b value_b = 3;

struct Data
{
    type_a a;
    type_b b;
};

struct StdPairView
{
    typedef type_a first_type;
    typedef type_b second_type;
    type_a first;
    type_b second;
};

struct ReverseStdPairView
{
    typedef type_a second_type;
    typedef type_b first_type;
    type_a second;
    type_b first;
};


struct MutantData
{
    typedef boost::mpl::list< StdPairView, ReverseStdPairView > mutant_views;

    MutantData(type_a ap, type_b bp) : a(ap), b(bp) {}
    type_a a;
    type_b b;
};


void test_mutant_basic()
{

    // mutant test
    {
        MutantData m(value_a,value_b);

        BOOST_CHECK( sizeof( MutantData ) == sizeof( StdPairView ) );

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).first  == value_b );
        BOOST_CHECK( mutate<ReverseStdPairView>(m).second == value_a );

        ReverseStdPairView & rpair = mutate<ReverseStdPairView>(m);
        rpair.first = value_b;
        rpair.second = value_a;

        BOOST_CHECK( mutate<StdPairView>(m).first  == value_a );
        BOOST_CHECK( mutate<StdPairView>(m).second == value_b );

        BOOST_CHECK( &mutate<StdPairView>(m).first  == &m.a );
        BOOST_CHECK( &mutate<StdPairView>(m).second == &m.b );
    }
}

int test_main( int, char* [] )
{
    test_mutant_basic();
    return 0;
}
