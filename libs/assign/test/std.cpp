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

#include <boost/assign/std.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <utility>
#include <string>

using namespace std;

template< typename K, typename V >
inline pair<K,V> P( K k, V v )
{
    return make_pair( k, v );
}

struct three
{
    three( int, int, int ) { }
    three( const string&, const string&, const string& ) { }
};

struct four
{
    four( int, int, int, int ) { }
    four( const string&, const string&, const string&, const string& ) { }
};

struct five
{
    five( int, int, int, int, int ) { }
    five( const string&, const string&, const string&,  
	  const string&, const string& ) { }
};

struct six
{
    six( int, int, int, int, int, int ) { }
    six( const string&, const string&, const string&,  
	  const string&, const string&, const string& ) { }
};


struct takes_ref
{
    int& i_;
    takes_ref( int& i) : i_(i) {}
};


template< class C >
typename C::value_type back( C& c )
{
    return c.back();
}

template< class T >
T back( set<T>& c )
{
    return *(--c.end());
}

template< class T >
T back( multiset<T>& c )
{
    return *(--c.end());
}

template< class T >
T back( stack<T>& c )
{
    return c.top();
}

template< class T >
T back( priority_queue<T>& c )
{
    return c.top();
}

template< class T >
T back( queue<T>& c )
{
    return c.front();
}

template< class T >
boost::assignment::insert_assigner< std::set<T> >
append( std::set<T>& s )
{
    return boost::assignment::insert( s );
}

template< class T >
boost::assignment::insert_assigner< std::multiset<T> >
append( std::multiset<T>& s )
{
    return boost::assignment::insert( s );
}


template< class C >
void test_int_sequence()
{
    using namespace boost::assignment;  
    C c;
    
    BOOST_CHECK_EQUAL( c.size(), 0 );
    c +=1,2,3,4,5,6,7,8,9,10;
    BOOST_CHECK_EQUAL( c.size(), 10 );
    BOOST_CHECK_EQUAL( back( c ), 10 );
    append( c )(11)(12)(13)(14)(15);
    BOOST_CHECK_EQUAL( c.size(), 15 );
    BOOST_CHECK_EQUAL( back( c ), 15 );
}



template< class C >
void test_string_sequence()
{
    using namespace boost::assignment;  
    C c;

    BOOST_CHECK_EQUAL( c.size(), 0 );
    c += "1","2","3","4","5","6","7","8","9","10";
    BOOST_CHECK_EQUAL( c.size(), 10 );
    BOOST_CHECK_EQUAL( back( c ), "10" );
    append( c )("11")("12")("13")("14")("15");
    BOOST_CHECK_EQUAL( c.size(), 15 );
    BOOST_CHECK_EQUAL( back( c ), "15" );
}



typedef pair<string,int> tuple; 

template< class C >
void test_tuple_sequence()
{
    using namespace boost::assignment;  
    C c;
    
    BOOST_CHECK_EQUAL( c.size(), 0 );
    c += P("1",1), P("2",2), P("3",3), P("4",4), P("5",5), P("6",6), 
         P("7",7), P("8",8), P("9",9), P("10",10);
    BOOST_CHECK_EQUAL( c.size(), 10 );
    C::value_type v = back( c ), p = P("10",10);
    BOOST_CHECK_EQUAL( v.first , p.first );
    BOOST_CHECK_EQUAL( v.second , p.second );
    append( c )( "11", 11 )( "12", 12 )( "13", 13 )( "14", 14 )( "15", 15 );
    BOOST_CHECK_EQUAL( c.size(), 15 );
    v = back( c ), p = P("15",15);
    BOOST_CHECK_EQUAL( v.first , p.first );
    BOOST_CHECK_EQUAL( v.second , p.second );
}



