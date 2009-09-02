// Boost.Pinhole library

// Copyright Jared McIntyre 2008-2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/pinhole.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

class TestPropertyGroup : public property_group
{
public:
    TestPropertyGroup( int name, int id, property_group *pParentGroup) :
        property_group( lexical_cast<string>(name), pParentGroup ),
        m_int(id)
    {
        add_property("ID", BOOST_SETTER_VAR(m_int), BOOST_GETTER_VAR(m_int));
    }

    int m_int;
};

class TestReadOnly : public property_group
{
public:
    TestReadOnly( int name, int id, property_group *pParentGroup) :
      property_group( lexical_cast<string>(name), pParentGroup ),
          m_int(id)
      {
          add_property("ID", BOOST_SETTER_NONE, BOOST_GETTER_VAR(m_int));
      }

      int m_int;
};

BOOST_AUTO_TEST_CASE( TestSimplePath_RootSearch )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK_EQUAL( get_single_value<int>("/1", "ID"), 1 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1/2", "ID"), 2 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1/2/3", "ID"), 3 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1/2/3/4", "ID"), 4 );

    // First parameter is unimportant when fully qualified path is provided
    BOOST_CHECK_EQUAL( get_single_value<int>(group4, "/1", "ID"), 1 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group3, "/1/2", "ID"), 2 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group2, "/1/2/3", "ID"), 3 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1, "/1/2/3/4", "ID"), 4 );

    set_single_value("/1", "ID", 5);
    BOOST_CHECK_EQUAL( get_single_value<int>("/1", "ID"), 5 );

    set_single_value( group4, "/1/2", "ID", 55);
    BOOST_CHECK_EQUAL( get_single_value<int>(group2, "/1/2", "ID"), 55 );
}

BOOST_AUTO_TEST_CASE( TestSimplePath_RelativePath )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);
    TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK_EQUAL( get_single_value<int>(group1, "2", "ID"), 2 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1, "2/3", "ID"), 3 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1, "2/3/4", "ID"), 4 );
}

BOOST_AUTO_TEST_CASE( TestInvalidPath )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);

    BOOST_CHECK_THROW( get_single_value<int>("1", "ID"), boost::pinhole::invalid_path );
    BOOST_CHECK_THROW( get_single_value<int>("2", "ID"), boost::pinhole::invalid_path );

    BOOST_CHECK_THROW( set_single_value("1", "ID", 5), boost::pinhole::invalid_path );
}

BOOST_AUTO_TEST_CASE( TestFailedToFindGroup )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);

    BOOST_CHECK_THROW( get_single_value<int>("/error", "ID"), boost::pinhole::failed_to_find_group );
    BOOST_CHECK_THROW( get_single_value<int>("/1/error", "ID"), boost::pinhole::failed_to_find_group );
    BOOST_CHECK_THROW( get_single_value<int>("/2", "ID"), boost::pinhole::failed_to_find_group );

    BOOST_CHECK_THROW( get_single_value<int>(group1, "1", "ID"), boost::pinhole::failed_to_find_group );
    BOOST_CHECK_THROW( get_single_value<int>(group1, "/2", "ID"), boost::pinhole::failed_to_find_group );

    BOOST_CHECK_THROW( set_single_value("/2/error", "ID", 5), boost::pinhole::failed_to_find_group );
    BOOST_CHECK_THROW( set_single_value(group1, "1", "ID", 5), boost::pinhole::failed_to_find_group );
    BOOST_CHECK_THROW( set_single_value(group1, "/3", "ID", 5), boost::pinhole::failed_to_find_group );
}

