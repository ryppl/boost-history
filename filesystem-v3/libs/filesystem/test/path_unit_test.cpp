//  filesystem3 path_unit_test.cpp  --------------------------------------------------  //

//  Copyright Beman Dawes 2008

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/filesystem

//  ----------------------------------------------------------------------------------  //
//
//  The purpose of this test is to ensure that each function in the public
//  interface can be called with arguments of the appropriate types. It does
//  not attempt to verify that the full range of values for each argument
//  are processed correctly.
//
//  For full functionality tests, including probes with many different argument
//  values, see path_test.cpp and other test programs. 
//
//  ----------------------------------------------------------------------------------  //

#include <boost/filesystem/path.hpp>
#include "../src/utf8_codecvt_facet.hpp"  // for imbue tests

#include <iostream>
#include <string>

namespace fs = boost::filesystem;
using boost::filesystem::path;
using std::cout;
using std::string;

# ifndef BOOST_FILESYSTEM_NARROW_ONLY

    using std::wstring;
    typedef std::wstring bstring;
#   define BOO_LIT( lit ) L##lit
#   define BOO_COUT std::wcout

# else

    typedef std::string bstring;
#   define BOO_LIT( lit ) lit
#   define BOO_COUT std::cout

#   endif

#define CHECK(x) check( x, __FILE__, __LINE__ )
#define PATH_IS( a, b ) check_path( a, b, __FILE__, __LINE__ )
#define IS( a,b ) check_equal( a, b, __FILE__, __LINE__ )

//  It is convenient for editing to have these ready to copy and paste
# ifdef BOOST_WINDOWS_PATH
# else  // BOOST_POSIX_PATH
# endif

# ifndef BOOST_FILESYSTEM_NARROW_ONLY
# endif  // narrow only

namespace
{
  int errors;

  boost::system::error_code ec;
  const boost::system::error_code ok;
  const boost::system::error_code ng(-1, boost::system::system_category);

  std::string platform( BOOST_PLATFORM );

  void check_path( const path & source,
              const bstring & expected, const char* file, int line )
  {
    if ( source == expected ) return;

    ++errors;

    std::cout << file;
#   ifndef BOOST_FILESYSTEM_NARROW_ONLY
    std::wcout << L'(' << line << L"): source.wstring(): \"" << source.wstring()
               << L"\" != expected: \"" << expected
               << L"\"\n" ;
#   else
    std::cout << '(' << line << "): source.string(): \"" << source.string()
              << "\" != expected: \"" << expected
              << "\"\n" ;
#   endif
  }

  template< class T1, class T2 >
  void check_equal( const T1 & value,
                    const T2 & expected, const char* file, int line )
  {
    if ( value == expected ) return;

    ++errors;

    std::cout << file;

#   ifndef BOOST_FILESYSTEM_NARROW_ONLY
    std::wcout << L'(' << line << L"): value: \"" << value
               << L"\" != expected: \"" << expected
               << L"\"\n" ;
#   else
    std::cout << '(' << line << "): value: \"" << value
              << "\" != expected: \"" << expected
              << "\"\n" ;
#   endif
   }

  void check( bool ok, const char* file, int line )
  {
    if ( ok ) return;

    ++errors;

    std::cout << file << '(' << line << "): test failed\n";
  }

  path x;
  path y;
  string s("string iterators");
# ifndef BOOST_FILESYSTEM_NARROW_ONLY
  wstring ws(L"wstring iterators");
# endif  // narrow only

  //  test_constructors  ---------------------------------------------------------------//

