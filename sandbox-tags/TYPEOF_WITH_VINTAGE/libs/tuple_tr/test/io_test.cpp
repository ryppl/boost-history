// Copyright (C) 1999, 2000 Jaakko Järvi (jaakko.jarvi@cs.utu.fi)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

// -- io_test.cpp -----------------------------------------------
//
// Testing the I/O facilities of tuples


#include "boost/test/minimal.hpp"    // see "Header Implementation Option"

#include <boost/tr1/tuple.hpp>

#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>

#if defined BOOST_NO_STRINGSTREAM
#include <strstream>
#else
#include <sstream>
#endif

using boost::tr1::tuple;
using boost::tr1::make_tuple;
using boost::tr1::tuple_close;
using boost::tr1::tuple_open;
using boost::tr1::tuple_delimiter;


#if defined BOOST_NO_STRINGSTREAM
using std::ostrstream;
using std::istrstream;
typedef ostrstream useThisOStringStream;
typedef istrstream useThisIStringStream;
#else
using std::ostringstream;
using std::istringstream;
typedef ostringstream useThisOStringStream;
typedef istringstream useThisIStringStream;
#endif

using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;

int test_main(int argc, char * argv[] ) {
   (void)argc;
   (void)argv;
   using boost::tr1::tuple_close;
   using boost::tr1::tuple_open;
   using boost::tr1::tuple_delimiter;
   
  useThisOStringStream os1;

  // Set format [a, b, c] for os1
  os1 << tuple_open('[');
  os1 << tuple_close(']');
  os1 << tuple_delimiter(',');
  os1 << make_tuple(1, 2, 3);
  BOOST_TEST (os1.str() == std::string("[1,2,3]") );

  {
  useThisOStringStream os2;
  // Set format (a:b:c) for os2; 
  os2 << tuple_open('(');
  os2 << tuple_close(')');
  os2 << tuple_delimiter(':');
#if !defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  os2 << make_tuple("TUPU", "HUPU", "LUPU", 4.5);
  BOOST_TEST (os2.str() == std::string("(TUPU:HUPU:LUPU:4.5)") );
#endif
  }

  // The format is still [a, b, c] for os1
  os1 << make_tuple(1, 2, 3);
  BOOST_TEST (os1.str() == std::string("[1,2,3][1,2,3]") );

  std::ofstream tmp("temp.tmp");

#if !defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  tmp << make_tuple("One", "Two", 3);
#endif   
  tmp << tuple_delimiter(':');
  tmp << make_tuple(1000, 2000, 3000) << endl;

  tmp.close();
  
  // When reading tuples from a stream, manipulators must be set correctly:
  ifstream tmp3("temp.tmp");
  tuple<string, string, int> j;

#if !defined (BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  tmp3 >> j; 
  BOOST_TEST (tmp3.good() ); 
#endif
   
  tmp3 >> tuple_delimiter(':');
  tuple<int, int, int> i;
  tmp3 >> i; 
  BOOST_TEST (tmp3.good() ); 
   
  tmp3.close(); 


  // reading tuple<int, int, int> in format (a b c); 
  useThisIStringStream is("(100 200 300)"); 
   
  tuple<int, int, int> ti; 
  BOOST_TEST(bool(is >> ti));
  BOOST_TEST(ti == make_tuple(100, 200, 300));
   

  // Note that strings are problematic:
  // writing a tuple on a stream and reading it back doesn't work in
  // general. If this is wanted, some kind of a parseable string class
  // should be used.
  
  return 0;
}

