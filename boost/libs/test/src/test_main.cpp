//  boost/test_main.cpp (header or not, as you like it) ----------------------//

//  (C) Copyright Beman Dawes 1995-2001. Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright notice
//  appears in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

//  This file may be included as a header file, or may be compiled and placed
//  in a library for traditional linking. It is unusual for non-template
//  non-inline implementation code to be used as a header file, but the user
//  may elect to do so because header-only implementation requires no library
//  build support. (Suggested by Ed Brey)

//  Revision History

//   10 Apr 01 Use new unit_test log features (Ullrich)
//    8 Apr 01 Use boost/test/unit_test.hpp as framework. (Beman)
//   26 Feb 01 Numerous changes suggested during formal review. (Beman)
//   22 Jan 01 Use boost/cpp_main.hpp as framework. (Beman)
//    5 Nov 00 Initial boost version (Beman Dawes)

//  header dependencies are deliberately kept minimal to reducing coupling.


#include <boost/config.hpp>           // for broken compiler workarounds
#include <boost/cstdlib.hpp>          // for exit codes
#include <boost/test/unit_test.hpp>   // for unit_test framework
#include <cstring>                    // for strcmp
#include <iostream>                   // for cout, cerr
#include <cstdlib>                    // for atoi

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::getenv; using ::atoi; using :: strcmp; }
# endif

int test_main( int argc, char * argv[] );  // prototype for user's test_main()

namespace
{
  int      argc_;
  char **  argv_;
  int      test_main_result;

  void call_test_main()
  {
    test_main_result = test_main( argc_, argv_ );

    // translate a test_main non-success return into a unit_test error
    BOOST_TEST( test_main_result == 0
                || test_main_result == boost::exit_success );
  }

}

//  main()  ------------------------------------------------------------------//

int main( int argc, char ** argv )
{
  //  set the log level from environment or command line
  const char * p = std::getenv( "BOOST_LOGLEVEL" );
  if ( p ) boost::set_loglevel( std::atoi( p ) );
  for ( int i = 1; i < argc; ++i ) // scan for command line switch
  {
    if ( std::strcmp( argv[i], "--loglevel=" ) == 0 )
    {
      boost::set_loglevel( std::atoi( argv[i]+11 ) );
      // remove command line entry so test_main() needn't worry about it.
      // note: C++ standard (3.6.1/2) requires argv[argc] be 0. 
      for ( int j = i; j < argc; ++j ) { argv[j] = argv[j+1]; }
      --argc;
      --i;
    }
  }

  //  set up and execute the test

  argc_ = argc;
  argv_ = argv;

  boost::test_suite test( argv[0] );
  test.add( BOOST_TEST_CASE( &call_test_main ) );

  int run_result = test.run();
  
  if(run_result != 0 || 
     (test_main_result != 0 &&
      test_main_result != boost::exit_success))
  {
      std::cerr << "*** Failures in test programm \"" << 
                   argv[0] << "\"" << std::endl;
      return ( test_main_result != 0
           && test_main_result != boost::exit_success )
        ? test_main_result
        : run_result;
  }
  else
  {
      return 0;
  }

}
