// Boost md5_digest_test.cpp test file  --------------------------------------//

// (C) Copyright 2008 Daryle Walker.  Distributed under the Boost Software
// License, Version 1.0.  (See the accompanying file LICENSE_1_0.txt or a copy
// at <http://www.boost.org/LICENSE_1_0.txt>.)

// See <http://www.boost.org/libs/coding> for the library's home page.


#define BOOST_TEST_MODULE  "MD5 tests"

#include <boost/coding/md5.hpp>

#include <boost/archive/xml_iarchive.hpp>  // for boost::archive::xml_iarchive
#include <boost/archive/xml_oarchive.hpp>  // for boost::archive::xml_oarchive
#include <boost/lexical_cast.hpp>          // for boost::lexical_cast
#include <boost/serialization/nvp.hpp>     // for boost::serialization::make_nvp
#include <boost/test/unit_test.hpp>        // unit testing framework
#include <boost/test/output/compiler_log_formatter.hpp>  // for new formatter

#include <cstddef>  // for std::size_t
#include <cstdio>   // for EOF
#include <cwchar>   // for WEOF, std::wint_t
#include <iomanip>  // for std::setfill, setw
#include <ios>      // for std::left, uppercase
#include <istream>  // for std::basic_istream
#include <memory>   // for std::auto_ptr [for xcode_config]
#include <ostream>  // for std::basic_ostream
#include <sstream>  // for std::[w](o|i)stringstream
#include <string>   // for std::string, wstring


#pragma mark Intro stuff

// Put any using-ed types & templates, and typedefs here
using boost::coding::md5_digest;


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
 
    ~xcode_config()  {}

};  // xcode_config
 
BOOST_GLOBAL_FIXTURE(xcode_config);
#endif

// Sample MD5 message digest values
md5_digest const  md5_initial = { {0x67452301ul, 0xEFCDAB89ul, 0x98BADCFEul,
 0x10325476ul} };  // from RFC 1321, section 3.3
md5_digest const  zeros = { {0u, 0u, 0u, 0u} };  // all zeros (duh!)

// Common string representations of the above values, based on RFC 1321,
// section 3.5
char const     md5_initial_s[] =    "0123456789abcdeffedcba9876543210";
wchar_t const  md5_initial_ws[] =  L"0123456789abcdeffedcba9876543210";
char const     md5_initial_us[] =   "0123456789ABCDEFFEDCBA9876543210";
wchar_t const  md5_initial_uws[] = L"0123456789ABCDEFFEDCBA9876543210";

char const     zeros_s[] =    "00000000000000000000000000000000";
wchar_t const  zeros_ws[] =  L"00000000000000000000000000000000";

// Completely read an archive from a stream
template < typename Ch, class Tr, typename T >
void  read_xml_archive( std::basic_istream<Ch, Tr> &i, T &target, char const
 *name )
{
    boost::archive::xml_iarchive  ar( i );

    ar >> boost::serialization::make_nvp( name, target );
}

// Completely write an archive to a stream
template < typename Ch, class Tr, typename T >
void  write_xml_archive( std::basic_ostream<Ch, Tr> &o, T const &target, char
 const *name )
{
    boost::archive::xml_oarchive  ar( o );

    ar << boost::serialization::make_nvp( name, target );
}

}  // unnamed namespace


// Mark any unprintable tested types here


#pragma mark -
#pragma mark MD5 Digest Result Suite

// Test the operations of the MD5 result structure
BOOST_AUTO_TEST_SUITE( md5_digest_suite )

