
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

#include <boost/view/window_view.hpp>



void test1()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::window_view< std::vector<int> > w_view;

  w_view wv( v, v.begin(), 3 );

  for( int i = 0; i < N+1; ++i )
  {
    BOOST_TEST( wv.front() == v[ i%N ] );
    BOOST_TEST( wv.back() == v[ (i+2)%N ] );

    BOOST_TEST( wv[0] == v[ i%N ] );
    BOOST_TEST( wv[1] == v[ (i+1)%N ] );
    BOOST_TEST( wv[2] == v[ (i+2)%N ] );

    wv.rotate();
  }

  for( int i = 0; i < N+1; ++i )
  {
    wv.rotate(-1);

    BOOST_TEST( wv.front() == v[ (N-i)%N ] );
    BOOST_TEST( wv.back() == v[ (N-i+2)%N ] );

    BOOST_TEST( wv[0] == v[ (N-i)%N ] );
    BOOST_TEST( wv[1] == v[ (N-i+1)%N ] );
    BOOST_TEST( wv[2] == v[ (N-i+2)%N ] );
    
  }
}


void test2()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::window_view< std::vector<int> > w_view;

  w_view wva( v, v.begin()+4, v.begin()+9 );
  BOOST_TEST( wva.size() == 5 );
  BOOST_TEST( wva.front() == v[4] );
  BOOST_TEST( wva.back() == v[(9-1)%N] );

  w_view wvb( v, v.begin()+4, 11 );
  BOOST_TEST( wvb.size() == 11 );
  BOOST_TEST( wvb.front() == v[4] );
  BOOST_TEST( wvb.back() == v[(4+11-1)%N] );

  w_view wvc( v, v.begin()+4, v.begin()+2 );
  BOOST_TEST( wvc.size() == 2 );
  BOOST_TEST( wvc.front() == v[4] );
  BOOST_TEST( wvc.back() == v[2+1] );
  BOOST_TEST( wvc[0] == v[4] );
  BOOST_TEST( wvc[1] == v[2+1] );

  w_view::iterator wit = wvc.begin();
  BOOST_TEST( *wit == v[4] ); ++wit;
  BOOST_TEST( *wit == v[3] );

  wvc.rotate();
  BOOST_TEST( wvc.front() == v[3] );
  BOOST_TEST( wvc.back() == v[1+1] );
  BOOST_TEST( wvc[0] == v[3] );
  BOOST_TEST( wvc[1] == v[1+1] );

  w_view wvd( v, v.begin()+4, -11 );
  BOOST_TEST( wvd.size() == 11 );
  BOOST_TEST( wvd.front() == v[4] );
  BOOST_TEST( wvd.back() == v[(4-11+1+N)%N] );

  wit = wvd.begin();
  for( int i = 0; i < 11; ++i, ++wit )
  {
    BOOST_TEST( *wit == v[ (4-i+N)%N ] );
    BOOST_TEST( wvd[i] == v[ (4-i+N)%N ] );
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