BOOST_AUTO_TEST_CASE( TestFindByValue_RootSearch )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 13, &group1a);
    TestPropertyGroup group2b(2, 14, &group1a);
    TestPropertyGroup group3a(3, 25, &group1b);
    TestPropertyGroup group3b(3, 26, &group1b);

    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=1", "ID"), 1 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=2", "ID"), 2 );

    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=1/2.ID=13", "ID"), 13 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=1/2.ID=14", "ID"), 14 );

    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=2/3.ID=25", "ID"), 25 );
    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=2/3.ID=26", "ID"), 26 );

    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "/1.ID=1/2.ID=13", "ID"), 13 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "/1.ID=2/3.ID=26", "ID"), 26 );

    set_single_value( "/1.ID=2/3.ID=25", "ID", 3);
    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=2/3.ID=3", "ID"), 3 );

    set_single_value( "/1.ID=1/2.ID=14", "ID", 44);
    BOOST_CHECK_EQUAL( get_single_value<int>("/1.ID=1/2.ID=44", "ID"), 44);
}

BOOST_AUTO_TEST_CASE( TestFindByValue_RelativePath )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 1, &group1a);
    TestPropertyGroup group2b(2, 2, &group1a);
    TestPropertyGroup group3a(3, 1, &group1b);
    TestPropertyGroup group3b(3, 2, &group1b);

    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "2.ID=1", "ID"), 1 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "2.ID=2", "ID"), 2 );

    BOOST_CHECK_EQUAL( get_single_value<int>(group1b, "3.ID=1", "ID"), 1 );
    BOOST_CHECK_EQUAL( get_single_value<int>(group1b, "3.ID=2", "ID"), 2 );

    set_single_value( group1a, "2.ID=1", "ID", 5);
    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "2.ID=5", "ID"), 5 );
    set_single_value( group1a, "2.ID=5", "ID", 6);
    BOOST_CHECK_EQUAL( get_single_value<int>(group1a, "2.ID=6", "ID"), 6 );
}

BOOST_AUTO_TEST_CASE( TestFindMultiples )
{
    TestPropertyGroup group1a(1, 1, NULL);
    TestPropertyGroup group1b(1, 2, NULL);
    TestPropertyGroup group2a(2, 1, &group1a);
    TestPropertyGroup group2b(2, 2, &group1a);

    BOOST_CHECK_THROW( get_single_value<int>("/1", "ID"), boost::pinhole::multiple_property_groups );
    BOOST_CHECK_THROW( get_single_value<int>("/1.ID=1/2", "ID"), boost::pinhole::multiple_property_groups );

    BOOST_CHECK_THROW( set_single_value("/1", "ID", 5), boost::pinhole::multiple_property_groups );
    BOOST_CHECK_THROW( set_single_value("/1.ID=1/2", "ID", 5), boost::pinhole::multiple_property_groups );
}

BOOST_AUTO_TEST_CASE( TestBadProperty )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);

    BOOST_CHECK_THROW( get_single_value<int>("/1", "xID"), std::out_of_range );
    BOOST_CHECK_THROW( get_single_value<int>("/1/2", "xID"), std::out_of_range );

    BOOST_CHECK_THROW( set_single_value("/1", "xID", 5), std::out_of_range );
    BOOST_CHECK_THROW( set_single_value("/1/2", "xID", 5), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestBadCast )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestPropertyGroup group2(2, 2, &group1);
    TestPropertyGroup group3(3, 3, &group2);

    BOOST_CHECK_THROW( get_single_value<bool>("/1", "ID"), std::bad_cast );
    BOOST_CHECK_THROW( get_single_value<double>("/1", "ID"), std::bad_cast );
    BOOST_CHECK_THROW( get_single_value<string>("/1", "ID"), std::bad_cast );

    BOOST_CHECK_THROW( set_single_value("/1", "ID", false), std::bad_cast );
    BOOST_CHECK_THROW( set_single_value("/1", "ID", 5.0), std::bad_cast );
    string s = "5";
    BOOST_CHECK_THROW( set_single_value("/1", "ID", s), std::bad_cast );
}

BOOST_AUTO_TEST_CASE( TestSetReadOnly )
{
    TestPropertyGroup group1(1, 1, NULL);
    TestReadOnly group2(2, 2, &group1);
    TestReadOnly group3(3, 3, &group2);

    set_single_value("/1", "ID", 5);
    BOOST_CHECK_THROW( set_single_value("/1/2", "ID", 5), boost::bad_function_call );
    BOOST_CHECK_THROW( set_single_value("/1/2/3", "ID", 5), boost::bad_function_call );
}
