// Boost hat_test.cpp test file  ---------------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/containers> for the library's home page.

#define BOOST_TEST_MODULE  "Hat tests"

#include <boost/containers/hat.hpp>

#include <boost/test/unit_test.hpp>                      // unit-test framework
#include <boost/test/output/compiler_log_formatter.hpp>  // for new formatter
#include <boost/mpl/assert.hpp>                          // for BOOST_MPL_ASSERT
#include <boost/random/mersenne_twister.hpp>             // for boost::mt19937
#include <boost/type_traits/is_same.hpp>                 // for boost::is_same

#include <algorithm>  // for std::copy, remove, sort, mismatch
#include <cmath>      // for std::abs
#include <complex>    // for std::complex
#include <ctime>      // for std::time
#include <deque>      // for std::deque
#include <iterator>   // for std::ostream_iterator
#include <map>        // for std::map
#include <memory>     // for std::auto_ptr [for xcode_config]
#include <ostream>    // for std::(basic_)ostream [even for xcode_log_formatter]


#pragma mark Intro stuff

// Put any using-ed types & templates, and typedefs here


// Put custom types/templates, helper functions, and objects here
namespace
{

#ifdef __APPLE_CC__
/* Xcode-compatible logging format, idea by Richard Dingwall at
   <http://richarddingwall.name/2008/06/01/using-the-boost-unit-test-framework-
   with-xcode-3/>.
*/
class xcode_log_formatter
    : public boost::unit_test::output::compiler_log_formatter
{
protected:
    virtual  void  print_prefix( std::ostream &o, boost::unit_test::const_string
     file, std::size_t line )
    {
        o << file << ':' << line << ": ";
    }

};  // xcode_log_formatter

class xcode_config
{
public:
    xcode_config()
    {
        std::auto_ptr<xcode_log_formatter>  p( new xcode_log_formatter );

        boost::unit_test::unit_test_log.set_formatter( p.get() );
        p.release();
    }

};  // xcode_config
 
BOOST_GLOBAL_FIXTURE(xcode_config);
#endif

// Sample wrapping container to inspect what's happening
template < typename T, class C = std::deque<T>, class R = boost::mt19937 >
class my_container
    : public boost::containers::hat<T, C, R>
{
    typedef boost::containers::hat<T, C, R>  base_type;

public:
    // Forwarding constructors
    explicit  my_container( R const &e = R(), C const &c = C() )
      : base_type( e, c )  {}

    template < typename InputIterator >
    my_container( InputIterator f, InputIterator l, R const &e = R(), C const &c
     = C() )  : base_type( f, l, e, c )  {}

    // Move the protected members to public for testing
    using base_type::c;
    using base_type::engine;

};

template < typename T1, typename T2, class C1, class C2, class R1, class R2 >
inline bool
operator ==( my_container<T1,C1,R1> const &l, my_container<T2,C2,R2> const &r )
{ return ( l.c == r.c ) && ( l.engine() == r.engine() ); }

template < typename T1, typename T2, class C1, class C2, class R1, class R2 >
inline bool
operator !=( my_container<T1,C1,R1> const &l, my_container<T2,C2,R2> const &r )
{ return !operator ==( l, r ); }

template < typename Ch, class Tr, typename T, class C, class R >
std::basic_ostream<Ch, Tr> &
operator <<( std::basic_ostream<Ch, Tr> &o, my_container<T, C, R> const &h )
{
    o << '[';
    std::copy( h.c.begin(), h.c.end(), std::ostream_iterator<T>(o, ", ") );
    return o << h.engine() << ']';
}

typedef std::complex<double>  complexd;
typedef my_container<complexd>  cd_hat;

}  // unnamed namespace


// Mark any unprintable tested types here
BOOST_TEST_DONT_PRINT_LOG_VALUE( std::deque<complexd> );


#pragma mark -
#pragma mark Hat Suite

// Test the operations of the hat container adapter
BOOST_AUTO_TEST_SUITE( hat_suite )

