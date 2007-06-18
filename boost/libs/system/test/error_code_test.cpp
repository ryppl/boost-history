//  error_code_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------// 

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can be cause buffer overruns or other possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable alternative functions, and VC++ 8.0's own libraries use the
//  functions in question. So turn off the warnings.
#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE

#include <boost/test/minimal.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <sstream>
#include <cerrno>

//  Although using directives are not the best programming practice, testing
//  with a boost::system using directive increases use scenario coverage.
using namespace boost::system;

# if defined( BOOST_WINDOWS_API )
#   include "winerror.h"
#   include <boost/cerrno.hpp>
# endif

namespace
{
  void check_ostream( error_code ec, const char * expected )
  {
    std::stringstream ss;
    std::string s;

    ss << ec;
    ss >> s;
    BOOST_CHECK( s == expected );
  }
}

//  test_main  ---------------------------------------------------------------//

// TODO: add message decoder tests
// TODO: add hash_value tests

int test_main( int, char ** )
{
  error_code ec;
  error_code ec_0_native( 0, native_category );
  error_code ec_0_posix( 0, posix_category );
  error_code ec_1_native( 1, native_category );
  error_code ec_1_posix( 1, posix_category );

  BOOST_CHECK( !ec );
  BOOST_CHECK( ec.value() == 0 );
  BOOST_CHECK( ec.posix() == 0 );

  BOOST_CHECK( !ec_0_native );
  BOOST_CHECK( ec_0_native.value() == 0 );
  BOOST_CHECK( ec_0_native.posix() == 0 );

  BOOST_CHECK( !ec_0_posix );
  BOOST_CHECK( ec_0_posix.value() == 0 );
  BOOST_CHECK( ec_0_posix.posix() == 0 );
  BOOST_CHECK( ec == ec_0_posix );
  BOOST_CHECK( ec_0_native != ec_0_posix );

  BOOST_CHECK( ec_1_native );
  BOOST_CHECK( ec_1_native.value() == 1 );
  BOOST_CHECK( ec_1_native.value() != 0 );
  BOOST_CHECK( ec_1_native.posix() != 0 );
  BOOST_CHECK( ec != ec_1_native );
  BOOST_CHECK( ec_0_native != ec_1_native );
  BOOST_CHECK( ec_0_posix != ec_1_native );

  BOOST_CHECK( ec_1_posix );
  BOOST_CHECK( ec_1_posix.value() == 1 );
  BOOST_CHECK( ec_1_posix.posix() == 1 );
  BOOST_CHECK( ec_1_posix.posix() != 0 );
  BOOST_CHECK( ec != ec_1_posix );
  BOOST_CHECK( ec_0_native != ec_1_posix );
  BOOST_CHECK( ec_0_posix != ec_1_posix );

  BOOST_CHECK( ec_0_posix.category() == posix_category );
  BOOST_CHECK( ec_0_posix.category() == ec_1_posix.category() );
  BOOST_CHECK( ec_0_posix.category() != native_category );
  BOOST_CHECK( ec_0_posix.category() != ec_0_native.category() );

  BOOST_CHECK( ec_0_posix.category().name() == "POSIX" );
  BOOST_CHECK( ec_0_native.category().name() == "native" );

  BOOST_CHECK( ec_0_posix.category() < ec_0_native.category() );
  BOOST_CHECK( ec_0_posix < ec_0_native );

  check_ostream( ec_0_posix, "POSIX:0" );
  check_ostream( ec_1_posix, "POSIX:1" );
  check_ostream( ec_0_native, "native:0" );
  check_ostream( ec_1_native, "native:1" );

  error_code ec_2_posix( no_such_file_or_directory );
  error_code ec_3_posix( no_such_file_or_directory, posix_category );

  BOOST_CHECK( ec_2_posix == ec_3_posix );
  BOOST_CHECK( ec_2_posix == no_such_file_or_directory );
  BOOST_CHECK( no_such_file_or_directory == ec_2_posix );


#ifdef BOOST_WINDOWS_API
  // these tests probe the Windows posix decoder
  //   test the first entry in the decoder table:
  ec = error_code( ERROR_FILE_NOT_FOUND, native_category );
  BOOST_CHECK( ec.value() == ERROR_FILE_NOT_FOUND );
  BOOST_CHECK( ec.posix() == no_such_file_or_directory );

  //   test the second entry in the decoder table:
  ec = error_code( ERROR_PATH_NOT_FOUND, native_category );
  BOOST_CHECK( ec.value() == ERROR_PATH_NOT_FOUND );
  BOOST_CHECK( ec.posix() == no_such_file_or_directory );

  //   test the third entry in the decoder table:
  ec = error_code( ERROR_ACCESS_DENIED, native_category );
  BOOST_CHECK( ec.value() == ERROR_ACCESS_DENIED );
  BOOST_CHECK( ec.posix() == permission_denied );

  //   test the last regular entry in the decoder table:
  ec = error_code( ERROR_WRITE_PROTECT, native_category );
  BOOST_CHECK( ec.value() == ERROR_WRITE_PROTECT );
  BOOST_CHECK( ec.posix() == permission_denied );

  //   test not-in-table condition:
  ec = error_code( 1234567890, native_category );
  BOOST_CHECK( ec.value() == 1234567890 );
  BOOST_CHECK( ec.posix() == other );

#else
  BOOST_CHECK( ec == ec_0_native );
#endif
  
  return 0;
}


