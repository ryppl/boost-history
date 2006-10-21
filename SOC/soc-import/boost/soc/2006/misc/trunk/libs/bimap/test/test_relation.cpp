// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// std
#include <string>

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.MPL
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

// Boost.Bimap
#include <boost/bimap/detail/test/check_metadata.hpp>
#include <boost/bimap/tags/tagged.hpp>

// Boost.Bimap.Relation
#include <boost/bimap/relation/relation.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/get.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/relation/support/pair_type_by.hpp>
#include <boost/bimap/relation/support/value_type_of.hpp>
#include <boost/bimap/relation/support/member_with_tag.hpp>
#include <boost/bimap/relation/support/is_tag_of_member_at.hpp>

// Bimap Test Utilities
#include "test_relation.hpp"

BOOST_BIMAP_TEST_STATIC_FUNCTION( untagged_static_test )
{
    using namespace boost::bimap::relation::member_at;
    using namespace boost::bimap::relation;
    using namespace boost::bimap::tags;

    struct left_data  {};
    struct right_data {};

    typedef select_relation< left_data, right_data >::type rel;

    BOOST_BIMAP_CHECK_METADATA(rel,left_value_type ,left_data);
    BOOST_BIMAP_CHECK_METADATA(rel,right_value_type,right_data);

    BOOST_BIMAP_CHECK_METADATA(rel,left_tag ,left );
    BOOST_BIMAP_CHECK_METADATA(rel,right_tag,right);

    typedef tagged<left_data ,left > desired_tagged_left_type;
    BOOST_BIMAP_CHECK_METADATA(rel,tagged_left_type,desired_tagged_left_type);

    typedef tagged<right_data,right> desired_tagged_right_type;
    BOOST_BIMAP_CHECK_METADATA(rel,tagged_right_type,desired_tagged_right_type);

}

BOOST_BIMAP_TEST_STATIC_FUNCTION( tagged_static_test)
{
    using namespace boost::bimap::relation::member_at;
    using namespace boost::bimap::relation;
    using namespace boost::bimap::tags;

    struct left_data  {};
    struct right_data {};

    struct left_tag   {};
    struct right_tag  {};

    typedef select_relation< tagged<left_data,left_tag>, tagged<right_data,right_tag> >::type rel;

    BOOST_BIMAP_CHECK_METADATA(rel,left_value_type ,left_data);
    BOOST_BIMAP_CHECK_METADATA(rel,right_value_type,right_data);

    BOOST_BIMAP_CHECK_METADATA(rel,left_tag ,left_tag  );
    BOOST_BIMAP_CHECK_METADATA(rel,right_tag,right_tag );

    typedef tagged<left_data ,left_tag > desired_tagged_left_type;
    BOOST_BIMAP_CHECK_METADATA(rel,tagged_left_type,desired_tagged_left_type);

    typedef tagged<right_data,right_tag> desired_tagged_right_type;
    BOOST_BIMAP_CHECK_METADATA(rel,tagged_right_type,desired_tagged_right_type);
}

struct relation_builder
{
    template< class LeftType, class RightType >
    struct build
    {
        typedef typename boost::bimap::relation::select_relation<LeftType,RightType>::type type;
    };
};

void test_best_relation()
{
    test_relation< relation_builder, char  , double >( 'l', 2.5 );

    test_relation< relation_builder, double, char   >( 2.5, 'r' );

    test_relation< relation_builder, int   , int    >(  1 ,   2 );

    test_relation< relation_builder, std::string, int * >( "left value", 0 );
}

int test_main( int, char* [] )
{

    // Test metadata correctness with untagged relation version
    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( tagged_static_test   );

    // Test metadata correctness with tagged relation version
    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( untagged_static_test );

    // Test basic
    test_best_relation();

    return 0;
}

