//  Boost iomanip_test.cpp test file  ----------------------------------------//

//  Copyright 2003-2004 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/io/> for the library's home page.

//  Revision History
//   03 Dec 2004  Initial version (Daryle Walker)

#include <boost/io/iomanip.hpp>      // for boost::io::newl, etc.
#include <boost/test/unit_test.hpp>  // for main, BOOST_CHECK_EQUAL, etc.

#include <cwchar>   // for std::wint_t
#include <iomanip>  // for std::setbase, etc.
#include <ios>      // for std::ios_base, std::noskipws
#include <ostream>  // for std::ostream, std::endl
#include <sstream>  // for std::stringbuf, std::istringstream, etc.
#include <string>   // for std::string, std::char_traits


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
    BOOST_CHECK_EQUAL( std::ios_base::showpos, f1.flags()
     & f1.overridden_flags() );
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
    BOOST_CHECK_EQUAL( std::ios_base::showpos | std::ios_base::adjustfield,
     f1.overridden_flags() );
    BOOST_CHECK_EQUAL( std::ios_base::showpos | std::ios_base::internal,
     f1.flags() & f1.overridden_flags() );
    oss << f1( v1 );
    BOOST_CHECK_EQUAL( "+**5", oss.str() );
    oss.str( "" );
}

// Unit test for expect
void
expect_unit_test
(
)
{
    using boost::io::expect;
    using std::char_traits;
    using std::ios_base;
    using std::string;
    using std::wint_t;

    typedef char_traits<char>     c_traits;
    typedef char_traits<wchar_t>  w_traits;

    // C++ string tests
    char const          test_str[] = "abcdefghij";
    std::istringstream  iss( test_str );

    char const  seg1_str[] = "abc";
    iss >> expect( string(seg1_str) );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'d' );

    iss >> expect( string(seg1_str) );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'d' );

    char const  seg2_str[] = "def";
    iss >> expect( string(seg2_str) );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'g' );

    iss >> expect( string() );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'g' );

    char const  bad1_str[] = "ghK";
    iss >> expect( string(bad1_str) );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'i' );

    char const  bad2_str[] = "ijk";
    iss >> expect( string(bad2_str) );
    BOOST_CHECK( c_traits::eq_int_type(c_traits::eof(), iss.peek()) );

    // C string tests, "char" specific
    iss.clear();
    iss.seekg( 0, ios_base::beg );

    iss >> expect( seg1_str );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'d' );

    iss >> expect( seg1_str );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'d' );

    iss >> expect( seg2_str );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'g' );

    char const  nul = '\0';
    iss >> expect( &nul );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'g' );

    iss >> expect( (char const *)0 );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'g' );

    iss >> expect( bad1_str );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'i' );

    iss >> expect( bad2_str );
    BOOST_CHECK( c_traits::eq_int_type(c_traits::eof(), iss.peek()) );

    // C string tests, non-"char"
    wchar_t const        test_wstr[] = L"abcdefghij";
    std::wistringstream  wiss( test_wstr );

    wchar_t const  seg1_wstr[] = L"abc";
    wiss >> expect( seg1_wstr );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'd' );

    wiss >> expect( seg1_wstr );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'd' );

    wchar_t const  seg2_wstr[] = L"def";
    wiss >> expect( seg2_wstr );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wchar_t const  wnul = L'\0';
    wiss >> expect( &wnul );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wiss >> expect( (wchar_t const *)0 );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wchar_t const  bad1_wstr[] = L"ghK";
    wiss >> expect( bad1_wstr );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'i' );

    wchar_t const  bad2_wstr[] = L"ijk";
    wiss >> expect( bad2_wstr );
    BOOST_CHECK( w_traits::eq_int_type(w_traits::eof(), wiss.peek()) );

    // C string tests, "char" values on non-"char" streams
    wiss.clear();
    wiss.seekg( 0, ios_base::beg );

    wiss >> expect( seg1_str );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'd' );

    wiss >> expect( seg1_str );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'd' );

    wiss >> expect( seg2_str );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wiss >> expect( &nul );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wiss >> expect( (char const *)0 );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'g' );

    wiss >> expect( bad1_str );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'i' );

    wiss >> expect( bad2_str );
    BOOST_CHECK( w_traits::eq_int_type(w_traits::eof(), wiss.peek()) );

    // Same character tests, "char" on "char"
    char const  stest_str[] = "abbccee";
    iss.clear();
    iss.seekg( 0, ios_base::beg );
    iss.str( stest_str );

    iss >> expect( 'a' );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'b' );

    iss >> expect( 'b' );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'b' );

    iss >> expect( 'b', 2 );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'c' );

    iss >> expect( 'c', 2 );
    BOOST_CHECK( iss );
    BOOST_CHECK_EQUAL( iss.peek(), (int)'e' );

    iss >> expect( 'd' );
    BOOST_CHECK( !iss );
    iss.clear();
    BOOST_CHECK_EQUAL( iss.peek(), (int)'e' );

    iss >> expect( 'e', 3 );
    BOOST_CHECK( c_traits::eq_int_type(c_traits::eof(), iss.peek()) );

    // Same character tests, "wchar_t" on "wchar_t"
    wchar_t const  stest_wstr[] = L"abbccee";
    wiss.clear();
    wiss.seekg( 0, ios_base::beg );
    wiss.str( stest_wstr );

    wiss >> expect( L'a' );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'b' );

    wiss >> expect( L'b' );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'b' );

    wiss >> expect( L'b', 2 );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'c' );

    wiss >> expect( L'c', 2 );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'e' );

    wiss >> expect( L'd' );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'e' );

    wiss >> expect( L'e', 3 );
    BOOST_CHECK( w_traits::eq_int_type(w_traits::eof(), wiss.peek()) );

    // Same character tests, "char" values on "wchar_t" streams
    wiss.clear();
    wiss.seekg( 0, ios_base::beg );

    wiss >> expect( 'a' );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'b' );

    wiss >> expect( 'b' );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'b' );

    wiss >> expect( 'b', 2 );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'c' );

    wiss >> expect( 'c', 2 );
    BOOST_CHECK( wiss );
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'e' );

    wiss >> expect( 'd' );
    BOOST_CHECK( !wiss );
    wiss.clear();
    BOOST_CHECK_EQUAL( wiss.peek(), (wint_t)L'e' );

    wiss >> expect( 'e', 3 );
    BOOST_CHECK( w_traits::eq_int_type(w_traits::eof(), wiss.peek()) );
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
    test->add( BOOST_TEST_CASE(expect_unit_test) );

    return test;
}
