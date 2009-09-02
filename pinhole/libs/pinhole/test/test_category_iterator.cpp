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
#include <boost/pinhole/category_iterator.hpp>

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

BOOST_AUTO_TEST_CASE( TestCategoryIterator )
{
    std::list<property_group*> properties;

    TestPropertyGroup group1(1, 1, NULL);
    group1.add_category("a");
    group1.add_category("e");
    TestPropertyGroup group2(2, 2, NULL);
    group2.add_category("b");
    group2.add_category("e");
    TestPropertyGroup group3(3, 3, NULL);
    group3.add_category("c");
    TestPropertyGroup group4(4, 4, NULL);
    group4.add_category("d");
    group4.add_category("e");

    properties.push_back( &group1 );
    properties.push_back( &group2 );
    properties.push_back( &group3 );
    properties.push_back( &group4 );

    {
        category_iterator< std::list<property_group*>::iterator > itr = make_category_iterator( "a", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd = make_category_iterator( "a", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 1 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "b", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "b", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "c", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "c", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "d", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "d", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 1 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 1 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "e", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "e", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 3 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 1 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 3 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "All", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "All", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 4 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            switch( ct )
            {
                case  0:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 1 );
                    break;
                case  1:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 2 );
                    break;
                case  2:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 3 );
                    break;
                case  3:
                    BOOST_CHECK_EQUAL( (*itr)->get<int>("ID"), 4 );
                    break;
                default:
                    BOOST_ERROR("found too many groups");
            }
        }
        BOOST_CHECK_EQUAL( ct, 4 );
    }

    {
        category_iterator< std::list<property_group*>::iterator > itr( "none", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd( "none", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 0 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            BOOST_ERROR("found too many groups");
        }
        BOOST_CHECK_EQUAL( ct, 0 );
    }
}

BOOST_AUTO_TEST_CASE( TestCategoryIterator_EmptyCollection )
{
    std::list<property_group*> properties;

    {
        category_iterator< std::list<property_group*>::iterator > itr = make_category_iterator( "a", properties.begin(), properties.end() );
        category_iterator< std::list<property_group*>::iterator > itrEnd = make_category_iterator( "a", properties.end(), properties.end() );

        BOOST_CHECK_EQUAL( std::distance(itr, itrEnd), 0 );

        int ct=0;
        for( ; itr != itrEnd; ++itr, ++ct )
        {
            BOOST_ERROR("found too many groups");
        }
        BOOST_CHECK_EQUAL( ct, 0 );
    }
}
