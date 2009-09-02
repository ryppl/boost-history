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
    TestPropertyGroup( int level, int id, property_group *pParentGroup) :
        property_group( lexical_cast<string>(level), pParentGroup ),
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

BOOST_AUTO_TEST_CASE( TestFilterNodes_SimplePaths )
{
    TestPropertyGroup group1(1, 1, NULL);
        TestPropertyGroup group2(2, 2, &group1);
            TestPropertyGroup group3(3, 3, &group2);
                TestPropertyGroup group4(4, 4, &group3);

    BOOST_CHECK_THROW( get_path_filtered_range("1"), invalid_path );

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("/5");

        BOOST_CHECK( range.begin() == range.end() );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("/1/2");

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 2 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        // test const

        boost::pinhole::const_path_filtered_range range = get_const_path_filtered_range("/1/2");

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch(ct)
            {
            case 0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 2 );
                break;
            default:
                BOOST_ERROR("Too many items");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("/1/2/3/");

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 3 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("/1/2/3/4");

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 4 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }
}

BOOST_AUTO_TEST_CASE( TestPathFilteredIterator_ComplexPaths_NoValueCheck )
{
    TestPropertyGroup group1(1, 1, NULL);
        TestPropertyGroup group2(2, 2, &group1);
            TestPropertyGroup group3(3, 3, &group2);
        TestPropertyGroup group4(2, 4, &group1);
        TestPropertyGroup group5(2, 5, &group1);
            TestPropertyGroup group6(3, 6, &group5);
            TestPropertyGroup group7(3, 7, &group5);
                TestPropertyGroup group8(4, 8, &group7);
            TestPropertyGroup group9(3, 9, &group5);

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("2/3", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 4 );

        int ct = 0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 3 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 6 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 7 );
                    break;
                case  3:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 9 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }

    {
        // test const

        boost::pinhole::const_path_filtered_range range = get_const_path_filtered_range("2/3", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 4 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 3 );
                break;
            case  1:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 6 );
                break;
            case  2:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 7 );
                break;
            case  3:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 9 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("/1/2/3", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 4 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 3 );
                break;
            case  1:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 6 );
                break;
            case  2:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 7 );
                break;
            case  3:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 9 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }

    {
        // test const

        boost::pinhole::const_path_filtered_range range = get_const_path_filtered_range("/1/2/3", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 4 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 3 );
                break;
            case  1:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 6 );
                break;
            case  2:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 7 );
                break;
            case  3:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 9 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("2", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 3 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 2 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 4 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 5 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 3 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("2/3/4/", group1);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( prop->get<int>("ID"), 8 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("2", group5);

        BOOST_CHECK( range.begin() == range.end() );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("3", group5);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 3 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 6 );
                break;
            case  1:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 7 );
                break;
            case  2:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 9 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 3 );
    }

    {
        boost::pinhole::path_filtered_range range = get_path_filtered_range("3/4", group5);

        BOOST_CHECK( range.begin() != range.end() );

        BOOST_CHECK_EQUAL( std::distance(begin(range), end(range)), 1 );

        int ct=0;
        BOOST_FOREACH( property_group* prop, range )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( prop->get<int>("ID"), 8 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }

            ++ct;
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }
}