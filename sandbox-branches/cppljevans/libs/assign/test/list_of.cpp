/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#include <boost/assign/list_of.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <algorithm>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>

struct nothing
{
    template< class T >
    void operator()( T )
    { }
    
};

template< class Range >
void for_each( const Range& r )
{
    std::for_each( r.begin(), r.end(), nothing() );
}

namespace ba = boost::assignment;
    
template< class C >
void test_sequence_list_of_string()
{
    const C c = ba::list_of( "foo" )( "bar" );   
    BOOST_CHECK_EQUAL( c.size(), 2 );
}

template< class C >
void test_sequence_list_of_int()
{
    const C c  = ba::list_of(1)(2)(3)(4);
    const C c2 = (ba::list_of(1),2,3,4);
    BOOST_CHECK_EQUAL( c.size(), 4 );
    BOOST_CHECK_EQUAL( c2.size(), 4 );
}

template< class C >
void test_map_list_of()
{
    const C c  = ba::list_of< std::pair<std::string,int> >( "foo", 1 )( "bar", 2 )( "buh", 3 )( "bah", 4 );
    BOOST_CHECK_EQUAL( c.size(), 4 );
    const C c2  = ba::map_list_of( "foo", 1 )( "bar", 2 )( "buh", 3 )( "bah", 4 );
    BOOST_CHECK_EQUAL( c2.size(), 4 );
}

template< class T >
void test_list_of()
{
/* Maybe this could be usefull in a later version?

    // an anonymous lists, fulfills Range concept
    for_each( ba::list_of( T() )( T() )( T() ) );
    
    // non-anonymous lists
    ba::generic_list<T> list_1 = ba::list_of( T() );
    BOOST_CHECK_EQUAL( list_1.size(), 1 );
    ba::generic_list<T> list_2 = list_1 + ba::list_of( T() )( T() ) + list_1;
    BOOST_CHECK_EQUAL( list_2.size(), 4 );
    list_1 += list_2;
    BOOST_CHECK_EQUAL( list_1.size(), 5 );
*/
}



void check_list_of()
{
    test_sequence_list_of_int< std::vector<int> >();
    test_sequence_list_of_int< std::list<int> >();
    test_sequence_list_of_int< std::deque<int> >();
    test_sequence_list_of_int< std::set<int> >();
    test_sequence_list_of_int< std::multiset<int> >();
    test_sequence_list_of_int< std::vector<float> >();
    test_sequence_list_of_string< std::vector<std::string> >();
    test_map_list_of< std::map<std::string,int> >();
    test_map_list_of< std::multimap<std::string,int> >();
    
    test_list_of<int>();
    test_list_of<std::string>();
}



#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_list_of ) );

    return test;
}

/*
namespace std
{

    struct _Lockit
    {
        _Lockit() {}
        ~_Lockit() {}
    };

}
*/