// Equality operators test
BOOST_AUTO_TEST_CASE( md5_digest_equality_test )
{
    // Equality operator
    md5_digest const  initial_copy = md5_initial;

    BOOST_CHECK_EQUAL( md5_initial.hash[0], initial_copy.hash[0] );
    BOOST_CHECK_EQUAL( md5_initial.hash[1], initial_copy.hash[1] );
    BOOST_CHECK_EQUAL( md5_initial.hash[2], initial_copy.hash[2] );
    BOOST_CHECK_EQUAL( md5_initial.hash[3], initial_copy.hash[3] );
    BOOST_CHECK( md5_initial == initial_copy );

    // Inequality operator
    md5_digest const  just_one = { {1u, 0u, 0u, 0u} };

    BOOST_CHECK( just_one.hash[0] != zeros.hash[0] );
    BOOST_CHECK_EQUAL( just_one.hash[1], zeros.hash[1] );
    BOOST_CHECK_EQUAL( just_one.hash[2], zeros.hash[2] );
    BOOST_CHECK_EQUAL( just_one.hash[3], zeros.hash[3] );
    BOOST_CHECK( just_one != zeros );

    // Cross- and self-checks
    BOOST_CHECK( zeros != md5_initial );
    BOOST_CHECK( md5_initial == md5_initial );
}

// Output streaming test
BOOST_AUTO_TEST_CASE( md5_digest_output_test )
{
    using boost::lexical_cast;
    using std::string;
    using std::wstring;
    using std::setfill;
    using std::setw;
    using boost::coding::detail::md5_constants;
    using std::left;
    using std::uppercase;

    // Basics, using Boost.LexicalCast
    BOOST_CHECK_EQUAL( lexical_cast<string>(md5_initial), md5_initial_s );
    BOOST_CHECK_EQUAL( lexical_cast<string>(zeros), zeros_s );

    // Use character types besides "char"
    BOOST_CHECK( lexical_cast<wstring>(md5_initial) == md5_initial_ws );
    BOOST_CHECK( lexical_cast<wstring>(zeros) == zeros_ws );

    // Use format options
    {
        std::ostringstream  oss;

        // Check generic options
        oss << setfill( '*' ) << setw( md5_constants::chars_per_digest::value +
         2 ) << left << md5_initial;
        BOOST_CHECK_EQUAL( oss.str(), string(md5_initial_s) + "**" );

        // Check upper-case flag
        oss.str( "" );
        oss << uppercase << md5_initial;
        BOOST_CHECK_EQUAL( oss.str(), md5_initial_us );

        // Redo for the all-zeros value
        oss.str( "" );
        oss << setfill( '*' ) << setw( md5_constants::chars_per_digest::value +
         2 ) << left << zeros;
        BOOST_CHECK_EQUAL( oss.str(), string(zeros_s) + "**" );

        oss.str( "" );
        oss << uppercase << zeros;
        BOOST_CHECK_EQUAL( oss.str(), zeros_s );
    }

    // Use format options on non-"char"
    {
        std::wostringstream  woss;

        // Check generic options
        woss << setfill( L'*' ) << setw( md5_constants::chars_per_digest::value
         + 2 ) << left << md5_initial;
        BOOST_CHECK( woss.str() == (wstring( md5_initial_ws ) + L"**") );

        // Check upper-case flag
        woss.str( L"" );
        woss << uppercase << md5_initial;
        BOOST_CHECK( woss.str() == md5_initial_uws );

        // Redo for the all-zeros value
        woss.str( L"" );
        woss << setfill( L'*' ) << setw( md5_constants::chars_per_digest::value
         + 2 ) << left << zeros;
        BOOST_CHECK( woss.str() == (wstring( zeros_ws ) + L"**") );

        woss.str( L"" );
        woss << uppercase << zeros;
        BOOST_CHECK( woss.str() == zeros_ws );
    }
}

