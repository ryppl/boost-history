//  boost persistence_test2.cpp  ---------------------------------------------//

//  (C) Copyright Beman Dawes 2001. Permission to copy, use, modify, sell
//  and distribute this software is granted provided this copyright notice
//  appears in all copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates and documentation.

//  Revision History
//  15 Apr 01  Initial version (Beman Dawes)

#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

#include <boost/config.hpp>
# ifndef BOOST_NO_STRINGSTREAM
#  include <sstream>
# else
#  include <strstream>
# endif

#include <iostream>
#include <string>
#include <boost/limits.hpp>  // workaround missing <limits>
#include <boost/persistence/persistence.hpp>
#include <boost/persistence/text.hpp>
#include <boost/persistence/binary.hpp>
//#include <boost/persistence/xdr.hpp>

# ifdef BOOST_NO_STDC_NAMESPACE
    namespace std { using ::strcmp; using ::strcpy; }
# endif

//  struct_type  -------------------------------------------------------------//

struct struct_type
{
  int v1;
  bool v2;
  char v3;
  signed char v4;
  unsigned char v5;
  short v6;
  long v7;
  unsigned short v8;
  unsigned int v9;
  unsigned long v10;
//  float v11;
//  double v12;
//  long double v13;
//  char v14[32];
//  std::string v15;

  template<class Desc>
  void describe(Desc & d)
    { d & v1 & v2 & v3 & v4 & v5 & v6 & v7 & v8 & v9 & v10 /*& v11 & v12 & v13 & v15*/ ; }

  int operator-( const struct_type & ) const { return 0; }
};

inline bool operator==( const struct_type & l, const struct_type & r )
{
  return l.v1 == r.v1
  && l.v2 == r.v2
  && l.v3 == r.v3
  && l.v4 == r.v4
  && l.v5 == r.v5
  && l.v6 == r.v6
  && l.v7 == r.v7
  && l.v8 == r.v8
  && l.v9 == r.v9
  && l.v10 == r.v10
//  && l.v11 == r.v11
//  && l.v12 == r.v12
//  && l.v13 == r.v13
//  && std::strcmp( l.v14, r.v14 ) == 0
//  && l.v15 == r.v15
  ;
}
inline bool operator!=( const struct_type & l, const struct_type & r )
{ return !(l == r); }

inline std::ostream& operator<<( std::ostream& os, const struct_type & s )
{
   os << s.v1
      << ", " << s.v2
      << ", " << (int)s.v3
      << ", " << (int)s.v4
      << ", " << (int)s.v5
      << ", " << s.v6
      << ", " << s.v7
      << ", " << s.v8
      << ", " << s.v9
      << ", " << s.v10
//      << ", " << s.v11
//      << ", " << s.v12
//      << ", " << s.v13
//      << ", " << s.v14
//      << ", " << s.v15
      ;
  return os; 
}

//  test_value  --------------------------------------------------------------//
//
//  test that a value passed through a Writer and Reader is unchanged.

template< class Writer, class Reader, typename T >
void test_value( const T & old_value, int line )
{
# ifndef BOOST_NO_STRINGSTREAM
  std::stringstream ss;
# else
  std::strstream ss; // for out-of-the-box g++ 2.95.2
# endif

  Writer w(ss);
  boost::save( w, old_value );
  Reader r(ss);
  T new_value;
  boost::load( r, new_value );
  if ( old_value != new_value )
  {
    std::cout << "\nold_value: " << old_value
              << "\nnew_value: " << new_value;
    std::cout << "\ndifference: " << (old_value - new_value);
    boost::report_error( "old_value != new_value", __FILE__, line );
  }
}

//  test_types  --------------------------------------------------------------//
//
//  call test_value() for various types of interest