template< class M >
void test_map()
{
    using namespace boost::assignment;  
    M m;
    m += P( "january",   31 ), P( "february", 28 ),
         P( "march",     31 ), P( "april",    30 ),
         P( "may",       31 ), P( "june",     30 ),
         P( "july",      31 ), P( "august",   31 ),
         P( "september", 30 ), P( "october",  31 ),
         P( "november",  30 ), P( "december", 31 );
    BOOST_CHECK_EQUAL( m.size(), 12 );
    m.clear();
    insert( m )
        ( "january",   31 )( "february", 28 )
        ( "march",     31 )( "april",    30 )
        ( "may",       31 )( "june",     30 )
        ( "july",      31 )( "august",   31 )
        ( "september", 30 )( "october",  31 )
        ( "november",  30 )( "december", 31 );
    BOOST_CHECK_EQUAL( m.size(), 12 );
}



void test_tuple()
{
    using namespace boost::assignment;      
    vector<three>    v_three;
    vector<four>     v_four;
    vector<five>     v_five;
    vector<six>      v_six;

    append( v_three ) (1,2,3) ("1","2","3");
    append( v_four ) (1,2,3,4) ("1","2","3","4");
    append( v_five ) (1,2,3,4,5) ("1","2","3","4","5");
    append( v_six ) (1,2,3,4,5,6) ("1","2","3","4","5","6");
    BOOST_CHECK_EQUAL( v_three.size(), 2 );
    BOOST_CHECK_EQUAL( v_four.size(), 2 );
    BOOST_CHECK_EQUAL( v_five.size(), 2 );
    BOOST_CHECK_EQUAL( v_six.size(), 2 ); 
}



void check_std()
{
    using namespace boost::assignment;      
    test_int_sequence< deque<int> >();
    test_int_sequence< list<int> >();          
    test_int_sequence< vector<int> >();       
    //test_int_sequence< set<int> >();          
    //test_int_sequence< multiset<int> >();     
    test_int_sequence< stack<int> >();        
    //test_int_sequence< queue<int> >();        
    //test_int_sequence< priority_queue<int> >();
            
    test_string_sequence< deque<string> >();             
    test_string_sequence< list<string> >();              
    test_string_sequence< vector<string> >();            
    //test_string_sequence< set<string> >();               
    //test_string_sequence< multiset<string> >();          
    test_string_sequence< stack<string> >();             
    //test_string_sequence< queue<string> >();             
    //test_string_sequence< priority_queue<string> >();    

    test_tuple_sequence< deque<tuple> >();             
    test_tuple_sequence< list<tuple> >();              
    test_tuple_sequence< vector<tuple> >();            
    //test_tuple_sequence< set<tuple> >();               
    //test_tuple_sequence< multiset<tuple> >();          
    test_tuple_sequence< stack<tuple> >();             
    //test_tuple_sequence< queue<tuple> >();             
    //test_tuple_sequence< priority_queue<tuple> >();    
    test_tuple();
    
    deque<int>          di; append( di )( 1 );
    BOOST_CHECK_EQUAL( di[0], 1 );
    
    list<int>           li; append( li )( 2 );
    BOOST_CHECK_EQUAL( li.front(), 2 );
    
    vector<int>         vi; append( vi )( 3 );
    BOOST_CHECK_EQUAL( vi[0], 3 );
    
    set<int>            si; insert( si )( 4 );
    BOOST_CHECK_EQUAL( *si.find( 4 ), 4 );
    
    multiset<int>       msi; insert( msi )( 5 );
    BOOST_CHECK_EQUAL( *msi.find( 5 ), 5 );
    
    stack<int>          sti; append( sti )( 6 );
    BOOST_CHECK_EQUAL( sti.top(), 6 );
    
    queue<int>          qi; append( qi )( 7 );
    BOOST_CHECK_EQUAL( qi.back(), 7 );
    
    priority_queue<int> pqi; append( pqi )( 8 );
    BOOST_CHECK_EQUAL( pqi.top(), 8 );
    
    test_map< map<string,int> >();
    test_map< multimap<string,int> >();

}



#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "List Test Suite" );

    test->add( BOOST_TEST_CASE( &check_std ) );

    return test;
}

