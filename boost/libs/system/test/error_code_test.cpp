//  error_code_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------// 

//  VC++ 8.0 warns on usage of certain Standard Library and API functions that
//  can be cause buffer overruns or no_posix_equivalent possible security issues if misused.
//  See http://msdn.microsoft.com/msdnmag/issues/05/05/SafeCandC/default.aspx
//  But the wording of the warning is misleading and unsettling, there are no
//  portable altersystem functions, and VC++ 8.0's own libraries use the
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
#   define BOOST_ACCESS_ERROR_MACRO ERROR_ACCESS_DENIED
# elif defined( BOOST_POSIX_API )
#   define BOOST_ACCESS_ERROR_MACRO EACCES
# else
#   error "Only supported for POSIX and Windows"
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

// TODO: add hash_value tests

int test_main( int, char ** )
{
  // unit tests:
  error_code ec;
  BOOST_CHECK( !ec );
  BOOST_CHECK( ec.value() == 0 );
  BOOST_CHECK( ec.posix() == 0 );
  BOOST_CHECK( ec.posix() == posix::no_error );
  BOOST_CHECK( ec == posix::no_error );
  BOOST_CHECK( ec.category() == posix_category );
  BOOST_CHECK( ec.category().name() == "POSIX" );

  error_code ec_0_system( 0, system_category );
  BOOST_CHECK( !ec_0_system );
  BOOST_CHECK( ec_0_system.value() == 0 );
  BOOST_CHECK( ec_0_system.posix() == 0 );
  BOOST_CHECK( ec_0_system.posix() == posix::no_error );
  BOOST_CHECK( ec_0_system == posix::no_error );
  BOOST_CHECK( ec_0_system.category() == system_category );
  BOOST_CHECK( ec_0_system.category().name() == "system" );
  check_ostream( ec_0_system, "system:0" );

  BOOST_CHECK( ec_0_system == ec );

  error_code ec_1_system( 1, system_category );
  BOOST_CHECK( ec_1_system );
  BOOST_CHECK( ec_1_system.value() == 1 );
  BOOST_CHECK( ec_1_system.value() != 0 );
  BOOST_CHECK( ec != ec_1_system );
  BOOST_CHECK( ec_0_system != ec_1_system );
  check_ostream( ec_1_system, "system:1" );

  ec = error_code( BOOST_ACCESS_ERROR_MACRO, system_category );
  BOOST_CHECK( ec );
  BOOST_CHECK( ec.value() == BOOST_ACCESS_ERROR_MACRO );
  BOOST_CHECK( ec.posix() == static_cast<int>(posix::permission_denied) );
  BOOST_CHECK( ec.posix() == posix::permission_denied );
  BOOST_CHECK( posix::permission_denied == ec.posix() );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.category() == system_category );
  BOOST_CHECK( ec.category().name() == "system" );

#ifdef BOOST_WINDOWS_API
  // these tests probe the Windows posix decoder
  //   test the first entry in the decoder table:
  ec = error_code( ERROR_FILE_NOT_FOUND, system_category );
  BOOST_CHECK( ec.value() == ERROR_FILE_NOT_FOUND );
  BOOST_CHECK( ec == posix::no_such_file_or_directory );
  BOOST_CHECK( ec.posix() == posix::no_such_file_or_directory );

  //   test the second entry in the decoder table:
  ec = error_code( ERROR_PATH_NOT_FOUND, system_category );
  BOOST_CHECK( ec.value() == ERROR_PATH_NOT_FOUND );
  BOOST_CHECK( ec == posix::no_such_file_or_directory );
  BOOST_CHECK( ec.posix() == posix::no_such_file_or_directory );

  //   test the third entry in the decoder table:
  ec = error_code( ERROR_ACCESS_DENIED, system_category );
  BOOST_CHECK( ec.value() == ERROR_ACCESS_DENIED );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.posix() == posix::permission_denied );

  //   test the last regular entry in the decoder table:
  ec = error_code( ERROR_WRITE_PROTECT, system_category );
  BOOST_CHECK( ec.value() == ERROR_WRITE_PROTECT );
  BOOST_CHECK( ec == posix::permission_denied );
  BOOST_CHECK( ec.posix() == posix::permission_denied );

  //   test not-in-table condition:
  ec = error_code( 1234567890, system_category );
  BOOST_CHECK( ec.value() == 1234567890 );
  BOOST_CHECK( ec == posix::no_posix_equivalent );
  BOOST_CHECK( ec.posix() == posix::no_posix_equivalent );

#else
  // TODO: write some POSIX tests
#endif
  
  return 0;
}


