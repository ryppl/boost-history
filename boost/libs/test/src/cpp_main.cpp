//  boost/cpp_main.cpp (header or not, as you like it)  ----------------------//

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
//    4 Jun 01 Rewrite to use exception_monitor
//   26 Feb 01 Numerous changes suggested during formal review.
//   25 Jan 01 catch_exceptions.hpp code factored out.
//   22 Jan 01 Remove test_tools dependencies to reduce coupling.
//    5 Nov 00 Initial boost version (Beman Dawes)

//  header dependencies are deliberately restricted to reduce coupling.
#include <boost/test/execution_monitor.hpp>
#include <boost/cstdlib.hpp>
#include <boost/config.hpp>
#include <iostream>

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::getenv; using ::atoi; }
# endif

//  It is easiest (because cpp_main.cpp is sometimes used as a header)
//  to always compile execution_monitor.cpp in the same translation unit.  
#include <libs/test/src/execution_monitor.cpp>

int cpp_main( int argc, char * argv[] );  // prototype for user's cpp_main()

namespace
{
  class cpp_main_caller : public boost::execution_monitor
  {
   public:
    cpp_main_caller( int argc, char ** argv )
                       : _argc(argc), _argv(argv) {}

    int  function() { return cpp_main( _argc, _argv ); }

   private:
    int           _argc;
    char **       _argv;
  };
}

//  main()  ------------------------------------------------------------------//

int main( int argc, char * argv[] )
{
  cpp_main_caller caller( argc, argv );

  int result;
  
  try
  {
    result = caller.execute();

    //  Some prefer a confirming message when all is well, while others don't
    //  like the clutter.  Use an environmental variable to avoid command
    //  line argument modifications; for use in production programs
    //  that's a no-no in some organizations.
    const char * p = std::getenv( "BOOST_CPP_MAIN_CONFIRMATION" );
    if ( !p || std::atoi(p) != 0 ) 
      { std::cout << std::flush << "no errors detected" << std::endl; }
  }
  catch ( const boost::execution_exception & exex )
  {
    std::cout << "\n** Error: " << exex.what() << std::endl;
    result = exex.code();
  }

  if ( result != 0 && result != boost::exit_success )
  {
    std::cout << std::endl << "**** error return code "
              << result << std::endl;
    std::cerr
      << "**********  errors detected; see stdout for details  ***********"
      << std::endl;
  }
  return result;
}
