//  Boost iomanip_test.cpp test file  ----------------------------------------//

//  Copyright 2003-2004 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   29 Aug 2004  Initial version (Daryle Walker)

#include <boost/io/iomanip.hpp>      // for boost::io::newl, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK_EQUAL, etc.

#include <iomanip>  // for std::setbase, etc.
#include <ios>      // for std::ios_base, std::noskipws
#include <ostream>  // for std::ostream, std::endl
#include <sstream>  // for std::stringbuf, std::istringstream, etc.
#include <string>   // for std::string


// A stream-buffer that counts its syncs (i.e. flushes)
class sync_count_stringbuf
    : public std::stringbuf
{
    typedef std::stringbuf  base_type;

public:
    // Root types
    typedef base_type::char_type      char_type;
    typedef base_type::traits_type  traits_type;

    typedef base_type::allocator_type  allocator_type;

    // Other types
    typedef base_type::int_type  int_type;
    typedef base_type::pos_type  pos_type;
    typedef base_type::off_type  off_type;

    // Lifetime management (use automatically-defined destructor)
    explicit
    sync_count_stringbuf
    (
        std::ios_base::openmode  which = std::ios_base::in | std::ios_base::out
    )
        : base_type( which )
        , count_( 0ul )
    {
    }

    explicit
    sync_count_stringbuf
    (
        std::string const &      str,
        std::ios_base::openmode  which = std::ios_base::in | std::ios_base::out
    )
        : base_type( str, which )
        , count_( 0ul )
    {
    }

    // Accessors
    unsigned long  sync_count() const  { return this->count_; }

protected:
    // Overridden virtual functions
    virtual  int  sync()  { return ++this->count_, this->base_type::sync(); }

private:
    // Member data
    unsigned long  count_;

};


// Unit test for newl
void
newl_unit_test
(
)
{
    using boost::io::newl;

    sync_count_stringbuf  scsb;
    std::ostream          os( &scsb );

    // Do the test
    os << "Hello" << newl << "There";
    BOOST_CHECK_EQUAL( scsb.str(), "Hello\nThere" );
    BOOST_CHECK_EQUAL( 0ul, scsb.sync_count() );

    // Just to contrast....
    os << ',' << std::endl << "Boosters!" << newl;
    BOOST_CHECK_EQUAL( scsb.str(), "Hello\nThere,\nBoosters!\n" );
    BOOST_CHECK_EQUAL( 1ul, scsb.sync_count() );
}

// Unit test for skipl
void
skipl_unit_test
(
)
{
    using boost::io::skipl;

    std::istringstream  iss( "Hello\nThere\nBoosters!" );
    std::string         scratch;

    // Skip over a line of text
    iss >> skipl >> scratch;
    BOOST_CHECK_EQUAL( scratch, "There" );

    // Skip over a (now) blank line
    iss >> skipl >> scratch;
    BOOST_CHECK_EQUAL( scratch, "Boosters!" );
}

// Unit test for resetios
void
resetios_unit_test
(
)
{
    using boost::io::resetios;
    using std::ios_base;

    std::stringstream  ss( "Hello!\n" );

    // Confirm initial state
    BOOST_REQUIRE_MESSAGE( (ios_base::skipws | ios_base::dec) == ss.flags(),
     "non-standard initial stream (flag) state" );
    BOOST_REQUIRE_MESSAGE( 0 == ss.width(),
     "non-standard initial stream (width) state" );
    BOOST_REQUIRE_MESSAGE( 6 == ss.precision(),
     "non-standard initial stream (precision) state" );
    BOOST_REQUIRE_MESSAGE( ' ' == ss.fill(),
     "non-standard initial stream (fill) state" );

    // Change that state (and confirm it)
    ss << std::noskipws << std::setbase( 8 ) << std::setfill<char>( 'G' )
     << std::setprecision( 10 ) << std::setw( 3 );
    BOOST_REQUIRE_MESSAGE( ios_base::oct == ss.flags(),
     "broken stream (flag) state change" );
    BOOST_REQUIRE_MESSAGE( 3 == ss.width(),
     "broken stream (width) state change" );
    BOOST_REQUIRE_MESSAGE( 10 == ss.precision(),
     "broken stream (precision) state change" );
    BOOST_REQUIRE_MESSAGE( 'G' == ss.fill(),
     "broken stream (fill) state change" );

    // Test the change-back
    ss << resetios;
    BOOST_CHECK_EQUAL( (ios_base::skipws | ios_base::dec), ss.flags() );
    BOOST_CHECK_EQUAL( 0, ss.width() );
    BOOST_CHECK_EQUAL( 6, ss.precision() );
    BOOST_CHECK_EQUAL( ' ', ss.fill() );
}

