// Boost.bimap
//
// (C) Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// See http://www.boost.org/libs/{{ library name }}  for the library home page.

// Boost.Test
#include <boost/test/minimal.hpp>

// Boost.MPL
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

// Boost.Bimap
#include <boost/bimap/detail/test/check_metadata.hpp>
#include <boost/bimap/tagged/tagged.hpp>

// Boost.Bimap.Relation
#include <boost/bimap/relation/relation.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/get.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/relation/support/pair_type_by.hpp>
#include <boost/bimap/relation/support/value_type_of.hpp>
#include <boost/bimap/relation/support/member_with_tag.hpp>
#include <boost/bimap/relation/support/is_tag_of_member_at.hpp>


BOOST_BIMAP_TEST_STATIC_FUNCTION( untagged_static_test )
{
    using namespace boost::bimap::relation::member_at;
    using namespace boost::bimap::relation;
    using namespace boost::bimap::tagged;

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
    using namespace boost::bimap::tagged;

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


void test_basic_access()
{
    using namespace boost::bimap::relation;

    select_relation< int, double >::type rel( 100, 2.5 );

    BOOST_CHECK( rel.left  == 100 );
    BOOST_CHECK( rel.right == 2.5 );
}


void test_views()
{
    using namespace boost::bimap::relation::support;
    using namespace boost::bimap::relation;

    typedef select_relation< int, double >::type rel_type;

    rel_type rel( 100, 2.5 );

    BOOST_CHECK( pair_by<member_at::left >(rel).first == 100 );
    BOOST_CHECK( pair_by<member_at::left >(rel).second == 2.5 );

    BOOST_CHECK( pair_by<member_at::right >(rel).first == 2.5 );
    BOOST_CHECK( pair_by<member_at::right >(rel).second == 100 );

    BOOST_CHECK( &pair_by<member_at::left >(rel).first  == &rel.left  );
    BOOST_CHECK( &pair_by<member_at::left >(rel).second == &rel.right );

    BOOST_CHECK( &pair_by<member_at::right>(rel).first  == &rel.right );
    BOOST_CHECK( &pair_by<member_at::right>(rel).second == &rel.left  );

    BOOST_CHECK( get<member_at::left >(rel) == rel.left  );
    BOOST_CHECK( get<member_at::right>(rel) == rel.right );

    BOOST_CHECK( get<member_at::left >(pair_by<member_at::left>(rel)) == rel.left  );
    BOOST_CHECK( get<member_at::right>(pair_by<member_at::left>(rel)) == rel.right );

    BOOST_CHECK( get<member_at::left >(pair_by<member_at::right>(rel)) == rel.left  );
    BOOST_CHECK( get<member_at::right>(pair_by<member_at::right>(rel)) == rel.right );

}

struct my_left  {};
struct my_right {};

void test_tagged()
{
    using namespace boost::bimap::relation::support;
    using namespace boost::bimap::relation;
    using namespace boost::bimap::tagged;

    typedef select_relation< tagged<int, my_left>, tagged<double, my_right> >::type rel_type;

    rel_type rel( 100, 2.5 );

    // It must work with normal tags

    BOOST_CHECK( pair_by<member_at::left >(rel).first == 100 );
    BOOST_CHECK( pair_by<member_at::left >(rel).second == 2.5 );

    BOOST_CHECK( pair_by<member_at::right >(rel).first == 2.5 );
    BOOST_CHECK( pair_by<member_at::right >(rel).second == 100 );

    BOOST_CHECK( &pair_by<member_at::left >(rel).first  == &rel.left  );
    BOOST_CHECK( &pair_by<member_at::left >(rel).second == &rel.right );

    BOOST_CHECK( &pair_by<member_at::right>(rel).first  == &rel.right );
    BOOST_CHECK( &pair_by<member_at::right>(rel).second == &rel.left  );

    BOOST_CHECK( get<member_at::left >(rel) == rel.left  );
    BOOST_CHECK( get<member_at::right>(rel) == rel.right );

    BOOST_CHECK( get<member_at::left >(pair_by<member_at::left>(rel)) == rel.left  );
    BOOST_CHECK( get<member_at::right>(pair_by<member_at::left>(rel)) == rel.right );

    BOOST_CHECK( get<member_at::left >(pair_by<member_at::right>(rel)) == rel.left  );
    BOOST_CHECK( get<member_at::right>(pair_by<member_at::right>(rel)) == rel.right );

    // And with users ones

    BOOST_CHECK( pair_by<my_left >(rel).first == 100 );
    BOOST_CHECK( pair_by<my_left >(rel).second == 2.5 );

    BOOST_CHECK( pair_by<my_right >(rel).first == 2.5 );
    BOOST_CHECK( pair_by<my_right >(rel).second == 100 );

    BOOST_CHECK( &pair_by<my_left >(rel).first  == &rel.left  );
    BOOST_CHECK( &pair_by<my_left >(rel).second == &rel.right );

    BOOST_CHECK( &pair_by<my_right>(rel).first  == &rel.right );
    BOOST_CHECK( &pair_by<my_right>(rel).second == &rel.left  );

    BOOST_CHECK( get<my_left >(rel) == rel.left  );
    BOOST_CHECK( get<my_right>(rel) == rel.right );

    BOOST_CHECK( get<my_left >(pair_by<my_left>(rel)) == rel.left  );
    BOOST_CHECK( get<my_right>(pair_by<my_left>(rel)) == rel.right );

    BOOST_CHECK( get<my_left >(pair_by<my_right>(rel)) == rel.left  );
    BOOST_CHECK( get<my_right>(pair_by<my_right>(rel)) == rel.right );

}


int test_main( int, char* [] )
{

    // Test metadata correctness with untagged relation version
    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( tagged_static_test   );

    // Test metadata correctness with tagged relation version
    BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION( untagged_static_test );

    // Test basic accessors
    test_basic_access();

    // Test relation mutation property
    test_views();

    return 0;
}

