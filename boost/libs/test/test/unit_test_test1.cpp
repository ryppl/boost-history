//  Boost unit_test_test1.cpp  -----------------------------------------------//

//  (C) Copyright Beman Dawes 2001. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this copyright
//  notice appears in all copies. This software is provided "as is" without
//  express or implied warranty, and with no claim as to its suitability for
//  any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

//  Revision History
//  16 Jun 01  Initial version (Beman Dawes)

#include <boost/test/unit_test.hpp>
#include <iostream>

namespace
{
  enum error_type_enum
  {
    et_begin,
    et_none = et_begin,
    et_user,
    et_cpp_exception,
    et_system,
    et_fatal_user,
    et_fatal_system,
    et_end
  } error_type;

  int divide_by_zero = 0;

  // will cause an error coresponding to the current error_type;
  void error_on_demand()
  {
    switch ( error_type )
    {
      case et_none:
        BOOST_MESSAGE( "error_on_demand() BOOST_MESSAGE" );
        break;

      case et_user:
        BOOST_ERROR( "error_on_demand() BOOST_ERROR" );
        break;

      case et_fatal_user:
        BOOST_CRITICAL_ERROR( "error_on_demand() BOOST_CRITICAL_ERROR" );
        break;

      case et_cpp_exception:
        BOOST_CHECKPOINT( "error_on_demand() throw runtime_error" );
        throw std::runtime_error( "test std::runtime error what() message" );
        break;

      case et_system:
        BOOST_CHECKPOINT( "error_on_demand() divide by zero" );
        divide_by_zero = 1 / divide_by_zero;
        break;

      case et_fatal_system:
        BOOST_CHECKPOINT( "error_on_demand() write to an invalid address" );
        {
          int * p = 0;
          *p = 0;
        }
        break;

      default: BOOST_ERROR( "Should never reach this code!" );
    }
    return;
  }

  enum error_location_enum
  {
    el_begin,
    el_constructor = el_begin,
    el_member_function,
    el_destructor,
    el_non_member_function,
    el_end
  } error_location;

  //  simulated user classes to be tested  -----------------------------------//

  class bad
  {
   public:
  
    bad()
    {
      if ( error_location == el_constructor )
      {
        std::cout << "(constructor)\n";
        error_on_demand();
      }
    }

    int operator*()
    {
      if ( error_location == el_member_function )
      {
        std::cout << "(member function)\n";
        error_on_demand();
      }
      return 0;
    }

    ~bad()
    {
      if ( error_location == el_destructor )
      {
        std::cout << "(destructor)\n";
        error_on_demand();
      }
    }
  };

  //  tests  -----------------------------------------------------------------//

  struct bad_test
  {
    bad _bg;

    void test() { int t = *_bg; }
  };

  //  free function tests  ---------------------------------------------------//

  void bad_function()
  {
    std::cout << "(non-member function)\n";
    error_on_demand();
  }


}  // unnamed namespace

//  main()  ------------------------------------------------------------------//

int main( int argc, char * argv[] )
{
  // for each log level
  for ( boost::log::report_level level = boost::log::report_successful_tests;
        level <= boost::log::report_nothing;
        level = static_cast<boost::log::report_level>(level+1) )
  {
    boost::set_loglevel( level );

    // for each error type
    for ( error_type = et_begin;
          error_type != et_end;
          error_type = static_cast<error_type_enum>(error_type+1) )
    {
      // for each error location
      for ( error_location = el_begin;
            error_location != el_end;
            error_location = static_cast<error_location_enum>(error_location+1) )
      {
        std::cout << "\n===========================\n\n"
                  << "log level: " << int(level) 
                  << " error type: " << int(error_type)
                  << " location: " << int(error_location) << std::endl;
        // In typical user code, multiple test cases would be added to a single
        // test suite.  But for testing the unit test code itself, it is easier
        // to isolate each case in its own test suite.
        boost::test_suite test( "Unit Test Test" );
        if ( error_location != el_non_member_function )
          { test.add( BOOST_TEST_CASE( &bad_test::test ) ); }
        else
          { test.add( BOOST_TEST_CASE( &bad_function ) ); }
        std::cout << "run() returned " << test.run() << std::endl;
      }
    }
  }

  return 0;
} // main

