
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


#include <vector>
#include <string>
#include <iterator>

#include <boost/view/range_view.hpp>


void test1()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::range_view< 
    std::vector<int>::iterator, std::vector<int>::const_iterator
#ifdef BOOST_MSVC
    , int, int&, int* //, std::random_access_iterator_tag, long 
#endif  
    > r_view;

  r_view rv( v.begin(), v.end() );

  BOOST_TEST( v.size() == rv.size() );
  for( int i = 0; i < N; ++i )
  {
    BOOST_TEST( v[i] == rv[i] );
  }

  std::vector<int>::const_iterator it = v.begin();
  r_view::const_iterator rit = rv.begin();

  for( ; rit != rv.end(); ++it, ++rit )
  {
    BOOST_TEST( *it == *rit );
    BOOST_TEST( it == rit ); // Is this always guaranteed?
  }
}



void test2()
{
  char* ch = "A sentence with five words.";

  std::string str( ch );
  int N = str.size();

  typedef boost::view::range_view< char*, const char*, char, char&, char*,
    std::random_access_iterator_tag, long > r_view;

  r_view rv( ch, ch + N );

  BOOST_TEST( str.size() == rv.size() );
  for( int i = 0; i < N; ++i )
  {
    BOOST_TEST( str[i] == rv[i] );
  }
}


int test_main(int, char *[])
{
  test1();
  test2();

  bool error_on_purpose = false;
  //BOOST_TEST( error_on_purpose );

  return 0;
}

