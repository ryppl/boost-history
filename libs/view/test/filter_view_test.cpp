
#include <boost/config.hpp>

/* With my current MS VC++ 7.0 config, Boost.Test fails to compile,
   and I do not have the time to nail it down. Thus, VC++ tests are done
   with Boost 1.28.0.  - RR
*/

#ifdef BOOST_MSVC
#  define BOOST_INCLUDE_MAIN
#  include <boost/test/test_tools.hpp>
#else
#  include <boost/test/included/test_exec_monitor.hpp>
#endif

#include <iostream>

#include <vector>
#include <algorithm>

#include <boost/view/filter_view.hpp>


// Any 'const' inside will prevent this from compiling *hmm*
template<class A, class B>
bool is_equal( A& a, B& b )
{
  if( a.size() != b.size() ) return false;

  typename A::iterator ai = a.begin();
  typename B::iterator bi = b.begin();

  for( ; ai != a.end(); ++ai, ++bi )
  { if( *(ai) != *(bi) ) return false; }

  return true;
}


struct is_positive_number {
  bool operator()(int x) { return x > 0; }
};

struct is_negative_number {
  bool operator()(int x) { return x < 0; }
};

struct is_big_number {
  bool operator()(int x) { return x >= 10; }
};


void test1()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = 7;

  std::vector<int> v( numbers, numbers + N );

  boost::view::filter_view< std::vector<int>, is_big_number > bigview( v );

  BOOST_TEST( bigview.empty() );
  BOOST_TEST( bigview.size() == 0 );

  typedef boost::view::filter_view< std::vector<int>, is_positive_number > pos_view_type;
  pos_view_type posview( v );

  BOOST_TEST( posview.size() == 3 );

  BOOST_TEST( posview.front() == 4 );

  pos_view_type::iterator it = posview.begin();
  BOOST_TEST( *it == 4 ); ++it;
  BOOST_TEST( *it == 5 ); ++it;
  BOOST_TEST( *it == 8 );

  std::vector<int> positives( posview.size() );

  std::copy( posview.begin(), posview.end(), positives.begin() );
  BOOST_TEST( is_equal( positives, posview ) );
}


void test2()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = 7;

  std::vector<int> a( numbers, numbers + N );
  std::vector<int> b( numbers, numbers + N );

  std::replace_if( a.begin(), a.end(), is_negative_number(), 0 );

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  boost::view::filter_view< boost::view::ownership::external< std::vector<int> >,
  													 is_negative_number > v( b );
  std::fill( v.begin(), v.end(), 0 );

  BOOST_TEST( is_equal( a, b ) );
#endif
}


int test_main(int, char *[])
{
  test1();
  test2();

  bool error_on_purpose = false;
  //BOOST_TEST( error_on_purpose );

  return 0;
}

