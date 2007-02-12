// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Boost.Test
#include <boost/test/minimal.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

// Boost.Bimap
#include <boost/bimap/support/lambda.hpp>
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/list_of.hpp>

// Support metafunctions
#include <boost/bimap/support/data_type_by.hpp>
#include <boost/bimap/support/key_type_by.hpp>
#include <boost/bimap/support/map_type_by.hpp>
#include <boost/bimap/support/value_type_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>

using namespace boost::bimap;
using namespace boost::bimap::support;

typedef bimap<int, unconstrained_set_of<double> > bm;


namespace support_metafunctions_test {

    typedef boost::is_same
    <
        data_type_by< member_at::left , bm >::type,
        key_type_by < member_at::right, bm >::type

    >::type test_metafunction_1;
    BOOST_STATIC_ASSERT(test_metafunction_1::value);

    typedef boost::is_same
    <
        data_type_by< member_at::right, bm >::type,
        key_type_by < member_at::left , bm >::type

    >::type test_metafunction_2;
    BOOST_STATIC_ASSERT(test_metafunction_2::value);

    typedef boost::is_same
    <
        map_type_by  < member_at::left , bm >::type::value_type,
        value_type_by< member_at::left , bm >::type

    >::type test_metafunction_3;
    BOOST_STATIC_ASSERT(test_metafunction_3::value);

} // namespace support_metafunctions_test

void test_bimap_extra()
{
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

