
// container_tupple_test.cpp

#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#ifdef __GNUC__
#  if __GNUC__ == 2
#    define TUPPLE_SKIP_UNEQUAL
#  endif
#endif

#include <boost/tupple/container_tupple.hpp>

#include <vector>
#include <string>

#include <algorithm>
#include <numeric>
#include <functional>

template<class T>
bool operator==( const ::boost::tupple::detail::reference_wrapper<T>& lhs,
                 const T& rhs )
{
  return( lhs.get() == rhs );
}


void container_test()
{
  using namespace boost::tupple;

  std::vector<int>    a;
  std::vector<char>   b(3,'-');
  std::vector<double> c(7,3.14);

  container_tuple< std::vector<int>, std::vector<char>, std::vector<double> > t( a,b,c );
  typedef container_tuple< std::vector<int>, std::vector<char>, std::vector<double> >::size_type index_type;

  BOOST_CHECK( get0( t.empty() ) == a.empty() );
  BOOST_CHECK( get1( t.empty() ) == b.empty() );
  BOOST_CHECK( get2( t.empty() ) == c.empty() );

  BOOST_CHECK( get0( t.size() ) == a.size() );
  BOOST_CHECK( get1( t.size() ) == b.size() );
  BOOST_CHECK( get2( t.size() ) == c.size() );

  // Does NOT result in a runtime error, although vector a is EMPTY!
  index_type one(1,1,1);
  BOOST_CHECK( get1( t[one] ) == '-' );
  BOOST_CHECK( get2( t[one] ) == 3.14 );

  std::vector< std::string > d(5);
  std::fill( d.begin(), d.end(), std::string( "foo" ) );

  typedef container_tuple< std::vector<char>, 
                           std::vector<double>, 
                           std::vector<std::string> > ContainerType;

  ContainerType u( b,c,d );

  BOOST_CHECK( get0( u[one] ) == '-' );
  BOOST_CHECK( get1( u[one] ) == 3.14 );
  BOOST_CHECK( get2( u[one] ) == std::string("foo") );

  get0( u[one] ) = '+';
  get1( u[one] ) = 2.78;
  get2( u[one] ) = "yo";

  BOOST_CHECK( get0( u[one] ) == '+' );
  BOOST_CHECK( get1( u[one] ) == 2.78 );
  BOOST_CHECK( get2( u[one] ) == std::string( "yo" ) );

  ContainerType::iterator it = u.begin();

  BOOST_CHECK( get0( *it ) == '-' );
  BOOST_CHECK( get1( *it ) == 3.14 );

  //###### THIS DOES NOT COMPILE UNDER MSVC:
#ifndef BOOST_MSVC
  BOOST_CHECK( (*it).get2() == std::string( "foo" ) );
  BOOST_CHECK( get2( *it ) == std::string( "foo" ) );
#endif

  ++it;

  BOOST_CHECK( get0( *it ) == '+' );
  BOOST_CHECK( get1( *it ) == 2.78 );
#ifndef BOOST_MSVC
  BOOST_CHECK( get2( *it ) == std::string( "yo" ) );
#endif

  *it = tuple< char, double, std::string >( '*', 1.33, std::string("gasp") );

  BOOST_CHECK( get0( u[one] ) == '*' );
  BOOST_CHECK( get1( u[one] ) == 1.33 );
  BOOST_CHECK( get2( u[one] ) == std::string( "gasp" ) );
}




#if 0
void zip_test()
{
  char strA[] = "Just a few words on views.";
  char strB[] = "Just a views word on few. ";

  vector<char> a( strA, strA + 27 );
  vector<char> b( strB, strB + 27 );

  typedef zip_view< vector<char>, vector<char>, std::equal_to<char> > compare_view;
  //typedef zip_view< vector<char>&, vector<char>&, std::equal_to<char> > compare_view;

  compare_view v( a, b );

  BOOST_CHECK( v[0] == std::equal_to<char>()( a[0], b[0] ) );
  BOOST_CHECK( v[1] == std::equal_to<char>()( a[1], b[1] ) );
  BOOST_CHECK( v[7] == std::equal_to<char>()( a[7], b[7] ) );
  // etc.


  int m = 0;

  for( int i = 0; i < a.size(); ++i ) // How to do it in an STL-like way?
    if( a[i] != b[i] ) ++m;

  int n = std::count( v.begin(), v.end(), false );

  BOOST_CHECK( m == n );
}
#endif


int test_main(int, char *[])
{
  container_test();

  return 0;
}

