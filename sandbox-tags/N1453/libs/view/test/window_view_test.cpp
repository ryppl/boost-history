
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

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
    BOOST_CHECK( wv.front() == v[ i%N ] );
    BOOST_CHECK( wv.back() == v[ (i+2)%N ] );

    BOOST_CHECK( wv[0] == v[ i%N ] );
    BOOST_CHECK( wv[1] == v[ (i+1)%N ] );
    BOOST_CHECK( wv[2] == v[ (i+2)%N ] );

    wv.rotate();
  }

  for( int i = 0; i < N+1; ++i )
  {
    wv.rotate(-1);

    BOOST_CHECK( wv.front() == v[ (N-i)%N ] );
    BOOST_CHECK( wv.back() == v[ (N-i+2)%N ] );

    BOOST_CHECK( wv[0] == v[ (N-i)%N ] );
    BOOST_CHECK( wv[1] == v[ (N-i+1)%N ] );
    BOOST_CHECK( wv[2] == v[ (N-i+2)%N ] );
    
  }
}


void test2()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::window_view< std::vector<int> > w_view;

  w_view wva( v, v.begin()+4, v.begin()+9 );
  BOOST_CHECK( wva.size() == 5 );
  BOOST_CHECK( wva.front() == v[4] );
  BOOST_CHECK( wva.back() == v[(9-1)%N] );

  w_view wvb( v, v.begin()+4, 11 );
  BOOST_CHECK( wvb.size() == 11 );
  BOOST_CHECK( wvb.front() == v[4] );
  BOOST_CHECK( wvb.back() == v[(4+11-1)%N] );

  w_view wvc( v, v.begin()+4, v.begin()+2 );
  BOOST_CHECK( wvc.size() == 2 );
  BOOST_CHECK( wvc.front() == v[4] );
  BOOST_CHECK( wvc.back() == v[2+1] );
  BOOST_CHECK( wvc[0] == v[4] );
  BOOST_CHECK( wvc[1] == v[2+1] );

  w_view::iterator wit = wvc.begin();
  BOOST_CHECK( *wit == v[4] ); ++wit;
  BOOST_CHECK( *wit == v[3] );

  wvc.rotate();
  BOOST_CHECK( wvc.front() == v[3] );
  BOOST_CHECK( wvc.back() == v[1+1] );
  BOOST_CHECK( wvc[0] == v[3] );
  BOOST_CHECK( wvc[1] == v[1+1] );

  w_view wvd( v, v.begin()+4, -11 );
  BOOST_CHECK( wvd.size() == 11 );
  BOOST_CHECK( wvd.front() == v[4] );
  BOOST_CHECK( wvd.back() == v[(4-11+1+N)%N] );

  wit = wvd.begin();
  for( int i = 0; i < 11; ++i, ++wit )
  {
    BOOST_CHECK( *wit == v[ (4-i+N)%N ] );
    BOOST_CHECK( wvd[i] == v[ (4-i+N)%N ] );
  }

}


int test_main(int, char *[])
{
  test1();
  test2();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}