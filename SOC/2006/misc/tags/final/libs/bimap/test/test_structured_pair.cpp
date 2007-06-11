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

// std
#include <utility>
#include <cstddef>

// Boost.Static_assert
#include <boost/static_assert.hpp>

// Boost.Bimap
#include <boost/bimap/detail/test/check_metadata.hpp>
#include <boost/bimap/detail/test/check_size_of_pair.hpp>

#include <boost/bimap/relation/structured_pair.hpp>


BOOST_BIMAP_TEST_STATIC_FUNCTION( static_metadata_test )
{
    using namespace boost::bimap::relation;
    using namespace boost::bimap::detail::test;

    struct data_a { char     data; };
    struct data_b { double   data; };

    typedef structured_pair
    <
        data_a,
        data_b,
        normal_layout

    > sp_ab;

    typedef structured_pair
    <
        data_b,
        data_a,
        mirror_layout

    > sp_ba;

    check_size_of_pair<sp_ab,data_a,data_b> test_1;
    check_size_of_pair<sp_ba,data_a,data_b> test_2;

    BOOST_BIMAP_CHECK_METADATA(sp_ab, first_type , data_a);
    BOOST_BIMAP_CHECK_METADATA(sp_ab, second_type, data_b);

    BOOST_BIMAP_CHECK_METADATA(sp_ba, first_type , data_b);
    BOOST_BIMAP_CHECK_METADATA(sp_ba, second_type, data_a);

}


void test_basic()
{

    using namespace boost::bimap::relation;

    // Instanciate two pairs and test the storage alignmentDataData

    typedef structured_pair< short, double, normal_layout > pair_type;
    typedef structured_pair< double, short, mirror_layout > mirror_type;

    pair_type   pa( 2, 3.1416 );
    mirror_type pb( 3.1416, 2 );

    BOOST_CHECK( pa.first  == pb.second );
    BOOST_CHECK( pa.second == pb.first  );

}


int test_main( int, char* [] )
{

    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( static_are_storage_compatible_test );

    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( static_metadata_test );

    test_basic();

    return 0;
}

