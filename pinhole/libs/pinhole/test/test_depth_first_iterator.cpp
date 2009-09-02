// Boost.Pinhole library

// Copyright Jared McIntyre 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/pinhole.hpp>
#include <boost/pinhole/depth_first_iterator.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;
using namespace boost::pinhole::detail;

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

    depth_first_iterator itr( group1.get_children_collection().begin(), group1.get_children_collection().end() );
    depth_first_iterator itrEnd( group1.get_children_collection().end(), group1.get_children_collection().end() );

    BOOST_CHECK( itr != itrEnd );

    BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 3 );

    int ct=0;
    for( ; itr != itrEnd; ++itr, ++ct )
    {
        BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4-ct );
    }
    BOOST_CHECK_EQUAL( ct, 3 );
}

BOOST_AUTO_TEST_CASE( TestComplexPath_RootSearch )
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
        depth_first_iterator itr( group1.get_children_collection().begin(), group1.get_children_collection().end() );
        depth_first_iterator itrEnd( group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 8 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                case  3:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
                    break;
                case  4:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 8 );
                    break;
                case  5:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 7 );
                    break;
                case  6:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 9 );
                    break;
                case  7:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 5 );
                    break;
                case  8:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 1 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 8 );
    }

    {
        depth_first_iterator itr( group5.get_children_collection().begin(), group5.get_children_collection().end() );
        depth_first_iterator itrEnd( group5.get_children_collection().end(), group5.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 4 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 8 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 7 );
                    break;
                case  3:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 9 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }
}
/*
BOOST_AUTO_TEST_CASE( TestComplexPath_SubSearch )
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
        depth_first_iterator itr( group5.get_children_collection().begin(), group8.get_children_collection().begin() );
        depth_first_iterator itrEnd( group8.get_children_collection().begin() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 2 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 7 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 2 );
    }
}
*/
BOOST_AUTO_TEST_CASE( TestSimplePath_ManagerSearch )
{
    TestPropertyGroup group1(1, 1, NULL);
        TestPropertyGroup group2(2, 2, &group1);
            TestPropertyGroup group3(3, 3, &group2);
                TestPropertyGroup group4(4, 4, &group3);

    depth_first_iterator itr( property_manager::instance()->begin(), property_manager::instance()->end() );
    depth_first_iterator itrEnd( property_manager::instance()->end(), property_manager::instance()->end() );

    BOOST_CHECK( itr != itrEnd );

    BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 4 );

    int ct=0;
    for( ; itr != itrEnd; ++itr, ++ct )
    {
        BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4-ct );
    }
    BOOST_CHECK_EQUAL( ct, 4 );
}

BOOST_AUTO_TEST_CASE( TestComplexPath_ManagerSearch )
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
        depth_first_iterator itr( property_manager::instance()->begin(), property_manager::instance()->end() );
        depth_first_iterator itrEnd( property_manager::instance()->end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 9 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                case  3:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
                    break;
                case  4:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 8 );
                    break;
                case  5:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 7 );
                    break;
                case  6:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 9 );
                    break;
                case  7:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 5 );
                    break;
                case  8:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 1 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 9 );
    }
}