// Input streaming test
BOOST_AUTO_TEST_CASE( md5_digest_input_test )
{
    using boost::lexical_cast;
    using std::string;

    // Basics, using Boost.LexicalCast
    BOOST_CHECK_EQUAL( lexical_cast<md5_digest>(md5_initial_s), md5_initial );
    BOOST_CHECK_EQUAL( lexical_cast<md5_digest>(md5_initial_us), md5_initial );
    BOOST_CHECK_EQUAL( lexical_cast<md5_digest>(zeros_s), zeros );

    // Use character types besides "char"
    BOOST_CHECK( lexical_cast<md5_digest>(md5_initial_ws) == md5_initial );
    BOOST_CHECK( lexical_cast<md5_digest>(md5_initial_uws) == md5_initial );
    BOOST_CHECK( lexical_cast<md5_digest>(zeros_ws) == zeros );

    // Check mixed letter-case
    BOOST_CHECK_EQUAL( lexical_cast<md5_digest>(
     "0123456789AbCdEfFeDcBa9876543210"), md5_initial );
    BOOST_CHECK( lexical_cast<md5_digest>(L"0123456789aBcDeFfEdCbA9876543210")
     == md5_initial );

    // Check bad formats
    {
        // Bad characters
        std::istringstream  iss( "21b0@qM78C9a" );
        md5_digest          x = md5_initial;

        iss >> x;
        BOOST_CHECK_EQUAL( x, md5_initial );
        BOOST_CHECK( iss.fail() && !iss.bad() );
        iss.clear();
        BOOST_CHECK_EQUAL( iss.peek(), static_cast<int>('@') );

        // Not enough characters
        iss.str( "31a0F" );
        iss >> x;
        BOOST_CHECK_EQUAL( x, md5_initial );
        BOOST_CHECK( iss.fail() && !iss.bad() );
        iss.clear();
        BOOST_CHECK_EQUAL( iss.peek(), EOF );
        BOOST_CHECK( iss.eof() );

        // Read 2 in a row
        iss.clear();
        iss.str( string(zeros_s) + md5_initial_s );
        iss >> x;
        BOOST_CHECK_EQUAL( x, zeros );
        iss >> x;
        BOOST_CHECK_EQUAL( x, md5_initial );
        BOOST_CHECK( iss.good() );
    }

    // Check bad formats on non-"char"
    {
        // Bad characters
        std::wistringstream  wiss( L"21b0@qM78C9a" );
        md5_digest           y = md5_initial;

        wiss >> y;
        BOOST_CHECK_EQUAL( y, md5_initial );
        BOOST_CHECK( wiss.fail() && !wiss.bad() );
        wiss.clear();
        BOOST_CHECK_EQUAL( wiss.peek(), static_cast<std::wint_t>(L'@') );

        // Not enough characters
        wiss.str( L"31a0F" );
        wiss >> y;
        BOOST_CHECK_EQUAL( y, md5_initial );
        BOOST_CHECK( wiss.fail() && !wiss.bad() );
        wiss.clear();
        BOOST_CHECK_EQUAL( wiss.peek(), WEOF );
        BOOST_CHECK( wiss.eof() );

        // Read 2 in a row (in upper-case)
        wiss.clear();
        wiss.str( std::wstring(zeros_ws) + md5_initial_uws );
        wiss >> y;
        BOOST_CHECK_EQUAL( y, zeros );
        wiss >> y;
        BOOST_CHECK_EQUAL( y, md5_initial );
        BOOST_CHECK( wiss.good() );
    }

    // Matching input vs. output
    BOOST_CHECK_EQUAL( md5_initial,
     lexical_cast<md5_digest>(lexical_cast<string>( md5_initial )) );
}

// Archiving test
BOOST_AUTO_TEST_CASE( md5_digest_serialization_test )
{
    // Write to archive
    std::stringstream  ss;
    md5_digest         test = md5_initial;
    char const         key[] = "test-digest";

    BOOST_REQUIRE_NE( test, zeros );
    BOOST_REQUIRE_EQUAL( test, md5_initial );
    write_xml_archive( ss, test, key );

    // Ensure receiving object's value is different
    test = zeros;
    BOOST_REQUIRE_NE( test, md5_initial );
    BOOST_REQUIRE_EQUAL( test, zeros );

    // Read from archive
    BOOST_TEST_MESSAGE( ss.str() );
    read_xml_archive( ss, test, key );

    // Confirm the change and proper read-back
    BOOST_CHECK_NE( test, zeros );
    BOOST_CHECK_EQUAL( test, md5_initial );
}

BOOST_AUTO_TEST_SUITE_END()