  void test_constructors()
  {
    std::cout << "testing constructors..." << std::endl;

    path x0;                                           // #1
    PATH_IS(x0, BOO_LIT(""));

    path x1("path x1");                                // #3
    PATH_IS(x1, BOO_LIT("path x1"));

    path x2(x1);                                       // #2
    PATH_IS(x2, BOO_LIT("path x1"));

    path x4(string("std::string::iterator").begin());  // #3
    PATH_IS(x4, BOO_LIT("std::string::iterator"));

    path x7(s.begin(), s.end());                       // #4
    PATH_IS(x7, BOO_LIT("string iterators"));

    path x10(string("std::string"));                   // #5
    PATH_IS(x10, BOO_LIT("std::string"));

# ifndef BOOST_FILESYSTEM_NARROW_ONLY
    path x3(L"const wchar_t *");                       // #3
    PATH_IS(x3, L"const wchar_t *");

    path x5(wstring(L"std::wstring::iterator").begin()); // #3
    PATH_IS(x5, L"std::wstring::iterator");

    path x8(ws.begin(), ws.end());                     // #4
    PATH_IS(x8, L"wstring iterators");

    path x11(wstring(L"std::wstring"));                // #5
    PATH_IS(x11, L"std::wstring");
# endif
  }

  //  test_use_cases  ------------------------------------------------------------------//

  string use( const path & p ) { return p; }

  void test_use_cases()
  {
    std::cout << "testing use cases..." << std::endl;

    CHECK( use("foo") == "foo" );
    CHECK( use(string("foo")) == "foo" );
    CHECK( "foo" == use("foo") );
    CHECK( "foo" == use(string("foo")) );

# ifndef BOOST_FILESYSTEM_NARROW_ONLY
    CHECK( use(L"foo") == "foo" );
    CHECK( use(wstring(L"foo")) == "foo" );
    CHECK( "foo" == use(L"foo") );
    CHECK( "foo" == use(wstring(L"foo")) );
# endif
  }

  //  test_conversion_operators  -------------------------------------------------------//

  void test_conversion_operators()
  {
    std::cout << "testing test_conversion_operators..." << std::endl;

    path p( "foo" );

    string s1( p );
    CHECK( s1 == "foo" );

    string s2 = p;
    CHECK( s2 == "foo" );

    s2.clear();
    s2 = p;
    CHECK( s2 == "foo" );

# ifndef BOOST_FILESYSTEM_NARROW_ONLY
    wstring ws1( p );
    CHECK( ws1 == L"foo" );

    wstring ws2 = p;
    CHECK( ws2 == L"foo" );

    ws2.clear();
    ws2 = p;
    CHECK( ws2 == L"foo" );
# endif

  }

  //  test_assignments  ----------------------------------------------------------------//

  void test_assignments()
  {
    std::cout << "testing assignments..." << std::endl;

    y = "path y";                                      // #2
    PATH_IS(y, L"path y");

    x = y;                                             // #1
    PATH_IS(x, L"path y");

    x = "const char *";                                // #2
    PATH_IS(x, L"const char *");

    x = L"const wchar_t *";                            // #2
    PATH_IS(x, L"const wchar_t *");

    x = string("std::string::iterator").begin();       // #2 w/iterator 
    PATH_IS(x, L"std::string::iterator");

    x = wstring(L"std::wstring::iterator").begin();    // #2 w/iterator
    PATH_IS(x, L"std::wstring::iterator");

    y.assign("path y w/o ec");                         // #3 w/o ec
    PATH_IS(y, L"path y w/o ec");

    ec = ng;
    y.assign("path y w/ec", ec);                       // #3 w/ec
    PATH_IS(y, L"path y w/ec");
    IS(ec, ok);

    x.assign(s.begin(), s.end());                      // #4
    PATH_IS(x, L"string iterators");

    ec = ng;
    x.assign(s.begin(), s.end(), ec);                  // #4 w/ec
    PATH_IS(x, L"string iterators");
    IS(ec, ok);

    x.assign(ws.begin(), ws.end());                    // #4
    PATH_IS(x, L"wstring iterators");

    x = string("std::string");                         // #5
    PATH_IS(x, L"std::string");

    x = wstring(L"std::wstring");                      // #5
    PATH_IS(x, L"std::wstring");

    x.assign(string("std::string"));                   // #6
    PATH_IS(x, L"std::string");

    x.assign(wstring(L"std::wstring"));                // #6
    PATH_IS(x, L"std::wstring");

    ec = ng;
    x.assign(string("std::string"), ec);               // #6 w/ec
    PATH_IS(x, L"std::string");
    IS(ec, ok);

   }

