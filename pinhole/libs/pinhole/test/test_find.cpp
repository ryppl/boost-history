// Boost.Pinhole library

// Copyright Jared McIntyre 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include <boost/pinhole.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

class TestPropertyGroup : public property_group
{
public:
    TestPropertyGroup( int level, int id, property_group *pParentGroup) :
        property_group( lexical_cast<string>(level), pParentGroup ),
        m_int(id)
    {
        add_property("ID", "ID", BOOST_SETTER_NONE, BOOST_GETTER_VAR(m_int));
    }

    int m_int;
};

class TestPropertyManagerGuard
{
public:
    TestPropertyManagerGuard()
    {
        property_manager::delete_instance();
    }

    ~TestPropertyManagerGuard()
    {
        property_manager::delete_instance();
    }
};

BOOST_AUTO_TEST_CASE( TestSimplePath_RootSearch )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK( &group1 == select_single_node("/1") );
    BOOST_CHECK( &group2 == select_single_node("/1/2") );
    BOOST_CHECK( &group3 == select_single_node("/1/2/3") );
    BOOST_CHECK( &group4 == select_single_node("/1/2/3/4") );
}

BOOST_AUTO_TEST_CASE( TestSimplePath_RelativeSearch )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK( &group2 == select_single_node(group1, "2") );
    BOOST_CHECK( &group3 == select_single_node(group1, "2/3") );
    BOOST_CHECK( &group4 == select_single_node(group1, "2/3/4") );
}

BOOST_AUTO_TEST_CASE( TestRelativePathWithNoPropertyGroup )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK_THROW( select_single_node("1"), boost::pinhole::invalid_path );
}

BOOST_AUTO_TEST_CASE( TestNonExistantRootPath )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK( NULL == select_single_node("/error") );
    BOOST_CHECK( NULL == select_single_node("/1/error") );
}

BOOST_AUTO_TEST_CASE( TestNonExistantVariablePath )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK( NULL == select_single_node(group1, "error") );
    BOOST_CHECK( NULL == select_single_node(group1, "2/error") );
}

BOOST_AUTO_TEST_CASE( TestFindByValue_RootSearch )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 1, &group1a);
    TestPropertyGroup group2b(2, 2, &group1a);
    TestPropertyGroup group3a(3, 1, &group2a);
    TestPropertyGroup group3b(3, 2, &group2a);

    BOOST_CHECK( &group1a == select_single_node("/1.ID=1") );
    BOOST_CHECK( &group1b == select_single_node("/1.ID=2") );

    BOOST_CHECK( &group2a == select_single_node("/1.ID=1/2.ID=1") );
    BOOST_CHECK( &group2b == select_single_node("/1.ID=1/2.ID=2") );

    BOOST_CHECK( &group3a == select_single_node("/1.ID=1/2.ID=1/3.ID=1") );
    BOOST_CHECK( &group3b == select_single_node("/1.ID=1/2.ID=1/3.ID=2") );
}

BOOST_AUTO_TEST_CASE( TestFindByValue_VariableSearch )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 1, &group1a);
    TestPropertyGroup group2b(2, 2, &group1a);
    TestPropertyGroup group3a(3, 1, &group2a);
    TestPropertyGroup group3b(3, 2, &group2a);

    BOOST_CHECK( &group2a == select_single_node(group1a, "2.ID=1") );
    BOOST_CHECK( &group2b == select_single_node(group1a, "2.ID=2") );

    BOOST_CHECK( &group3a == select_single_node(group1a, "2.ID=1/3.ID=1") );
    BOOST_CHECK( &group3b == select_single_node(group1a, "2.ID=1/3.ID=2") );
}

BOOST_AUTO_TEST_CASE( TestFindMultiples )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 1, &group1a);
    TestPropertyGroup group2b(2, 2, &group1a);
    TestPropertyGroup group3a(3, 1, &group2a);
    TestPropertyGroup group3b(3, 2, &group2a);

    BOOST_CHECK_THROW( select_single_node("/1"), boost::pinhole::multiple_property_groups );
    BOOST_CHECK_THROW( select_single_node("/1.ID=1/2"), boost::pinhole::multiple_property_groups );
}