// Initialization and member access
BOOST_AUTO_TEST_CASE( hat_init_mem_test )
{
    using boost::is_same;
    using boost::mt19937;

    typedef std::deque<complexd>  cd_deque;

    // Type checks
    BOOST_MPL_ASSERT(( is_same<cd_deque, cd_hat::container_type> ));
    BOOST_MPL_ASSERT(( is_same<mt19937, cd_hat::engine_type> ));

    BOOST_MPL_ASSERT(( is_same<complexd, cd_hat::value_type> ));
    BOOST_MPL_ASSERT(( is_same<complexd &, cd_hat::reference> ));
    BOOST_MPL_ASSERT(( is_same<complexd const &, cd_hat::const_reference> ));

    // Default construction
    cd_hat const  c1;

    BOOST_CHECK_EQUAL( c1.c, cd_deque() );
    BOOST_CHECK_EQUAL( c1.engine(), mt19937() );

    // Member construction
    cd_hat const  c2( mt19937(17u) );

    BOOST_CHECK_EQUAL( c2.c, cd_deque() );
    BOOST_CHECK_NE( c2.engine(), mt19937() );  // const member access tests

    cd_hat  c3( mt19937(21u), cd_deque(3, complexd( 6.0, -8.0 )) );

    BOOST_CHECK_NE( c3.c, cd_deque() );
    BOOST_CHECK_NE( c3.engine(), mt19937() );  // non-const member access tests

    complexd const  samples[ 5 ] = { complexd(6.0, -8.0), complexd(6.0, -8.0),
     complexd(6.0, -8.0), complexd(-9.0, 2.0), complexd(10.5, 11.0) };
    cd_hat const    c4( samples + 3, samples + 5, mt19937(3u), cd_deque(3,
     complexd( 6.0, -8.0 )) );

    BOOST_CHECK_NE( c4.c, cd_deque() );
    BOOST_CHECK_NE( c4.engine(), mt19937() );

    BOOST_CHECK_EQUAL_COLLECTIONS( c3.c.begin(), c3.c.end(), samples, samples +
     3 );
    BOOST_CHECK_EQUAL_COLLECTIONS( c4.c.begin(), c4.c.end(), samples, samples +
     5 );

    // Should the copy constructor and the copy-assignment operator be tested?
    // They're automatically generated by the compiler, so why should I test
    // code that I didn't have to write?
}

// Pushing and counting elements
BOOST_AUTO_TEST_CASE( hat_push_count_test )
{
    // Empty
    cd_hat  c1;

    BOOST_CHECK_EQUAL( c1.size(), 0u );
    BOOST_CHECK( c1.empty() );

    // One element
    c1.push( complexd(1.0, 2.0) );

    BOOST_CHECK_EQUAL( c1.size(), 1u );
    BOOST_CHECK( !c1.empty() );
    BOOST_CHECK_EQUAL( c1.c.back(), complexd(1.0, 2.0) );

    // Two elements
    c1.push( complexd(-4.5, 3.0) );

    BOOST_CHECK_EQUAL( c1.size(), 2u );
    BOOST_CHECK_EQUAL( c1.c.back(), complexd(-4.5, 3.0) );
    BOOST_CHECK_EQUAL( c1.c.front(), complexd(1.0, 2.0) );

    // Three elements
    complexd const  samples[ 3 ] = { complexd(1.0, 2.0), complexd(-4.5, 3.0),
     complexd(0.0, -5.0) };

    c1.push( samples[2] );

    BOOST_CHECK_EQUAL( c1.size(), 3u );
    BOOST_CHECK_EQUAL( c1.c.back(), complexd(0.0, -5.0) );
    BOOST_CHECK_EQUAL( c1.c.front(), complexd(1.0, 2.0) );
    BOOST_CHECK_EQUAL_COLLECTIONS( c1.c.begin(), c1.c.end(), samples, samples +
     3 );
}