  //  test_observers  ------------------------------------------------------------------//

  void test_observers()
  {
    std::cout << "testing observers..." << std::endl;

# ifdef BOOST_WINDOWS_PATH

    path p( "abc\\def/ghi" );

    CHECK( wstring( p.c_str() ) == L"abc\\def/ghi" );

    CHECK( p.string() == "abc\\def/ghi" );
    CHECK( p.wstring() == L"abc\\def/ghi" );

    CHECK( p.generic().string() == "abc/def/ghi" );
    CHECK( p.generic().wstring() == L"abc/def/ghi" );

    CHECK( p.native().string() == "abc\\def\\ghi" );
    CHECK( p.native().wstring() == L"abc\\def\\ghi" );

# else  // BOOST_POSIX_PATH


# endif 
  }

//  //  test_appends  --------------------------------------------------------------------//
//
//  void test_appends()
//  {
//    std::cout << "testing appends..." << std::endl;
//
//  }

  //  test_relationals  ----------------------------------------------------------------//

  void test_relationals()
  {
    std::cout << "testing relationals..." << std::endl;

    const path p( "bar" );
    const path p2( "baz" );

    CHECK( !(p < p) );
    CHECK( p < p2 );
    CHECK( !(p2 < p) );
    CHECK( p < "baz" );
    CHECK( p < string("baz") );
    CHECK( p < L"baz" );
    CHECK( p < wstring(L"baz") );
    CHECK( !("baz" < p) );
    CHECK( !(string("baz") < p) );
    CHECK( !(L"baz" < p) );
    CHECK( !(wstring(L"baz") < p) );

    CHECK( p == p );
    CHECK( !(p == p2) );
    CHECK( !(p2 == p) );
    CHECK( p2 == "baz" );
    CHECK( p2 == string("baz") );
    CHECK( p2 == L"baz" );
    CHECK( p2 == wstring(L"baz") );
    CHECK( "baz" == p2 );
    CHECK( string("baz") == p2 );
    CHECK( L"baz" == p2 );
    CHECK( wstring(L"baz") == p2 );

    CHECK( !(p != p) );
    CHECK( p != p2 );
    CHECK( p2 != p );

    CHECK( p <= p );
    CHECK( p <= p2 );
    CHECK( !(p2 <= p) );

    CHECK( !(p > p) );
    CHECK( !(p > p2) );
    CHECK( p2 > p );

    CHECK( p >= p );
    CHECK( !(p >= p2) );
    CHECK( p2 >= p );
}

  //  test_other_non_members  ----------------------------------------------------------//

  void test_other_non_members()
  {
    std::cout << "testing other_non_members..." << std::endl;

    path p1("foo");
    path p2("bar");

    //  operator /

    CHECK( p1 / p2 == path( "foo/bar" ).native() );
    CHECK( "foo" / p2 == path( "foo/bar" ).native() );
    CHECK( L"foo" / p2 == path( "foo/bar" ).native() );
    CHECK( string( "foo" ) / p2 == path( "foo/bar" ).native() );
    CHECK( wstring( L"foo" ) / p2 == path( "foo/bar" ).native() );
    CHECK( p1 / "bar" == path( "foo/bar" ).native() );
    CHECK( p1 / L"bar" == path( "foo/bar" ).native() );
    CHECK( p1 / string( "bar" ) == path( "foo/bar" ).native() );
    CHECK( p1 / wstring( L"bar" ) == path( "foo/bar" ).native() );

    swap( p1, p2 );
    //  swap

    CHECK( p1 == "bar" );
    CHECK( p2 == "foo" );

    // swap( p1, "baz" );   // should fail to compile
    // swap( p1, L"baz" );  // should fail to compile

    CHECK( path( "" ).remove_filename() == "" );
    CHECK( path( "foo" ).remove_filename() == "" );
    CHECK( path( "foo/bar" ).remove_filename() == "foo/" );

  }

//  //  test_modifiers  ------------------------------------------------------------------//
//
//  void test_modifiers()
//  {
//    std::cout << "testing modifiers..." << std::endl;
//
//  }

