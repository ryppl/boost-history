// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//


#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
#  pragma warn -8091 // supress warning in Boost.Test
#  pragma warn -8057 // unused argument argc/argv in Boost.Test
#endif

#include <boost/range/iterator_range.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>
#include <string>
#include <vector>

// This should be included before "using namespace boost",
// otherwise gcc headers will be confused with boost::iterator
// namespace.
#include <boost/test/included/unit_test_framework.hpp> 

using namespace boost;
using namespace std;

struct add_one
{
    template< class T >
    T operator()( T r ) const
    {
        return r + 1;
    }
};

void check_iterator_range()
{
   
   
    typedef string::iterator               iterator;
    typedef string::const_iterator         const_iterator;
    typedef iterator_range<iterator>       irange;
    typedef iterator_range<const_iterator> cirange;
    string       str  = "hello world";
    const string cstr = "const world";
    irange r    = make_iterator_range( str );
    r           = make_iterator_range( str.begin(), str.end() );
    cirange r2  = make_iterator_range( cstr );
    r2          = make_iterator_range( cstr.begin(), cstr.end() );
    r2          = make_iterator_range( str );
 
    typedef sub_range<string>       srange;
    typedef sub_range<const string> csrange;
    srange s     = r;
    BOOST_CHECK( r == r );
    BOOST_CHECK( s == r );
    s            = make_iterator_range( str );
    csrange s2   = r;
    s2           = r2;
    s2           = make_iterator_range( cstr );
    BOOST_CHECK( r2 == r2 );
    BOOST_CHECK( s2 != r2 );
    s2           = make_iterator_range( str );
    BOOST_CHECK( !(s != s) );
    
    BOOST_CHECK( r.begin() == s.begin() );
    BOOST_CHECK( r2.begin()== s2.begin() );
    BOOST_CHECK( r.end()   == s.end() );
    BOOST_CHECK( r2.end()  == s2.end() );
    BOOST_CHECK_EQUAL( r.size(), s.size() );
    BOOST_CHECK_EQUAL( r2.size(), s2.size() );
    
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
    if( !(bool)r )
        BOOST_CHECK( false );
    if( !(bool)r2 )
        BOOST_CHECK( false );
    if( !(bool)s )
        BOOST_CHECK( false );
    if( !(bool)s2 )
        BOOST_CHECK( false );    
#else
    if( !r )
        BOOST_CHECK( false );
    if( !r2 )
        BOOST_CHECK( false );
    if( !s )
        BOOST_CHECK( false );
    if( !s2 )
        BOOST_CHECK( false );
#endif    

    cout << r << r2 << s << s2;
    
    string res  = copy_range<string>( r );
    BOOST_CHECK( equal( res.begin(), res.end(), r.begin() ) );
    
    typedef vector<char> string_type;
    string_type res2 = transform_range<string_type>( r, add_one() );
    BOOST_CHECK( res2[0] == 'i' );
    BOOST_CHECK( *res2.rbegin() == 'e' );
     
    r.empty();
    s.empty();
    r.size();
    s.size();
    
    irange singular_irange;
    BOOST_CHECK( singular_irange.empty() );
    BOOST_CHECK( singular_irange.size() == 0 );
    
    srange singular_srange;
    BOOST_CHECK( singular_srange.empty() );
    BOOST_CHECK( singular_srange.size() == 0 );

    BOOST_CHECK( empty( singular_irange ) );
    BOOST_CHECK( empty( singular_srange ) );
}


using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Range Test Suite" );

    test->add( BOOST_TEST_CASE( &check_iterator_range ) );

    return test;
}