// Choosing, with or without tokens
BOOST_AUTO_TEST_CASE( hat_choose_and_iterator_test )
{
    int const       samples[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int const       sample_size = sizeof(samples) / sizeof(samples[0]);
    cd_hat          c1( samples, samples + sample_size );
    cd_hat const &  c2( c1 );

    // Non-mutable access
    cd_hat::const_token  t1 = c2.pick();
    complexd const       &s1 = *t1, &s2 = c2.top();

    BOOST_CHECK_LE( 0, t1->real() );
    BOOST_CHECK_LE( t1->real(), 9 );
    BOOST_CHECK_EQUAL( s1.imag(), 0 );
    BOOST_CHECK_EQUAL( s2.imag(), 0 );
    BOOST_CHECK_GE( s2.real(), 0 );
    BOOST_CHECK_GE( 9, s2.real() );

    // Mutable access
    cd_hat::token  t2 = c1.pick();
    complexd       &s3 = *t2, &s4 = c1.top();

    BOOST_CHECK_LE( 0, t2->real() );
    BOOST_CHECK_LE( t2->real(), 9 );
    BOOST_CHECK_EQUAL( s3.imag(), 0 );
    BOOST_CHECK_EQUAL( s4.imag(), 0 );
    BOOST_CHECK_GE( s4.real(), 0 );
    BOOST_CHECK_GE( 9, s4.real() );

    // Actually mutate; also check token conversion
    cd_hat::const_token  t3( t2 );
    complexd const       old = *t3;

    BOOST_CHECK_EQUAL( &*t2, &*t3 );
    *t2 = 15.0;
    BOOST_CHECK_EQUAL( 15.0, *t3 );
    *t2 = old;
    BOOST_CHECK_EQUAL( old, *t3 );

    // Mutable-token comparison
    cd_hat::token  x1 = { c1.c.begin() }, x2 = x1, x3 = { c1.c.end() - 1 };

    BOOST_CHECK( x1 == x2 );
    BOOST_CHECK( x2 != x3 );

    // Const-token comparison
    cd_hat::const_token  y1 = x3, y2 = y1, y3 = { c2.c.begin() };

    BOOST_CHECK( y1 == y2 );
    BOOST_CHECK( y2 != y3 );

    // Cross-mutability token comparison, all combinations
    BOOST_CHECK( t2 == t3 );
    BOOST_CHECK( !(t3 != t2) );
    BOOST_CHECK( t3 == t2 );
    BOOST_CHECK( !(t2 != t3) );
}

// Choosing and popping elements
BOOST_AUTO_TEST_CASE( hat_choose_pop_test )
{
    typedef int                           sample_type;
    typedef my_container<sample_type>  container_type;

    int const       samples[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int const       sample_size = sizeof(samples) / sizeof(samples[0]);
    container_type  c1( samples, samples + sample_size,
     boost::mt19937(std::time( 0 )) );

    // Check random sampling trends
    {
        typedef long                             count_type;
        typedef std::map<sample_type, count_type>  map_type;

        map_type          counts;
        count_type const  single_trial_size = 10000;
        count_type const  total_trial_size = sample_size * single_trial_size;
        double const      expected_result( single_trial_size );

        for ( count_type  i = 0 ; i < total_trial_size ; ++i )
            ++counts[ c1.top() ];

        BOOST_CHECK_CLOSE( double(counts[0]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[1]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[2]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[3]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[4]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[5]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[6]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[7]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[8]), expected_result, 5.0 );
        BOOST_CHECK_CLOSE( double(counts[9]), expected_result, 5.0 );
    }

    // Check popping
    {
        using std::remove;
        using std::sort;

        sample_type  scratch[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        sample_type  *stop = scratch + sizeof(scratch) / sizeof(scratch[0]);

        // Remove via mutable-token
        container_type::token  t1 = c1.pick();

        BOOST_REQUIRE_EQUAL_COLLECTIONS( scratch,stop,c1.c.begin(),c1.c.end() );
        BOOST_REQUIRE_EQUAL( 1, stop - remove(scratch, stop, *t1) );
        c1.pop( t1 );
        sort( c1.c.begin(), c1.c.end() );
        BOOST_REQUIRE_EQUAL_COLLECTIONS( scratch, --stop, c1.c.begin(),
         c1.c.end() );

        // Remove via non-mutable-token
        container_type::const_token  t2 = c1.pick();

        BOOST_REQUIRE_EQUAL( 1, stop - remove(scratch, stop, *t2) );
        c1.pop( t2 );
        sort( c1.c.begin(), c1.c.end() );
        BOOST_REQUIRE_EQUAL_COLLECTIONS( scratch, --stop, c1.c.begin(),
         c1.c.end() );

        // Remove via regular "pop;" the tedious part is determining which
        // element was removed, so it can be removed from "scratch" too
        c1.pop();
        BOOST_REQUIRE_EQUAL( 1, stop - remove(scratch, stop,
         *std::mismatch( c1.c.begin(), c1.c.end(), scratch ).second) );
        sort( c1.c.begin(), c1.c.end() );
        BOOST_REQUIRE_EQUAL_COLLECTIONS( scratch, --stop, c1.c.begin(),
         c1.c.end() );
    }
}

BOOST_AUTO_TEST_SUITE_END()