  //  test_iterators  ------------------------------------------------------------------//

  void test_iterators()
  {
    std::cout << "testing iterators..." << std::endl;

    path p1;
    CHECK( p1.begin() == p1.end() );

    path p2("/");
    CHECK( p2.begin() != p2.end() );
    CHECK( *p2.begin() == "/" );
    CHECK( ++p2.begin() == p2.end() );

    path p3("foo/bar/baz");

    path::iterator it(p3.begin());
    CHECK( p3.begin() != p3.end() );
    CHECK( *it == "foo" );
    CHECK( *++it == "bar" );
    CHECK( *++it == "baz" );
    CHECK( *--it == "bar" );
    CHECK( *--it == "foo" );
    CHECK( *++it == "bar" );
    CHECK( *++it == "baz" );
    CHECK( ++it == p3.end() );

  }

  //  test_decompositions  -------------------------------------------------------------//

  void test_decompositions()
  {
    std::cout << "testing decompositions..." << std::endl;

    CHECK( path("").root_name().string() == "" );
    CHECK( path("foo").root_name().string() == "" );
    CHECK( path("/").root_name().string() == "" );
    CHECK( path("/foo").root_name().string() == "" );
    CHECK( path("//netname").root_name().string() == "//netname" );
    CHECK( path("//netname/foo").root_name().string() == "//netname" );

    CHECK( path("").root_directory().string() == "" );
    CHECK( path("foo").root_directory().string() == "" );
    CHECK( path("/").root_directory().string() == "/" );
    CHECK( path("/foo").root_directory().string() == "/" );
    CHECK( path("//netname").root_directory().string() == "" );
    CHECK( path("//netname/foo").root_directory().string() == "/" );

    CHECK( path("").root_path().string() == "" );
    CHECK( path("/").root_path().string() == "/" );
    CHECK( path("/foo").root_path().string() == "/" );
    CHECK( path("//netname").root_path().string() == "//netname" );
    CHECK( path("//netname/foo").root_path().string() == "//netname/" );

#   ifdef BOOST_WINDOWS_API
    CHECK( path("c:/foo").root_path().string() == "c:/" );
#   endif

    CHECK( path("").relative_path().string() == "" );
    CHECK( path("/").relative_path().string() == "" );
    CHECK( path("/foo").relative_path().string() == "foo" );

    CHECK( path("").parent_path().string() == "" );
    CHECK( path("/").parent_path().string() == "" );
    CHECK( path("/foo").parent_path().string() == "/" );
    CHECK( path("/foo/bar").parent_path().string() == "/foo" );

    CHECK( path("/foo/bar/baz.zoo").filename().string() == "baz.zoo" );

    CHECK( path("/foo/bar/baz.zoo").stem().string() == "baz" );
    CHECK( path("/foo/bar.woo/baz").stem().string() == "baz" );

    CHECK( path("/foo/bar/baz.zoo").extension().string() == ".zoo" );
    CHECK( path("/foo/bar.woo/baz").extension().string() == "" );
  }

  //  test_queries  --------------------------------------------------------------------//

  void test_queries()
  {
    std::cout << "testing queries..." << std::endl;

    path p1( "" );
    path p2( "//netname/foo" );

    CHECK( p1.empty() );
    CHECK( !p1.has_root_path() );
    CHECK( !p1.has_root_name() );
    CHECK( !p1.has_root_directory() );
    CHECK( !p1.has_relative_path() );
    CHECK( !p1.has_parent_path() );
    CHECK( !p1.has_filename() );
    CHECK( !p1.is_complete() );

    CHECK( !p2.empty() );
    CHECK( p2.has_root_path() );
    CHECK( p2.has_root_name() );
    CHECK( p2.has_root_directory() );
    CHECK( p2.has_relative_path() );
    CHECK( p2.has_parent_path() );
    CHECK( p2.has_filename() );
    CHECK( p2.is_complete() );

  }