template< class Writer, class Reader >
void test_types()
{
  // max and min values for all built in numeric types
  test_value<Writer,Reader,int>(std::numeric_limits<int>::max(),__LINE__);
  test_value<Writer,Reader,int>(std::numeric_limits<int>::min(),__LINE__);
  test_value<Writer,Reader,int>(0x1234,__LINE__);
  test_value<Writer,Reader,bool>(std::numeric_limits<bool>::max(),__LINE__);
  test_value<Writer,Reader,bool>(std::numeric_limits<bool>::min(),__LINE__);
  test_value<Writer,Reader,char>(std::numeric_limits<char>::max(),__LINE__);
  test_value<Writer,Reader,char>(std::numeric_limits<char>::min(),__LINE__);
  test_value<Writer,Reader,signed char>(std::numeric_limits<signed char>::max(),__LINE__);
  test_value<Writer,Reader,signed char>(std::numeric_limits<signed char>::min(),__LINE__);
  test_value<Writer,Reader,unsigned char>(std::numeric_limits<unsigned char>::max(),__LINE__);
  test_value<Writer,Reader,unsigned char>(std::numeric_limits<unsigned char>::min(),__LINE__);
  test_value<Writer,Reader,short>(std::numeric_limits<short>::max(),__LINE__);
  test_value<Writer,Reader,short>(std::numeric_limits<short>::min(),__LINE__);
  test_value<Writer,Reader,short>(0x1234,__LINE__);
  test_value<Writer,Reader,long>(std::numeric_limits<long>::max(),__LINE__);
  test_value<Writer,Reader,long>(std::numeric_limits<long>::min(),__LINE__);
  test_value<Writer,Reader,long>(0x12345678,__LINE__);
  test_value<Writer,Reader,unsigned short>(std::numeric_limits<unsigned short>::max(),__LINE__);
  test_value<Writer,Reader,unsigned short>(std::numeric_limits<unsigned short>::min(),__LINE__);
  test_value<Writer,Reader,unsigned short>(0x1234,__LINE__);
  test_value<Writer,Reader,unsigned int>(std::numeric_limits<unsigned int>::max(),__LINE__);
  test_value<Writer,Reader,unsigned int>(0x1234,__LINE__);
  test_value<Writer,Reader,unsigned int>(std::numeric_limits<unsigned int>::min(),__LINE__);
  test_value<Writer,Reader,unsigned long>(std::numeric_limits<unsigned long>::max(),__LINE__);
  test_value<Writer,Reader,unsigned long>(std::numeric_limits<unsigned long>::min(),__LINE__);
  test_value<Writer,Reader,unsigned long>(0x12345678,__LINE__);

// fp tests fail for text writer/readers, but I'm leaving them as is until Jens
// figures out exactly what testable guarantees can be made for fp.
// They do pass for binary writer/readers.
  test_value<Writer,Reader,float>(std::numeric_limits<float>::max(),__LINE__);
  test_value<Writer,Reader,float>(std::numeric_limits<float>::min(),__LINE__);
  test_value<Writer,Reader,double>(std::numeric_limits<double>::max(),__LINE__);
  test_value<Writer,Reader,double>(std::numeric_limits<double>::min(),__LINE__);
  test_value<Writer,Reader,long double>(std::numeric_limits<long double>::max(),__LINE__);
  test_value<Writer,Reader,long double>(std::numeric_limits<long double>::min(),__LINE__);

// wchar_t is a problem (as of 15 apr 01) for many compilers: Metrowerks and Cygwin fail
// to compile.  Borland compiles, but fails the run test.  I give up!
//  test_value<Writer,Reader,wchar_t>(std::numeric_limits<wchar_t>::max(),__LINE__);
//  test_value<Writer,Reader,wchar_t>(std::numeric_limits<wchar_t>::min(),__LINE__);

// C and C++ string values

//  test_value<Writer,Reader,char*>("C char *",__LINE__);
//  test_value<Writer,Reader,char*>("C char * with \" embedded quote",__LINE__);
//  test_value<Writer,Reader,std::string>("C++ string",__LINE__);
//  test_value<Writer,Reader,std::string>("C++ string with \" embedded quote",__LINE__);

// structure value
  struct_type st;
  st.v1 = std::numeric_limits<int>::min();
  st.v2 = std::numeric_limits<bool>::max();
  st.v3 = std::numeric_limits<char>::max();
  st.v4 = std::numeric_limits<signed char>::min();
  st.v5 = std::numeric_limits<unsigned char>::max();
  st.v6 = std::numeric_limits<short>::min();
  st.v7 = std::numeric_limits<long>::min();
  st.v8 = std::numeric_limits<unsigned short>::max();
  st.v9 = std::numeric_limits<unsigned int>::max();
  st.v10 = std::numeric_limits<unsigned long>::max();
//   st.v11 = std::numeric_limits<float>::max();
//   st.v12 = std::numeric_limits<double>::min();
//   st.v13 = std::numeric_limits<long double>::max();
//   st.v15 = "C++ string with \" embedded quote";

  test_value<Writer,Reader,struct_type>(st,__LINE__);

}

//  test_main  ---------------------------------------------------------------//
//
//  call test_types for various writer/readers

int test_main( int, char** )
{
  std::cout << "Testing text writer/reader\n";
  test_types< boost::text_writer, boost::text_reader >();
  std::cout << "Testing binary writer/reader\n";
  test_types< boost::binary_ostream_writer, boost::binary_istream_reader >();
//  std::cout << "Testing XDR writer/reader\n";
//  test_types< boost::xdr_writer, boost::xdr_reader >();

  /// TODO: add HTML tests.
  /// TODO: add Comma Separated Value tests.
  return 0;
}

