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

#include <boost/assign/stl.hpp>

#include <utility>
#include <iostream>

using namespace std;
using namespace boost::assignment;

template< typename K, typename V >
inline std::pair<K,V> P( K k, V v )
{
    return std::make_pair( k, v );
}

#include <algorithm>
#include <cassert>

template< typename C1, typename C2 >
bool inline assrt_equal( const C1& c1, const C2& c2 )
{
    return equal( c1.begin(), c1.end(), c2.begin() );
}

#define assert_equal( C1, C2 ) assert( assrt_equal( C1, C2 ) )
 
   
#include <deque>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <stack>
#include <queue>

    
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

int main()
{
        
        typedef int     primitive;
        typedef string  compound;
    
        try
        {
    	
    	deque<primitive>                d;
    	list<primitive>                 l;
    	vector<primitive>               v;
    	set<primitive>                  s;
    	multiset<primitive>             ms;
    	stack<primitive>                st;
    	queue<primitive>                q;
    	priority_queue<primitive>       pq;
    	map<primitive, primitive>       m;
    	multimap<primitive, primitive>  mm;
    	
    	
    	deque<compound>               d2;
    	list<compound>                l2;
    	vector<compound>              v2;
    	set<compound>                 s2;
    	multiset<compound>            ms2;
    	stack<compound>               st2;
    	queue<compound>               q2;
    	priority_queue<compound>      pq2;
    	map<compound, compound>       m2;
    	multimap<compound, compound>  mm2;
    	//------------------------------------
    	//------------------------------------
    	d += 1,2,3,4,5,6,7,8,9;
    	l += 1,2,3,4,5,6,7,8,9;
    	v += 1,2,3,4,5,6,7,8,9;
    	s += 1,2,3,4,5,6,7,8,9;
    	ms += 1,2,3,4,5,6,7,8,9;
    	st += 1,2,3,4,5,6,7,8,9;
    	q  += 1,2,3,4,5,6,7,8,9;
    	pq += 1,2,3,4,5,6,7,8,9;
    
    
    	assert_equal( d, l );
    	assert_equal( d, v );
    	assert_equal( d, s );
    	assert_equal( d, ms );
    	
    	m += P( 1, 2 ), P( 2, 3 ), P( 4, 5 );
    	assign( m )( 5,6 )( 6, 7 );    
    	
    	mm += P( 1, 2 ), P( 2, 3 ), P( 4, 5 );
    	assign( mm ) ( 5,6 )( 6, 7 );
    
    	assert_equal( m, mm );    
    	//------------------------------------
    	d2 += "strings", " to", "use as a", compound( "compound" ); 
    	l2 += "strings", " to", "use as a", compound( "compound" );
    	v2 += "strings", " to", "use as a", compound( "compound" );
    	s2 += "strings", " to", "use as a", compound( "compound" );
    	ms2 += "strings", " to", "use as a", compound( "compound" );  
    	st2 += "strings", " to", "use as a", compound( "compound" );  
    	q2 += "strings", " to", "use as a", compound( "compound" );  
    	pq2 += "strings", " to", "use as a", compound( "compound" );  
    
    	assert_equal( d2, l2 );
    	assert_equal( d2, v2 );
    	
    	//
    	// these won't hold since strings are sorted in the sets
    	//
    	//assert_equal( d2, s2 );
    	//assert_equal( d2, ms2 );
    
    	m2 += P( "one", "two" ), P( compound( "three"), "four" );
    	assign( m2 ) ( "five", "six" );
    	mm2 += P( "one", "two" ), P( compound( "three"), "four" );
    	assign( mm2 ) ( "five", "six" );
    
    	assert_equal( m2, mm2 );
    	//------------------------------------
    
    	map<const char*,int>  months;  
    	
    	months  += 
    	    P( "january",   31 ), P( "february", 28 ),
    	    P( "march",     31 ), P( "april",    30 ),
    	    P( "may",       31 ), P( "june",     30 ),
    	    P( "july",      31 ), P( "august",   31 ),
    	    P( "september", 30 ), P( "october",  31 ),
    	    P( "november",  30 ), P( "december", 31 );
    	months.clear();
    
    	assign( months )
    	    ( "january",   31 )( "february", 28 )
    	    ( "march",     31 )( "april",    30 )
    	    ( "may",       31 )( "june",     30 )
    	    ( "july",      31 )( "august",   31 )
    	    ( "september", 30 )( "october",  31 )
    	    ( "november",  30 )( "december", 31 );
    	
    	vector<three>    v_three;
    	vector<four>     v_four;
    	vector<five>     v_five;
    	vector<six>      v_six;
    
    	assign( v_three ) (1,2,3) ("1","2","3");
    	assign( v_four ) (1,2,3,4) ("1","2","3","4");
        assign( v_five ) (1,2,3,4,5) ("1","2","3","4","5");
    	assign( v_six ) (1,2,3,4,5,6) ("1","2","3","4","5","6");
        
    }
    catch( std::exception& e )
    {
        cout << e.what() << endl;
    }

    return 0;
}
