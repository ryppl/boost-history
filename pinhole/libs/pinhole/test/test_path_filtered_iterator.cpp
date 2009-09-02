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
#include <boost/pinhole/path_filtered_iterator.hpp>

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

BOOST_AUTO_TEST_CASE( TestPathFilteredIterator_SimplePaths )
{
    TestPropertyGroup group1(1, 1, NULL);
        TestPropertyGroup group2(2, 2, &group1);
            TestPropertyGroup group3(3, 3, &group2);
                TestPropertyGroup group4(4, 4, &group3);

    {
        path_filtered_iterator itr( "5", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }

    {
        path_filtered_iterator itr( "2", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        path_filtered_iterator itr( "2/3/", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        path_filtered_iterator itr( "2/3/4", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch(ct)
            {
                case 0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                default:
                    BOOST_ERROR("Too many items");
            }
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
        path_filtered_iterator itr( "2/3", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 4 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
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

    {
        path_filtered_iterator itr( "2", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 3 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 5 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 3 );
    }

    {
        path_filtered_iterator itr( "2/3/4/", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 8 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        path_filtered_iterator itr( "2", group5.get_children_collection().begin(), group5.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group5.get_children_collection().end(), group5.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }

    {
        path_filtered_iterator itr( "3", group5.get_children_collection().begin(), group5.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group5.get_children_collection().end(), group5.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 3 );

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
            case  2:
                BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 9 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 3 );
    }

    {
        path_filtered_iterator itr( "3/4", group5.get_children_collection().begin(), group5.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group5.get_children_collection().end(), group5.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
            case  0:
                BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 8 );
                break;
            default:
                BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }
}

BOOST_AUTO_TEST_CASE( TestPathFilteredIterator_ComplexPaths_WithValueCheck )
{
    TestPropertyGroup group1(1, 1, NULL);
        TestPropertyGroup group2(2, 2, &group1);
            TestPropertyGroup group3(3, 3, &group2);
        TestPropertyGroup group4(2, 4, &group1);
        TestPropertyGroup group5(2, 5, &group1);
            TestPropertyGroup group6(3, 6, &group5);
            TestPropertyGroup group7(3, 7, &group5);
                TestPropertyGroup group8(4, 8, &group7);
                TestPropertyGroup groupB(4, 9, &group7);
            TestPropertyGroup group9(3, 6, &group5);
            TestPropertyGroup groupA(3, 7, &group5);

    {
        path_filtered_iterator itr( "2/3.ID=6", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 2 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 6 );
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL((*itr), &group6);
                    break;
                case  1:
                    BOOST_CHECK_EQUAL((*itr), &group9);
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 2 );
    }

    {
        path_filtered_iterator itr( "2/3.ID=7", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 2 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL((*itr), &group7);
                    break;
                case  1:
                    BOOST_CHECK_EQUAL((*itr), &groupA);
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 2 );
    }

    {
        path_filtered_iterator itr( "2/3.ID=7/4.ID=8", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL((*itr), &group8);
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        path_filtered_iterator itr( "2/3.ID=7/4", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr != itrEnd );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 2 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL((*itr), &group8);
                    break;
                case  1:
                    BOOST_CHECK_EQUAL((*itr), &groupB);
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 2 );
    }

    {
        path_filtered_iterator itr( "2/3.ID=99/4.ID=8", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }

    {
        path_filtered_iterator itr( "2/3.ID=7/4.ID=99", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( group1.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }

    {
        path_filtered_iterator itr( "2.ID=99", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( "", group1.get_children_collection().end(), group1.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }

    {
        path_filtered_iterator itr( "2.missing=99", group1.get_children_collection().begin(), group1.get_children_collection().end() );
        path_filtered_iterator itrEnd( group1.get_children_collection().end() );

        BOOST_CHECK( itr == itrEnd );
    }
}