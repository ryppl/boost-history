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

#include <boost/assign/list_inserter.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>

namespace ba = boost::assignment;

template< class T >
void function_ptr( T )
{
    // do nothing
}

struct functor
{
    template< class T >
    void operator()( T ) const
    { 
        // do nothing
    }
};

class email
{
public:
    enum address_option
    {
        check_addr_book,
        dont_check_addr_book
    };
    
    typedef std::pair<std::string,address_option> bcc_type;
        
private:

    mutable std::map<std::string,address_option> cc_list;
    mutable std::map<std::string,address_option> to_list;
    std::vector< bcc_type >                      bcc_list;
        
    void add_cc_impl( const std::string& name, address_option ao )
    {
        cc_list[ name ] = ao;
    }
    
    void add_to_impl( const std::string& name )
    {
        to_list[ name ] = check_addr_book;
    }
    
    void add_bcc_impl( const bcc_type& bcc )
    {
        bcc_list.push_back( bcc ); 
    }
     
public:
    
    ba::list_inserter< boost::function2< void, std::string, address_option > >
    add_cc( std::string name, address_option ao )
    {
        add_cc_impl( name, ao );
        return ba::make_list_inserter( boost::bind( &email::add_cc_impl, this, _1, _2 ) );
    }

    ba::list_inserter< boost::function1< void, std::string > >
    add_to( const std::string& name )
    {
        add_to_impl( name );
        return ba::make_list_inserter( boost::bind( &email::add_to_impl, this, _1 ) );
    }
    
    ba::list_inserter< boost::function1< void, bcc_type >, bcc_type >
    add_bcc( const bcc_type& bcc )
    {
        add_bcc_impl( bcc );
        return ba::make_list_inserter( boost::bind( &email::add_bcc_impl, this, _1 ) );     
    }
    
    address_option
    cc_at( const std::string& name ) const
    {
        return cc_list[ name ];
    }
    
    address_option 
    to_at( const std::string& name ) const
    {
        return to_list[ name ];
    }
    
    address_option
    bcc_at( unsigned index ) const
    {
        return bcc_list.at( index ).second;
    }
};



void check_list_inserter()
{
    using namespace std;
    using namespace boost;
    using namespace boost::assignment;
    vector<int> v;
    
    make_list_inserter( &function_ptr<int> )( 5 ),3;
    make_list_inserter( functor() )( 4 ),2;
    make_list_inserter( bind( &vector<int>::push_back, &v, _1 ) )( 6 ),4;
    BOOST_CHECK_EQUAL( v.size(), 2 );
    BOOST_CHECK_EQUAL( v[0], 6 );
    BOOST_CHECK_EQUAL( v[1], 4 );
    
    typedef map<string,int> map_t;
    typedef map_t::value_type V;
    map_t m;
    make_list_inserter( bind( &map_t::insert, &m, _1 ) )
                      ( V("bar",3), V("foo", 2) );
    //make_list_inserter_of<V>( bind( &map_t::insert, &m, _1 ) )
    //                  ( "bar", 3 )( "foo", 2 );
    BOOST_CHECK_EQUAL( m.size(), 2 );
    BOOST_CHECK_EQUAL( m["foo"], 2 );
    
    email e;
    e.add_cc( "franz", email::dont_check_addr_book )
            ( "hanz", email::check_addr_book )
            ( "betty", email::dont_check_addr_book );
    BOOST_CHECK_EQUAL( e.cc_at( "franz" ), email::dont_check_addr_book );
    BOOST_CHECK_EQUAL( e.cc_at( "hanz" ), email::check_addr_book );
    BOOST_CHECK_EQUAL( e.cc_at( "betty" ), email::dont_check_addr_book );

    e.add_to( "betsy" )( "peter" );
    BOOST_CHECK_EQUAL( e.cc_at( "betsy" ), email::check_addr_book );
    BOOST_CHECK_EQUAL( e.cc_at( "peter" ), email::check_addr_book );
    
    e.add_bcc( email::bcc_type( "Mr. Foo", email::check_addr_book ) )
             ( "Mr. Bar", email::dont_check_addr_book );
    BOOST_CHECK_EQUAL( e.bcc_at( 0 ), email::check_addr_book );
    BOOST_CHECK_EQUAL( e.bcc_at( 1 ), email::dont_check_addr_book );
}



#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_list_inserter ) );

    return test;
}

