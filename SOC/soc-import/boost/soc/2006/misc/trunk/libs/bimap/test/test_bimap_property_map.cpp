// Boost.Bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/test for the library home page.

// std
#include <set>
#include <map>
#include <cstddef>
#include <cassert>
#include <algorithm>

// Boost.Test
#include <boost/test/minimal.hpp>
#include <boost/assign/list_of.hpp>

// Boost.Bimap

#include <boost/bimap/set_of.hpp>
#include <boost/bimap/property_map/set_support.hpp>

#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/property_map/unordered_set_support.hpp>

#include <boost/bimap/bimap.hpp>


template <class Map>
void test_readable_property_map(
    Map m,
    typename boost::property_traits<Map>::  key_type const & key,
    typename boost::property_traits<Map>::value_type const & value
)
{
    // TODO Add STATIC_ASSERT( boost::property_traits<Map>::category is readable )

    BOOST_CHECK( get(m,key) == value );
    //BOOST_CHECK( m[key]     == value );
}


void test_bimap_property_map()
{
    using namespace boost::bimap;

    typedef bimap< set_of<int>, unordered_set_of<double> > bm;

    bm b = boost::assign::list_of< bm::relation >(1,0.1)(2,0.2)(3,0.3);

    test_readable_property_map(b.left ,  1,0.1);
    test_readable_property_map(b.right,0.1,  1);
}

int test_main( int, char* [] )
{
    test_bimap_property_map();
    return 0;
}