// Unit test for new_lines
void
new_lines_unit_test
(
)
{
    using boost::io::new_lines;

    sync_count_stringbuf  scsb;
    std::ostream          os( &scsb );

    // Just like newl
    os << "Hello" << new_lines( 1, false ) << "There";
    BOOST_CHECK_EQUAL( scsb.str(), "Hello\nThere" );
    BOOST_CHECK_EQUAL( 0ul, scsb.sync_count() );

    // Doing several
    os << ',' << new_lines( 2 ) << "Boosters!";
    BOOST_CHECK_EQUAL( scsb.str(), "Hello\nThere,\n\nBoosters!" );
    BOOST_CHECK_EQUAL( 0ul, scsb.sync_count() );

    // Do a flush
    os << new_lines( 3, true );
    BOOST_CHECK_EQUAL( scsb.str(), "Hello\nThere,\n\nBoosters!\n\n\n" );
    BOOST_CHECK_EQUAL( 1ul, scsb.sync_count() );
}

// Unit test for skip_lines
void
skip_lines_unit_test
(
)
{
    using boost::io::skip_lines;

    sync_count_stringbuf  scsb( "Hello\nThere\n\nBoosters!\nGood\n\nBye Now." );
    std::istream          is( &scsb );
    std::string           scratch;

    // Just like skipl
    is >> skip_lines( 1, false ) >> scratch;
    BOOST_CHECK_EQUAL( scratch, "There" );
    BOOST_CHECK_EQUAL( 0ul, scsb.sync_count() );

    // Skip over two lines
    is >> skip_lines( 2 ) >> scratch;
    BOOST_CHECK_EQUAL( scratch, "Boosters!" );
    BOOST_CHECK_EQUAL( 0ul, scsb.sync_count() );

    // Do a sync
    is >> skip_lines( 3, true ) >> scratch;
    BOOST_CHECK_EQUAL( scratch, "Bye" );
    BOOST_CHECK_EQUAL( 1ul, scsb.sync_count() );
}

// Unit test for basic_ios_form
void
ios_form_unit_test
(
)
{
    using boost::io::ios_form;

    std::ostringstream  oss;
    int const           v1 = 5;

    // Quick test
    oss << v1;
    BOOST_CHECK_EQUAL( "5", oss.str() );
    oss.str( "" );

    // One non-flag aspect
    ios_form  f1;

    BOOST_CHECK( !f1.override_width() );
    f1.width( 4 );
    BOOST_CHECK( f1.override_width() );
    BOOST_CHECK_EQUAL( 4, f1.width() );
    oss << f1( v1 );
    BOOST_CHECK_EQUAL( "   5", oss.str() );
    oss.str( "" );

    // Continued new non-flag aspect
    BOOST_CHECK( !f1.override_fill() );
    f1.fill( '*' );
    BOOST_CHECK( f1.override_width() );
    BOOST_CHECK( f1.override_fill() );
    BOOST_CHECK_EQUAL( 4, f1.width() );
    BOOST_CHECK_EQUAL( '*', f1.fill() );
    oss << f1( v1 );
    BOOST_CHECK_EQUAL( "***5", oss.str() );
    oss.str( "" );

    // Continued with flag aspect
    BOOST_CHECK( !f1.override_flags() );
    BOOST_CHECK( !f1.override_showpos() );
    f1.showpos();
    BOOST_CHECK( f1.override_fill() );
    BOOST_CHECK( f1.override_flags() );
    BOOST_CHECK( f1.override_showpos() );
    BOOST_CHECK_EQUAL( '*', f1.fill() );
    BOOST_CHECK_EQUAL( std::ios_base::showpos, f1.overridden_flags() );
    BOOST_CHECK_EQUAL( std::ios_base::showpos, f1.flags() & f1.overridden_flags() );
    oss << f1( v1 );
    BOOST_CHECK_EQUAL( "**+5", oss.str() );
    oss.str( "" );

    // Continued with a ranged-flag aspect
    BOOST_CHECK( f1.override_flags() );
    BOOST_CHECK( !f1.override_adjustfield() );
    f1.internal();
    BOOST_CHECK( f1.override_flags() );
    BOOST_CHECK( f1.override_showpos() );
    BOOST_CHECK( f1.override_adjustfield() );
    BOOST_CHECK_EQUAL( std::ios_base::showpos | std::ios_base::adjustfield, f1.overridden_flags() );
    BOOST_CHECK_EQUAL( std::ios_base::showpos | std::ios_base::internal, f1.flags() & f1.overridden_flags() );
    oss << f1( v1 );
    BOOST_CHECK_EQUAL( "+**5", oss.str() );
    oss.str( "" );
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
     = BOOST_TEST_SUITE( "iomanip test" );

    test->add( BOOST_TEST_CASE(newl_unit_test) );
    test->add( BOOST_TEST_CASE(skipl_unit_test) );
    test->add( BOOST_TEST_CASE(resetios_unit_test) );
    test->add( BOOST_TEST_CASE(new_lines_unit_test) );
    test->add( BOOST_TEST_CASE(skip_lines_unit_test) );
    test->add( BOOST_TEST_CASE(ios_form_unit_test) );

    return test;
}
