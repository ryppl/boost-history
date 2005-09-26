//  path_test program  -------------------------------------------------------//

//  Copyright Beman Dawes 2002.
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

#include <boost/filesystem/operations.hpp>
#include <boost/utility.hpp>
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>

namespace fs = boost::filesystem;
using boost::filesystem::path;
using boost::next;
using boost::prior;

#include <boost/test/minimal.hpp>

#define PATH_CHECK( a, b ) check( a, b, __LINE__ )
#define DIR_CHECK( a, b ) check_dir( a, b, __LINE__ )
#define CHECK_EQUAL( a,b ) check_equal( a, b, __LINE__ )


namespace
{
  int errors;

  std::string platform( BOOST_PLATFORM );

  void check( const fs::path & source,
              const std::string & expected, int line )
  {
    if ( source.string()== expected ) return;

    ++errors;

    std::cout << '(' << line << ") source.string(): \"" << source.string()
              << "\" != expected: \"" << expected
              << "\"" << std::endl;
  }

  void check_dir( const fs::path & source,
              const std::string & expected, int line )
  {
    if ( source.directory_string()== expected ) return;

    ++errors;

    std::cout << '(' << line << ") source.directory_string(): \""
              << source.directory_string()
              << "\" != expected: \"" << expected
              << "\"" << std::endl;
  }

  void check_equal( const std::string & value,
              const std::string & expected, int line )
  {
    if ( value == expected ) return;

    ++errors;

    std::cout << '(' << line << ") value: \"" << value
              << "\" != expected: \"" << expected
              << "\"" << std::endl;
  }

  void check_throw( const std::string & arg )
  {
    try
    {
      fs::path arg_path( arg );
      ++errors;
      std::cout << "failed to throw with argument \"" << arg
                << "\"" << std::endl;
    }
    catch ( const fs::filesystem_error & /*ex*/ )
    {
//      std::cout << ex.what() << "\n";
    }
  }

  /*
  void check_normalize()
  {
    PATH_CHECK( path("").normalize(), "" );
    PATH_CHECK( path("/").normalize(), "/" );
    PATH_CHECK( path("//").normalize(), "//" );
    PATH_CHECK( path("///").normalize(), "/" );
    PATH_CHECK( path("f").normalize(), "f" );
    PATH_CHECK( path("foo").normalize(), "foo" );
    PATH_CHECK( path("foo/").normalize(), "foo/." );
    PATH_CHECK( path("f/").normalize(), "f/." );
    PATH_CHECK( path( "/foo" ).normalize(), "/foo" );
    PATH_CHECK( path( "foo/bar" ).normalize(), "foo/bar" );
    PATH_CHECK( path("..").normalize(), ".." );
    PATH_CHECK( path("../..").normalize(), "../.." );
    PATH_CHECK( path("/..").normalize(), "/.." );
    PATH_CHECK( path("/../..").normalize(), "/../.." );
    PATH_CHECK( path("../foo").normalize(), "../foo" );
    PATH_CHECK( path("foo/..").normalize(), "." );
    PATH_CHECK( path("foo/../").normalize(), "./." );
    PATH_CHECK( (path("foo") / "..").normalize() , "." );
    PATH_CHECK( path("foo/...").normalize(), "foo/..." );
    PATH_CHECK( path("foo/.../").normalize(), "foo/.../." );
    PATH_CHECK( path("foo/..bar").normalize(), "foo/..bar" );
    PATH_CHECK( path("../f").normalize(), "../f" );
    PATH_CHECK( path("/../f").normalize(), "/../f" );
    PATH_CHECK( path("f/..").normalize(), "." );
    PATH_CHECK( (path("f") / "..").normalize() , "." );
    PATH_CHECK( path("foo/../..").normalize(), ".." );
    PATH_CHECK( path("foo/../../").normalize(), "../." );
    PATH_CHECK( path("foo/../../..").normalize(), "../.." );
    PATH_CHECK( path("foo/../../../").normalize(), "../../." );
    PATH_CHECK( path("foo/../bar").normalize(), "bar" );
    PATH_CHECK( path("foo/../bar/").normalize(), "bar/." );
    PATH_CHECK( path("foo/bar/..").normalize(), "foo" );
    PATH_CHECK( path("foo/bar/../").normalize(), "foo/." );
    PATH_CHECK( path("foo/bar/../..").normalize(), "." );
    PATH_CHECK( path("foo/bar/../../").normalize(), "./." );
    PATH_CHECK( path("foo/bar/../blah").normalize(), "foo/blah" );
    PATH_CHECK( path("f/../b").normalize(), "b" );
    PATH_CHECK( path("f/b/..").normalize(), "f" );
    PATH_CHECK( path("f/b/../").normalize(), "f/." );
    PATH_CHECK( path("f/b/../a").normalize(), "f/a" );
    PATH_CHECK( path("foo/bar/blah/../..").normalize(), "foo" );
    PATH_CHECK( path("foo/bar/blah/../../bletch").normalize(), "foo/bletch" );
    PATH_CHECK( path( "//net" ).normalize(), "//net" );
    PATH_CHECK( path( "//net/" ).normalize(), "//net/" );
    PATH_CHECK( path( "//..net" ).normalize(), "//..net" );
    PATH_CHECK( path( "//net/.." ).normalize(), "//net/.." );
    PATH_CHECK( path( "//net/foo" ).normalize(), "//net/foo" );
    PATH_CHECK( path( "//net/foo/" ).normalize(), "//net/foo/." );
    PATH_CHECK( path( "//net/foo/.." ).normalize(), "//net/" );
    PATH_CHECK( path( "//net/foo/../" ).normalize(), "//net/." );

    PATH_CHECK( path( "/net/foo/bar" ).normalize(), "/net/foo/bar" );
    PATH_CHECK( path( "/net/foo/bar/" ).normalize(), "/net/foo/bar/." );
    PATH_CHECK( path( "/net/foo/.." ).normalize(), "/net" );
    PATH_CHECK( path( "/net/foo/../" ).normalize(), "/net/." );

    PATH_CHECK( path( "//net//foo//bar" ).normalize(), "//net/foo/bar" );
    PATH_CHECK( path( "//net//foo//bar//" ).normalize(), "//net/foo/bar/." );
    PATH_CHECK( path( "//net//foo//.." ).normalize(), "//net/" );
    PATH_CHECK( path( "//net//foo//..//" ).normalize(), "//net/." );

    PATH_CHECK( path( "///net///foo///bar" ).normalize(), "/net/foo/bar" );
    PATH_CHECK( path( "///net///foo///bar///" ).normalize(), "/net/foo/bar/." );
    PATH_CHECK( path( "///net///foo///.." ).normalize(), "/net" );
    PATH_CHECK( path( "///net///foo///..///" ).normalize(), "/net/." );

    if ( platform == "Windows" )
    {
      PATH_CHECK( path( "c:.." ).normalize(), "c:.." );
      PATH_CHECK( path( "c:foo/.." ).normalize(), "c:" );

      PATH_CHECK( path( "c:foo/../" ).normalize(), "c:." );

      PATH_CHECK( path( "c:/foo/.." ).normalize(), "c:/" );
      PATH_CHECK( path( "c:/foo/../" ).normalize(), "c:/." );
      PATH_CHECK( path( "c:/.." ).normalize(), "c:/.." );
      PATH_CHECK( path( "c:/../" ).normalize(), "c:/../." );
      PATH_CHECK( path( "c:/../.." ).normalize(), "c:/../.." );
      PATH_CHECK( path( "c:/../../" ).normalize(), "c:/../../." );
      PATH_CHECK( path( "c:/../foo" ).normalize(), "c:/../foo" );
      PATH_CHECK( path( "c:/../foo/" ).normalize(), "c:/../foo/." );
      PATH_CHECK( path( "c:/../../foo" ).normalize(), "c:/../../foo" );
      PATH_CHECK( path( "c:/../../foo/" ).normalize(), "c:/../../foo/." );
      PATH_CHECK( path( "c:/..foo" ).normalize(), "c:/..foo" );
    }
    else // POSIX
    {
      PATH_CHECK( path( "c:.." ).normalize(), "c:.." );
      PATH_CHECK( path( "c:foo/.." ).normalize(), "." );
      PATH_CHECK( path( "c:foo/../" ).normalize(), "./." );
      PATH_CHECK( path( "c:/foo/.." ).normalize(), "c:" );
      PATH_CHECK( path( "c:/foo/../" ).normalize(), "c:/." );
      PATH_CHECK( path( "c:/.." ).normalize(), "." );
      PATH_CHECK( path( "c:/../" ).normalize(), "./." );
      PATH_CHECK( path( "c:/../.." ).normalize(), ".." );
      PATH_CHECK( path( "c:/../../" ).normalize(), "../." );
      PATH_CHECK( path( "c:/../foo" ).normalize(), "foo" );
      PATH_CHECK( path( "c:/../foo/" ).normalize(), "foo/." );
      PATH_CHECK( path( "c:/../../foo" ).normalize(), "../foo" );
      PATH_CHECK( path( "c:/../../foo/" ).normalize(), "../foo/." );
      PATH_CHECK( path( "c:/..foo" ).normalize(), "c:/..foo" );
    }
  }
  */

} // unnamed namespace

