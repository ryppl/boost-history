//  Boost auto_array_test.cpp test file  -------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/memory/> for the library's home page.

//  Revision History
//   16 Jun 2003  Initial version (Daryle Walker)

#include <boost/memory/auto_array.hpp>  // for boost::memory::auto_array
#include <boost/test/unit_test.hpp>     // for main, BOOST_CHECK, etc.

#include <algorithm>  // for std::fill
#include <cstddef>    // for std::size_t
#include <numeric>    // for std::accumulate, std::partial_sum


// Sample function that returns auto-arrays
boost::memory::auto_array<int>
source
(
    std::size_t  size
)
{
    boost::memory::auto_array<int>  a( size );

    std::fill( &a[0], &a[size], 1 );
    std::partial_sum( &a[0], &a[size], &a[0] );

    return a;
}

// Sample function that takes auto-arrays
int
sink
(
    boost::memory::auto_array<int>  a,
    std::size_t                     size
)
{
    return std::accumulate( &a[0], &a[size], 0 );
}


// Unit test for the template class auto_array
void
auto_array_unit_test
(
)
{
    typedef boost::memory::auto_array<int>  aaint_type;

    // default initialization
    aaint_type  a;

    BOOST_CHECK( !a.get() );

    // new array initialization
    {
        aaint_type  b( new int[3] );

        BOOST_CHECK( b.get() );

        a = b;
        BOOST_CHECK( !b.get() );
    }

    BOOST_CHECK( a.get() );

    // release...
    int *  p = a.release();

    BOOST_CHECK( !a.get() );
    BOOST_CHECK( p );

    // ...and old array initialization
    {
        aaint_type  c( p );

        BOOST_CHECK( c.get() == p );

        p = 0;
        BOOST_CHECK( c.get() );
    }

    BOOST_CHECK( !p );

    // array size initialization...
    int const          numbers[] = { 1, 2, 3, 4, 5 };
    std::size_t const  numbers_length = sizeof(numbers) / sizeof(numbers[0]);

    {
        aaint_type  d( numbers_length );

        BOOST_CHECK( d.get() );

        // ...and accessor check
        std::copy( numbers, numbers + numbers_length, d.get() );
        BOOST_CHECK_EQUAL( 1, d[0] );
        BOOST_CHECK_EQUAL( 2, d[1] );
        BOOST_CHECK_EQUAL( 3, d[2] );
        BOOST_CHECK_EQUAL( 4, d[3] );
        BOOST_CHECK_EQUAL( 5, d[4] );
    }

    // function copy out (should use ref type)...
    a = source( numbers_length );
    BOOST_CHECK( a.get() );
    BOOST_CHECK_EQUAL_COLLECTIONS( numbers, numbers + numbers_length, a.get() );

    // function copy in
    int const  sum = sink( a, numbers_length );

    BOOST_CHECK_EQUAL( 15, sum );
    BOOST_CHECK( !a.get() );

    // copy construction
    a = source( numbers_length );

    {
        aaint_type  e( a );

        BOOST_CHECK( e.get() );
        BOOST_CHECK_EQUAL_COLLECTIONS( numbers, numbers + numbers_length,
         e.get() );
    }

    BOOST_CHECK( !a.get() );

    // resetting (and hopefully with conversion from ref type)
    a = source( numbers_length );
    BOOST_CHECK( a.get() );

    {
        aaint_type  f( source(numbers_length) );

        BOOST_CHECK( f.get() );
        BOOST_CHECK( a.get() != f.get() );

        a.reset();
        BOOST_CHECK( !a.get() );

        a.reset( f.release() );
        BOOST_CHECK( !f.get() );
    }

    BOOST_CHECK( a.get() );

    // check for self-reset
    a.reset( a.get() );
    BOOST_CHECK( a.get() );
}


// Unit test program
boost::unit_test_framework::test_suite *
init_unit_test_suite
(
    int         ,   // "argc" is unused
    char *      []  // "argv" is unused
)
{
    boost::unit_test_framework::test_suite *  test
     = BOOST_TEST_SUITE( "auto_array test" );

    test->add( BOOST_TEST_CASE(auto_array_unit_test) );

    return test;
}
