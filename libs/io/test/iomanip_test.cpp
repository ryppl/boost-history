//  Boost iomanip_test.cpp test file  ----------------------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   19 Nov 2003  Initial version (Daryle Walker)

#include <boost/io/iomanip.hpp>      // for boost::io::newl, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK_EQUAL, etc.

#include <iomanip>  // for std::setbase, etc.
#include <ios>      // for std::ios_base, std::noskipws
#include <ostream>  // for std::ostream, std::endl
#include <sstream>  // for std::stringbuf, std::istringstream, etc.
#include <string>   // for std::string


// A stream-buffer that counts its flushes
class flush_count_stringbuf
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
    flush_count_stringbuf
    (
        std::ios_base::openmode  which = std::ios_base::in | std::ios_base::out
    )
        : base_type( which )
        , count_( 0ul )
    {
    }

    explicit
    flush_count_stringbuf
    (
        std::string const &      str,
        std::ios_base::openmode  which = std::ios_base::in | std::ios_base::out
    )
        : base_type( str, which )
        , count_( 0ul )
    {
    }

    // Accessors
    unsigned long  flush_count() const  { return this->count_; }

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

    flush_count_stringbuf  fcsb;
    std::ostream           os( &fcsb );

    // Do the test
    os << "Hello" << newl << "There";
    BOOST_CHECK_EQUAL( fcsb.str(), "Hello\nThere" );
    BOOST_CHECK_EQUAL( 0ul, fcsb.flush_count() );

    // Just to contrast....
    os << ',' << std::endl << "Boosters!" << newl;
    BOOST_CHECK_EQUAL( fcsb.str(), "Hello\nThere,\nBoosters!\n" );
    BOOST_CHECK_EQUAL( 1ul, fcsb.flush_count() );
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

    return test;
}