int test_main( int, char*[] )
{
  // The choice of platform is make at runtime rather than compile-time
  // so that compile errors for all platforms will be detected even though
  // only the current platform is runtime tested.
  platform = ( platform == "Win32" || platform == "Win64" || platform == "Cygwin" )
               ? "Windows"
               : "POSIX";
  std::cout << "Platform is " << platform << '\n';

  path p1( "fe/fi/fo/fum" );
  path p2( p1 );
  path p3;
  BOOST_CHECK( p1.string() != p3.string() );
  p3 = p2;
  BOOST_CHECK( p1.string() == p3.string() );

  path p4( "foobar" );
  BOOST_CHECK( p4.string() == "foobar" );
  p4 = p4; // self-assignment
  BOOST_CHECK( p4.string() == "foobar" );

  // These verify various overloads don't cause compiler errors

  fs::exists( p1 );
  fs::exists( "foo" );
  fs::exists( std::string( "foo" ) );

  fs::exists( p1 / path( "foo" ) );
  fs::exists( p1 / "foo" );
  fs::exists( p1 / std::string( "foo" ) );

  fs::exists( "foo" / p1 );
  fs::exists( std::string( "foo" ) / p1 );

  p4 /= path( "foo" );
  p4 /= "foo";
  p4 /= std::string( "foo" );

# ifndef BOOST_NO_MEMBER_TEMPLATES
  BOOST_CHECK( p4.string() == path( p4.string().begin(), p4.string().end() ).string() );
# endif
  BOOST_CHECK( p1 != p4 );
  BOOST_CHECK( p1.string() == p2.string() );
  BOOST_CHECK( p1.string() == p3.string() );
  BOOST_CHECK( path( "foo" ).leaf() == "foo" );
  BOOST_CHECK( path( "foo" ).branch_path().string() == "" );
  BOOST_CHECK( p1.leaf() == "fum" );
  BOOST_CHECK( p1.branch_path().string() == "fe/fi/fo" );
  BOOST_CHECK( path( "" ).empty() == true );
  BOOST_CHECK( path( "foo" ).empty() == false );

  PATH_CHECK( "", "" );

  PATH_CHECK( "foo", "foo" );
  PATH_CHECK( "f", "f" );

  PATH_CHECK( "foo/", "foo/" );
  PATH_CHECK( "f/", "f/" );
  PATH_CHECK( "foo/..", "foo/.." );
  PATH_CHECK( "foo/../", "foo/../" );
  PATH_CHECK( "foo/bar/../..", "foo/bar/../.." );
  PATH_CHECK( "foo/bar/../../", "foo/bar/../../" );
  PATH_CHECK( path("") / "foo", "foo" );
  PATH_CHECK( path("") / "foo/", "foo/" );
  PATH_CHECK( path("foo") / "", "foo" );
  PATH_CHECK( path( "/" ), "/" );
  PATH_CHECK( path( "/" ) / "", "/" );
  PATH_CHECK( path( "/f" ), "/f" );

  PATH_CHECK( "/foo", "/foo" );
  PATH_CHECK( path("") / "/foo", "/foo" );
  PATH_CHECK( path("/foo") / "", "/foo" );

  //check_normalize();

  PATH_CHECK( "foo/bar", "foo/bar" );
  PATH_CHECK( path("foo") / path("bar"), "foo/bar" ); // path arg
  PATH_CHECK( path("foo") / "bar", "foo/bar" );       // const char * arg
  PATH_CHECK( path("foo") / path("woo/bar").leaf(), "foo/bar" ); // const std::string & arg
  PATH_CHECK( "foo" / path("bar"), "foo/bar" );

  PATH_CHECK( "a/b", "a/b" );  // probe for length effects
  PATH_CHECK( path("a") / "b", "a/b" );

  PATH_CHECK( "..", ".." );
  PATH_CHECK( path("..") / "", ".." );
  PATH_CHECK( path("") / "..", ".." );

  PATH_CHECK( "../..", "../.." );
  PATH_CHECK( path("..") / ".." , "../.." );

  PATH_CHECK( "/..", "/.." );
  PATH_CHECK( path("/") / ".." , "/.." );

  PATH_CHECK( "/../..", "/../.." );
  PATH_CHECK( path("/..") / ".." , "/../.." );

  PATH_CHECK( "../foo", "../foo" );
  PATH_CHECK( path("..") / "foo" , "../foo" );

  PATH_CHECK( "foo/..", "foo/.." );
  PATH_CHECK( path("foo") / ".." , "foo/.." );
  PATH_CHECK( path( "foo/..bar"), "foo/..bar" );

  PATH_CHECK( "../f", "../f" );
  PATH_CHECK( path("..") / "f" , "../f" );

  PATH_CHECK( "/../f", "/../f" );
  PATH_CHECK( path("/..") / "f" , "/../f" );

  PATH_CHECK( "f/..", "f/.." );
  PATH_CHECK( path("f") / ".." , "f/.." );

  PATH_CHECK( "foo/../..", "foo/../.." );
  PATH_CHECK( path("foo") / ".." / ".." , "foo/../.." );

  PATH_CHECK( "foo/../../..", "foo/../../.." );
  PATH_CHECK( path("foo") / ".." / ".." / ".." , "foo/../../.." );

  PATH_CHECK( "foo/../bar", "foo/../bar" );
  PATH_CHECK( path("foo") / ".." / "bar" , "foo/../bar" );

  PATH_CHECK( "foo/bar/..", "foo/bar/.." );
  PATH_CHECK( path("foo") / "bar" / ".." , "foo/bar/.." );

  PATH_CHECK( "foo/bar/../..", "foo/bar/../.." );
  PATH_CHECK( path("foo") / "bar" / ".." / "..", "foo/bar/../.." );

  PATH_CHECK( "foo/bar/../blah", "foo/bar/../blah" );
  PATH_CHECK( path("foo") / "bar" / ".." / "blah", "foo/bar/../blah" );

  PATH_CHECK( "f/../b", "f/../b" );
  PATH_CHECK( path("f") / ".." / "b" , "f/../b" );

  PATH_CHECK( "f/b/..", "f/b/.." );
  PATH_CHECK( path("f") / "b" / ".." , "f/b/.." );

  PATH_CHECK( "f/b/../a", "f/b/../a" );
  PATH_CHECK( path("f") / "b" / ".." / "a", "f/b/../a" );

  PATH_CHECK( "foo/bar/blah/../..", "foo/bar/blah/../.." );
  PATH_CHECK( path("foo") / "bar" / "blah" / ".." / "..", "foo/bar/blah/../.." );

  PATH_CHECK( "foo/bar/blah/../../bletch", "foo/bar/blah/../../bletch" );
  PATH_CHECK( path("foo") / "bar" / "blah" / ".." / ".." / "bletch", "foo/bar/blah/../../bletch" );

  PATH_CHECK( "...", "..." );
  PATH_CHECK( "....", "...." );
  PATH_CHECK( "foo/...", "foo/..." );
  PATH_CHECK( "abc.", "abc." );
  PATH_CHECK( "abc..", "abc.." );
  PATH_CHECK( "foo/abc.", "foo/abc." );
  PATH_CHECK( "foo/abc..", "foo/abc.." );

  PATH_CHECK( path(".abc"), ".abc" );
  PATH_CHECK( "a.c", "a.c" );
  PATH_CHECK( path("..abc"), "..abc" );
  PATH_CHECK( "a..c", "a..c" );
  PATH_CHECK( path("foo/.abc"), "foo/.abc" );
  PATH_CHECK( "foo/a.c", "foo/a.c" );
  PATH_CHECK( path("foo/..abc"), "foo/..abc" );
  PATH_CHECK( "foo/a..c", "foo/a..c" );

  PATH_CHECK( ".", "." );
  PATH_CHECK( path("") / ".", "." );
  PATH_CHECK( "./foo", "./foo" );
  PATH_CHECK( path(".") / "foo", "./foo" );
  PATH_CHECK( "./..", "./.." );
  PATH_CHECK( path(".") / "..", "./.." );
  PATH_CHECK( "./../foo", "./../foo" );
  PATH_CHECK( "foo/.", "foo/." );
  PATH_CHECK( path("foo") / ".", "foo/." );
  PATH_CHECK( "../.", "../." );
  PATH_CHECK( path("..") / ".", "../." );
  PATH_CHECK( "./.", "./." );
  PATH_CHECK( path(".") / ".", "./." );
  PATH_CHECK( "././.", "././." );
  PATH_CHECK( path(".") / "." / ".", "././." );
  PATH_CHECK( "./foo/.", "./foo/." );
  PATH_CHECK( path(".") / "foo" / ".", "./foo/." );
  PATH_CHECK( "foo/./bar", "foo/./bar" );
  PATH_CHECK( path("foo") / "." / "bar", "foo/./bar" );
  PATH_CHECK( "foo/./.", "foo/./." );
  PATH_CHECK( path("foo") / "." / ".", "foo/./." );
  PATH_CHECK( "foo/./..", "foo/./.." );
  PATH_CHECK( path("foo") / "." / "..", "foo/./.." );
  PATH_CHECK( "foo/./../bar", "foo/./../bar" );
  PATH_CHECK( "foo/../.", "foo/../." );
  PATH_CHECK( path(".") / "." / "..", "././.." );
  PATH_CHECK( "././..", "././.." );
  PATH_CHECK( path(".") / "." / "..", "././.." );
  PATH_CHECK( "./../.", "./../." );
  PATH_CHECK( path(".") / ".." / ".", "./../." );
  PATH_CHECK( ".././.", ".././." );
  PATH_CHECK( path("..") / "." / ".", ".././." );

  // iterator tests

  path itr_ck = "";
  path::const_iterator itr = itr_ck.begin();
  BOOST_CHECK( itr == itr_ck.end() );

  itr_ck = "/";
  itr = itr_ck.begin();
  BOOST_CHECK( *itr == std::string( "/" ) );
  BOOST_CHECK( ++itr == itr_ck.end() );
  BOOST_CHECK( *--itr == std::string( "/" ) );

  itr_ck = "foo";
  BOOST_CHECK( *itr_ck.begin() == std::string( "foo" ) );
  BOOST_CHECK( next( itr_ck.begin() ) == itr_ck.end() );
  BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "foo" ) );
  BOOST_CHECK( prior( itr_ck.end() ) == itr_ck.begin() );

  itr_ck = path( "/foo" );
  BOOST_CHECK( *itr_ck.begin() == std::string( "/" ) );
  BOOST_CHECK( *next( itr_ck.begin() ) == std::string( "foo" ) );
  BOOST_CHECK( next(next( itr_ck.begin() )) == itr_ck.end() );
  BOOST_CHECK( next( itr_ck.begin() ) == prior( itr_ck.end() ) );
  BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "foo" ) );
  BOOST_CHECK( *prior(prior( itr_ck.end() )) == std::string( "/" ) );
  BOOST_CHECK( prior(prior( itr_ck.end() )) == itr_ck.begin() );

  itr_ck = "/foo/bar";
  itr = itr_ck.begin();
  BOOST_CHECK( *itr == std::string( "/" ) );
  BOOST_CHECK( *++itr == std::string( "foo" ) );
  BOOST_CHECK( *++itr == std::string( "bar" ) );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "bar" );
  CHECK_EQUAL( *--itr, "foo" );
  CHECK_EQUAL( *--itr, "/" );

  itr_ck = "../f"; // previously failed due to short name bug
  itr = itr_ck.begin();
  CHECK_EQUAL( *itr, ".." );
  CHECK_EQUAL( *++itr, "f" );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "f" );
  CHECK_EQUAL( *--itr, ".." );

  // POSIX says treat "/foo/bar/" as "/foo/bar/."
  itr_ck = "/foo/bar/";
  itr = itr_ck.begin();
  CHECK_EQUAL( *itr, "/" );
  CHECK_EQUAL( *++itr, "foo" );
  CHECK_EQUAL( *++itr, "bar" );
  CHECK_EQUAL( *++itr, "." );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "." );
  CHECK_EQUAL( *--itr, "bar" );
  CHECK_EQUAL( *--itr, "foo" );
  CHECK_EQUAL( *--itr, "/" );

  // POSIX says treat "/f/b/" as "/f/b/."
  itr_ck = "/f/b/";
  itr = itr_ck.begin();
  CHECK_EQUAL( *itr, "/" );
  CHECK_EQUAL( *++itr, "f" );
  CHECK_EQUAL( *++itr, "b" );
  CHECK_EQUAL( *++itr, "." );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "." );
  CHECK_EQUAL( *--itr, "b" );
  CHECK_EQUAL( *--itr, "f" );
  CHECK_EQUAL( *--itr, "/" );

  itr_ck = "//net";
  itr = itr_ck.begin();
  // two leading slashes are permitted by POSIX (as implementation defined),
  // while for Windows it is always well defined (as a network name)
  CHECK_EQUAL( *itr, "//net" );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "//net" );

  itr_ck = "//net/";
  itr = itr_ck.begin();
  CHECK_EQUAL( *itr, "//net" );
  CHECK_EQUAL( *++itr, "/" );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "/" );
  CHECK_EQUAL( *--itr, "//net" );

  itr_ck = "//foo///bar///";
  itr = itr_ck.begin();
  CHECK_EQUAL( *itr, "//foo" );
  CHECK_EQUAL( *++itr, "/" );
  CHECK_EQUAL( *++itr, "bar" );
  CHECK_EQUAL( *++itr, "." );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "." );
  CHECK_EQUAL( *--itr, "bar" );
  CHECK_EQUAL( *--itr, "/" );
  CHECK_EQUAL( *--itr, "//foo" );

  itr_ck = "///foo///bar///";
  itr = itr_ck.begin();
  // three or more leading slashes are to be treated as a single slash
  CHECK_EQUAL( *itr, "/" );
  CHECK_EQUAL( *++itr, "foo" );
  CHECK_EQUAL( *++itr, "bar" );
  CHECK_EQUAL( *++itr, "." );
  BOOST_CHECK( ++itr == itr_ck.end() );
  CHECK_EQUAL( *--itr, "." );
  CHECK_EQUAL( *--itr, "bar" );
  CHECK_EQUAL( *--itr, "foo" );
  CHECK_EQUAL( *--itr, "/" );

  if ( platform == "Windows" )
  {
    itr_ck = "c:/";
    itr = itr_ck.begin();
    CHECK_EQUAL( *itr, "c:" );
    CHECK_EQUAL( *++itr, "/" );
    BOOST_CHECK( ++itr == itr_ck.end() );
    CHECK_EQUAL( *--itr, "/" );
    CHECK_EQUAL( *--itr, "c:" );

    itr_ck = "c:/foo";
    itr = itr_ck.begin();
    BOOST_CHECK( *itr == std::string( "c:" ) );
    BOOST_CHECK( *++itr == std::string( "/" ) );
    BOOST_CHECK( *++itr == std::string( "foo" ) );
    BOOST_CHECK( ++itr == itr_ck.end() );
    BOOST_CHECK( *--itr == std::string( "foo" ) );
    BOOST_CHECK( *--itr == std::string( "/" ) );
    BOOST_CHECK( *--itr == std::string( "c:" ) );

    itr_ck = "c:foo";
    itr = itr_ck.begin();
    BOOST_CHECK( *itr == std::string( "c:" ) );
    BOOST_CHECK( *++itr == std::string( "foo" ) );
    BOOST_CHECK( ++itr == itr_ck.end() );
    BOOST_CHECK( *--itr == std::string( "foo" ) );
    BOOST_CHECK( *--itr == std::string( "c:" ) );

    itr_ck = "c:foo/";
    itr = itr_ck.begin();
    BOOST_CHECK( *itr == std::string( "c:" ) );
    BOOST_CHECK( *++itr == std::string( "foo" ) );
    BOOST_CHECK( *++itr == std::string( "." ) );
    BOOST_CHECK( ++itr == itr_ck.end() );
    BOOST_CHECK( *--itr == std::string( "." ) );
    BOOST_CHECK( *--itr == std::string( "foo" ) );
    BOOST_CHECK( *--itr == std::string( "c:" ) );
  }
  else
  {
    itr_ck = "///";
    itr = itr_ck.begin();
    CHECK_EQUAL( *itr,  "/" );
    BOOST_CHECK( ++itr == itr_ck.end() );
  }

  path p;

  p = "";
  BOOST_CHECK( p.relative_path().string() == "" );
  BOOST_CHECK( p.branch_path().string() == "" );
  BOOST_CHECK( p.leaf() == "" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( !p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "/";
  BOOST_CHECK( p.relative_path().string() == "" );
  BOOST_CHECK( p.branch_path().string() == "" );
  BOOST_CHECK( p.leaf() == "/" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "/" );
  BOOST_CHECK( p.root_path().string() == "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  p = "//";
  CHECK_EQUAL( p.relative_path().string(), "" );
  CHECK_EQUAL( p.branch_path().string(), "" );
  CHECK_EQUAL( p.leaf(), "//" );
  CHECK_EQUAL( p.root_name(), "//" );
  CHECK_EQUAL( p.root_directory(), "" );
  CHECK_EQUAL( p.root_path().string(), "//" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );


  p = "///";
  CHECK_EQUAL( p.relative_path().string(), "" );
  CHECK_EQUAL( p.branch_path().string(), "" );
  CHECK_EQUAL( p.leaf(), "/" );
  CHECK_EQUAL( p.root_name(), "" );
  CHECK_EQUAL( p.root_directory(), "/" );
  CHECK_EQUAL( p.root_path().string(), "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  p = ".";
  BOOST_CHECK( p.relative_path().string() == "." );
  BOOST_CHECK( p.branch_path().string() == "" );
  BOOST_CHECK( p.leaf() == "." );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "..";
  BOOST_CHECK( p.relative_path().string() == ".." );
  BOOST_CHECK( p.branch_path().string() == "" );
  BOOST_CHECK( p.leaf() == ".." );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "foo";
  BOOST_CHECK( p.relative_path().string() == "foo" );
  BOOST_CHECK( p.branch_path().string() == "" );
  BOOST_CHECK( p.leaf() == "foo" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "/foo";
  CHECK_EQUAL( p.relative_path().string(), "foo" );
  CHECK_EQUAL( p.branch_path().string(), "/" );
  CHECK_EQUAL( p.leaf(), "foo" );
  CHECK_EQUAL( p.root_name(), "" );
  CHECK_EQUAL( p.root_directory(), "/" );
  CHECK_EQUAL( p.root_path().string(), "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  p = "/foo/";
  CHECK_EQUAL( p.relative_path().string(), "foo/" );
  CHECK_EQUAL( p.branch_path().string(), "/foo" );
  CHECK_EQUAL( p.leaf(), "." );
  CHECK_EQUAL( p.root_name(), "" );
  CHECK_EQUAL( p.root_directory(), "/" );
  CHECK_EQUAL( p.root_path().string(), "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  p = "///foo";
  CHECK_EQUAL( p.relative_path().string(), "foo" );
  CHECK_EQUAL( p.branch_path().string(), "/" );
  CHECK_EQUAL( p.leaf(), "foo" );
  CHECK_EQUAL( p.root_name(), "" );
  CHECK_EQUAL( p.root_directory(), "/" );
  CHECK_EQUAL( p.root_path().string(), "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  p = "foo/bar";
  BOOST_CHECK( p.relative_path().string() == "foo/bar" );
  BOOST_CHECK( p.branch_path().string() == "foo" );
  BOOST_CHECK( p.leaf() == "bar" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "../foo";
  BOOST_CHECK( p.relative_path().string() == "../foo" );
  BOOST_CHECK( p.branch_path().string() == ".." );
  BOOST_CHECK( p.leaf() == "foo" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "" );
  BOOST_CHECK( p.root_path().string() == "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "..///foo";
  CHECK_EQUAL( p.relative_path().string(), "..///foo" );
  CHECK_EQUAL( p.branch_path().string(), ".." );
  CHECK_EQUAL( p.leaf(), "foo" );
  CHECK_EQUAL( p.root_name(), "" );
  CHECK_EQUAL( p.root_directory(), "" );
  CHECK_EQUAL( p.root_path().string(), "" );
  BOOST_CHECK( !p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = "/foo/bar";
  BOOST_CHECK( p.relative_path().string() == "foo/bar" );
  BOOST_CHECK( p.branch_path().string() == "/foo" );
  BOOST_CHECK( p.leaf() == "bar" );
  BOOST_CHECK( p.root_name() == "" );
  BOOST_CHECK( p.root_directory() == "/" );
  BOOST_CHECK( p.root_path().string() == "/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( !p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  if ( platform == "POSIX" )
    BOOST_CHECK( p.is_complete() );
  else
    BOOST_CHECK( !p.is_complete() );

  // Both POSIX and Windows allow two leading slashs
  // (POSIX meaning is implementation defined)
  PATH_CHECK( path( "//resource" ), "//resource" );
  PATH_CHECK( path( "//resource/" ), "//resource/" );
  PATH_CHECK( path( "//resource/foo" ), "//resource/foo" );

  p = path( "//net" );
  CHECK_EQUAL( p.string(), "//net" );
  CHECK_EQUAL( p.relative_path().string(), "" );
  CHECK_EQUAL( p.branch_path().string(), "" );
  CHECK_EQUAL( p.leaf(), "//net" );
  CHECK_EQUAL( p.root_name(), "//net" );
  CHECK_EQUAL( p.root_directory(), "" );
  CHECK_EQUAL( p.root_path().string(), "//net" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( p.has_root_name() );
  BOOST_CHECK( !p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( !p.has_branch_path() );
  BOOST_CHECK( !p.is_complete() );

  p = path( "//net/" );
  BOOST_CHECK( p.relative_path().string() == "" );
  BOOST_CHECK( p.branch_path().string() == "//net" );
  BOOST_CHECK( p.leaf() == "/" );
  BOOST_CHECK( p.root_name() == "//net" );
  BOOST_CHECK( p.root_directory() == "/" );
  BOOST_CHECK( p.root_path().string() == "//net/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( !p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( p.is_complete() );

  p = path( "//net/foo" );
  BOOST_CHECK( p.relative_path().string() == "foo" );
  BOOST_CHECK( p.branch_path().string() == "//net/" );
  BOOST_CHECK( p.leaf() == "foo" );
  BOOST_CHECK( p.root_name() == "//net" );
  BOOST_CHECK( p.root_directory() == "/" );
  BOOST_CHECK( p.root_path().string() == "//net/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( p.is_complete() );

  p = path( "//net///foo" );
  CHECK_EQUAL( p.relative_path().string(), "foo" );
  CHECK_EQUAL( p.branch_path().string(), "//net/" );
  CHECK_EQUAL( p.leaf(), "foo" );
  CHECK_EQUAL( p.root_name(), "//net" );
  CHECK_EQUAL( p.root_directory(), "/" );
  CHECK_EQUAL( p.root_path().string(), "//net/" );
  BOOST_CHECK( p.has_root_path() );
  BOOST_CHECK( p.has_root_name() );
  BOOST_CHECK( p.has_root_directory() );
  BOOST_CHECK( p.has_relative_path() );
  BOOST_CHECK( p.has_leaf() );
  BOOST_CHECK( p.has_branch_path() );
  BOOST_CHECK( p.is_complete() );

  if ( platform == "Windows" )
  {
    DIR_CHECK( path( "/foo/bar/" ), "\\foo\\bar\\" );
    DIR_CHECK( path( "//foo//bar//" ), "\\\\foo\\bar\\\\" );
    DIR_CHECK( path( "///foo///bar///" ), "\\foo\\\\\\bar\\\\\\" );

    DIR_CHECK( path( "\\/foo\\/bar\\/" ), "\\\\foo\\bar\\\\" );
    DIR_CHECK( path( "\\//foo\\//bar\\//" ), "\\foo\\\\\\bar\\\\\\" );


    DIR_CHECK( path( "\\foo\\bar\\" ), "\\foo\\bar\\" );
    DIR_CHECK( path( "\\\\foo\\\\bar\\\\" ), "\\\\foo\\bar\\\\" );
    DIR_CHECK( path( "\\\\\\foo\\\\\\bar\\\\\\" ), "\\foo\\\\\\bar\\\\\\" );

    PATH_CHECK( path( "\\" ), "/" );
    PATH_CHECK( path( "\\f" ), "/f" );
    PATH_CHECK( path( "\\foo" ), "/foo" );
    PATH_CHECK( path( "foo\\bar" ), "foo/bar" );
    PATH_CHECK( path( "foo bar" ), "foo bar" );
    PATH_CHECK( path( "c:" ), "c:" );
    PATH_CHECK( path( "c:/" ), "c:/" );
    PATH_CHECK( path( "c:." ), "c:." );
    PATH_CHECK( path( "c:./foo" ), "c:./foo" );
    PATH_CHECK( path( "c:.\\foo" ), "c:./foo" );
    PATH_CHECK( path( "c:.." ), "c:.." );
    PATH_CHECK( path( "c:/." ), "c:/." );
    PATH_CHECK( path( "c:/.." ), "c:/.." );
    PATH_CHECK( path( "c:/../" ), "c:/../" );
    PATH_CHECK( path( "c:\\..\\" ), "c:/../" );
    PATH_CHECK( path( "c:/../.." ), "c:/../.." );
    PATH_CHECK( path( "c:/../foo" ), "c:/../foo" );
    PATH_CHECK( path( "c:\\..\\foo" ), "c:/../foo" );
    PATH_CHECK( path( "c:../foo" ), "c:../foo" );
    PATH_CHECK( path( "c:..\\foo" ), "c:../foo" );
    PATH_CHECK( path( "c:/../../foo" ), "c:/../../foo" );
    PATH_CHECK( path( "c:\\..\\..\\foo" ), "c:/../../foo" );
    PATH_CHECK( path( "c:foo/.." ), "c:foo/.." );
    PATH_CHECK( path( "c:/foo/.." ), "c:/foo/.." );
    PATH_CHECK( path( "c:/..foo" ), "c:/..foo" );
    PATH_CHECK( path( "c:foo" ), "c:foo" );
    PATH_CHECK( path( "c:/foo" ), "c:/foo" );
    PATH_CHECK( path( "\\\\netname" ), "//netname" );
    PATH_CHECK( path( "\\\\netname\\" ), "//netname/" );
    PATH_CHECK( path( "\\\\netname\\foo" ), "//netname/foo" );
    PATH_CHECK( path( "c:/foo" ), "c:/foo" );
    PATH_CHECK( path( "prn:" ), "prn:" );

    p = path( "c:" );
    BOOST_CHECK( p.relative_path().string() == "" );
    BOOST_CHECK( p.branch_path().string() == "" );
    BOOST_CHECK( p.leaf() == "c:" );
    BOOST_CHECK( p.root_name() == "c:" );
    BOOST_CHECK( p.root_directory() == "" );
    BOOST_CHECK( p.root_path().string() == "c:" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( !p.has_root_directory() );
    BOOST_CHECK( !p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( !p.has_branch_path() );
    BOOST_CHECK( !p.is_complete() );

    p = path( "c:foo" );
    BOOST_CHECK( p.relative_path().string() == "foo" );
    BOOST_CHECK( p.branch_path().string() == "c:" );
    BOOST_CHECK( p.leaf() == "foo" );
    BOOST_CHECK( p.root_name() == "c:" );
    BOOST_CHECK( p.root_directory() == "" );
    BOOST_CHECK( p.root_path().string() == "c:" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( !p.has_root_directory() );
    BOOST_CHECK( p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( p.has_branch_path() );
    BOOST_CHECK( !p.is_complete() );
 
    p = path( "c:/" );
    BOOST_CHECK( p.relative_path().string() == "" );
    BOOST_CHECK( p.branch_path().string() == "c:" );
    BOOST_CHECK( p.leaf() == "/" );
    BOOST_CHECK( p.root_name() == "c:" );
    BOOST_CHECK( p.root_directory() == "/" );
    BOOST_CHECK( p.root_path().string() == "c:/" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( p.has_root_directory() );
    BOOST_CHECK( !p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( p.has_branch_path() );
    BOOST_CHECK( p.is_complete() );

    p = path( "c:.." );
    BOOST_CHECK( p.relative_path().string() == ".." );
    BOOST_CHECK( p.branch_path().string() == "c:" );
    BOOST_CHECK( p.leaf() == ".." );
    BOOST_CHECK( p.root_name() == "c:" );
    BOOST_CHECK( p.root_directory() == "" );
    BOOST_CHECK( p.root_path().string() == "c:" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( !p.has_root_directory() );
    BOOST_CHECK( p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( p.has_branch_path() );
    BOOST_CHECK( !p.is_complete() );

    p = path( "c:/foo" );
    CHECK_EQUAL( p.relative_path().string(), "foo" );
    CHECK_EQUAL( p.branch_path().string(), "c:/" );
    CHECK_EQUAL( p.leaf(), "foo" );
    CHECK_EQUAL( p.root_name(), "c:" );
    CHECK_EQUAL( p.root_directory(), "/" );
    CHECK_EQUAL( p.root_path().string(), "c:/" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( p.has_root_directory() );
    BOOST_CHECK( p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( p.has_branch_path() );
    BOOST_CHECK( p.is_complete() );

    p = path( "prn:" );
    BOOST_CHECK( p.relative_path().string() == "" );
    BOOST_CHECK( p.branch_path().string() == "" );
    BOOST_CHECK( p.leaf() == "prn:" );
    BOOST_CHECK( p.root_name() == "prn:" );
    BOOST_CHECK( p.root_directory() == "" );
    BOOST_CHECK( p.root_path().string() == "prn:" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( !p.has_root_directory() );
    BOOST_CHECK( !p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( !p.has_branch_path() );
    BOOST_CHECK( !p.is_complete() );

    p = path( "\\\\net\\\\\\foo" );
    CHECK_EQUAL( p.relative_path().string(), "foo" );
    CHECK_EQUAL( p.branch_path().string(), "//net/" );
    CHECK_EQUAL( p.leaf(), "foo" );
    CHECK_EQUAL( p.root_name(), "//net" );
    CHECK_EQUAL( p.root_directory(), "/" );
    CHECK_EQUAL( p.root_path().string(), "//net/" );
    BOOST_CHECK( p.has_root_path() );
    BOOST_CHECK( p.has_root_name() );
    BOOST_CHECK( p.has_root_directory() );
    BOOST_CHECK( p.has_relative_path() );
    BOOST_CHECK( p.has_leaf() );
    BOOST_CHECK( p.has_branch_path() );
    BOOST_CHECK( p.is_complete() );

    itr_ck = path( "c:" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "c:" ) );
    BOOST_CHECK( next( itr_ck.begin() ) == itr_ck.end() );
    BOOST_CHECK( prior( itr_ck.end() ) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "c:" ) );

    itr_ck = path( "c:/" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "c:" ) );
    BOOST_CHECK( *next( itr_ck.begin() ) == std::string( "/" ) );
    BOOST_CHECK( next( next( itr_ck.begin() )) == itr_ck.end() );
    BOOST_CHECK( prior( prior( itr_ck.end() )) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "/" ) );
    BOOST_CHECK( *prior( prior( itr_ck.end() )) == std::string( "c:" ) );

    itr_ck = path( "c:foo" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "c:" ) );
    BOOST_CHECK( *next( itr_ck.begin() ) == std::string( "foo" ) );
    BOOST_CHECK( next(next( itr_ck.begin() )) == itr_ck.end() );
    BOOST_CHECK( prior(prior( itr_ck.end() )) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "foo" ) );
    BOOST_CHECK( *prior(prior( itr_ck.end() )) == std::string( "c:" ) );

    itr_ck = path( "c:/foo" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "c:" ) );
    BOOST_CHECK( *next( itr_ck.begin() ) == std::string( "/" ) );
    BOOST_CHECK( *next( next( itr_ck.begin() )) == std::string( "foo" ) );
    BOOST_CHECK( next( next( next( itr_ck.begin() ))) == itr_ck.end() );
    BOOST_CHECK( prior( prior( prior( itr_ck.end() ))) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "foo" ) );
    BOOST_CHECK( *prior( prior( itr_ck.end() )) == std::string( "/" ) );
    BOOST_CHECK( *prior( prior( prior( itr_ck.end() ))) == std::string( "c:" ) );

    itr_ck = path( "//net" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "//net" ) );
    BOOST_CHECK( next( itr_ck.begin() ) == itr_ck.end() );
    BOOST_CHECK( prior( itr_ck.end() ) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "//net" ) );

    itr_ck = path( "//net/" );
    CHECK_EQUAL( *itr_ck.begin(), "//net" );
    CHECK_EQUAL( *next( itr_ck.begin() ), "/" );
    BOOST_CHECK( next(next( itr_ck.begin() )) == itr_ck.end() );
    BOOST_CHECK( prior(prior( itr_ck.end() )) == itr_ck.begin() );
    CHECK_EQUAL( *prior( itr_ck.end() ), "/" );
    CHECK_EQUAL( *prior(prior( itr_ck.end() )), "//net" );

    itr_ck = path( "//net/foo" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "//net" ) );
    BOOST_CHECK( *next( itr_ck.begin() ) == std::string( "/" ) );
    BOOST_CHECK( *next(next( itr_ck.begin() )) == std::string( "foo" ) );
    BOOST_CHECK( next(next(next( itr_ck.begin() ))) == itr_ck.end() );
    BOOST_CHECK( prior(prior(prior( itr_ck.end() ))) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "foo" ) );
    BOOST_CHECK( *prior(prior( itr_ck.end() )) == std::string( "/" ) );
    BOOST_CHECK( *prior(prior(prior( itr_ck.end() ))) == std::string( "//net" ) );

    itr_ck = path( "prn:" );
    BOOST_CHECK( *itr_ck.begin() == std::string( "prn:" ) );
    BOOST_CHECK( next( itr_ck.begin() ) == itr_ck.end() );
    BOOST_CHECK( prior( itr_ck.end() ) == itr_ck.begin() );
    BOOST_CHECK( *prior( itr_ck.end() ) == std::string( "prn:" ) );
  } // Windows

  else
  { // POSIX
    DIR_CHECK( path( "/foo/bar/" ), "/foo/bar/" );
    DIR_CHECK( path( "//foo//bar//" ), "//foo//bar//" );
    DIR_CHECK( path( "///foo///bar///" ), "///foo///bar///" );

    p = path( "/usr/local/bin:/usr/bin:/bin" );
    BOOST_CHECK( p.file_string() == "/usr/local/bin:/usr/bin:/bin" );
  } // POSIX

  // test relational operators
                                                             
  path e, e2;
  path a( "a" );
  path a2( "a" );
  path b( "b" );

  // probe operator <
  BOOST_CHECK( !(e < e2) );
  BOOST_CHECK( e < a );
  BOOST_CHECK( a < b );
  BOOST_CHECK( !(a < a2) );

  // reality check character set is as expected
  BOOST_CHECK( std::string("a.b") < std::string("a/b") );
  // verify compare is actually lexicographical
  BOOST_CHECK( path("a/b") < path("a.b") );

  // make sure the derivative operators also work
  BOOST_CHECK( a == a2 );
  BOOST_CHECK( a != b );
  BOOST_CHECK( a <= b );
  BOOST_CHECK( a <= a2 );
  BOOST_CHECK( b >= a );
  BOOST_CHECK( a2 >= a );

  std::cout << errors << " errors detected\n";
  
  return errors;
}