  //  test_imbue_locale  ---------------------------------------------------------------//

  void test_imbue_locale()
  {
    std::cout << "testing imbue locale..." << std::endl;

    //  \u2722 and \xE2\x9C\xA2 are UTF-16 and UTF-8 FOUR TEARDROP-SPOKED ASTERISK

    path p0( L"\u2722" );  // for tests that depend on detail::convert_to_string
    CHECK( p0.string() != "\xE2\x9C\xA2" );
    string p0_string( p0.string() );

    path p1( "\xE2\x9C\xA2" );
    CHECK( p1 != L"\u2722" );
    wstring p1_wstring( p1.wstring() );

    // So that tests are run with known encoding, use Boost UTF-8 codecvt
    std::locale global_loc = std::locale();
    std::locale loc( global_loc, new fs::detail::utf8_codecvt_facet );
    std::locale old_loc = path::imbue( loc );

    CHECK( p0.string() == "\xE2\x9C\xA2" );
    path p2( "\xE2\x9C\xA2" );
    CHECK( p2 == L"\u2722" );
    CHECK( p2.wstring() == L"\u2722" );

    path::imbue( old_loc );

    CHECK( p0.string() == p0_string );
    path p3( "\xE2\x9C\xA2" );
    CHECK( p3 != L"\u2722" );
    CHECK( p3.wstring() == p1_wstring );
  }

//  //  test_locales  --------------------------------------------------------------------//
//
//  void test_locales()
//  {
//    std::cout << "testing locales..." << std::endl;
//
//  }

  //  test_user_supplied_type  ---------------------------------------------------------//

  typedef std::basic_string<int> user_string;

}  // unnamed namespace

namespace boost
{
namespace filesystem
{
  namespace path_traits
  {
    template<> struct is_iterator<const user_string::value_type *> { static const bool value = true; };
    template<> struct is_iterator<user_string::value_type *> { static const bool value = true; };
    template<> struct is_iterator<user_string::iterator> { static const bool value = true; };
    template<> struct is_iterator<user_string::const_iterator> { static const bool value = true; };
    template<> struct is_container<user_string> { static const bool value = true; };

    template<>
    void append<user_string::value_type>( const user_string::value_type * begin,
      const user_string::value_type * end, string_type & target, system::error_code & ec )
    {
      for ( ; begin != end && *begin; ++begin )
        target += *begin + 1;  // change so that results distinguishable from char cvts
    }

    template<>
    user_string convert<user_string>( const string_type & source,
      system::error_code & ec )
    {
      user_string temp;
      for ( string_type::const_iterator it = source.begin();
            it != source.end(); ++it )
        temp += *it - 1;
      return temp;
    }
  }  // namespace path_traits
}  // namespace filesystem
}  // namespace boost

namespace
{

  void test_user_supplied_type()
  {
    std::cout << "testing user supplied type..." << std::endl;

    user_string::value_type usr_c_str[] = { 'a', 'b', 'c', 0 };
    user_string usr( usr_c_str );

    path p1( usr_c_str );
    CHECK( p1 == path("bcd") );
    CHECK( p1 == "bcd" );
    user_string s1( p1.string<user_string>() );
    CHECK( s1 == usr );
  }

}  // unnamed namespace

  //------------------------------------------------------------------------------------//
  //                                                                                    //
  //                                  test_main                                         //
  //                                                                                    //
  //------------------------------------------------------------------------------------//

int main( int, char*[] )
{
  test_constructors();
  test_use_cases();
  test_conversion_operators();
  test_assignments();
  test_observers();
  ////test_appends();
  test_relationals();
  test_other_non_members();
  //test_modifiers();
  test_iterators();
  test_decompositions();
  test_queries();
  test_imbue_locale();
  //test_convert_with_locale();
  test_user_supplied_type();
 
  cout << errors << " errors detected\n";
  
  return errors;
}
