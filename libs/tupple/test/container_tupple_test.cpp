
// tupple_test.cpp

// Additional tests that are not covered in tupple_test_bench.cpp

#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#ifdef __GNUC__
#  if __GNUC__ == 2
#    define TUPPLE_SKIP_UNEQUAL
#  endif
#endif

#include <boost/tupple/tupple.hpp>

using namespace boost::tupple;

#include <vector>

#include <string>

#include <algorithm>
#include <numeric>
#include <functional>




struct convolve
{
  typedef int result_type;

  int operator()( int a, int b, int c ) const
  {
    return( -a + b - c );
  }
};


void iterator_test()
{
  char str[10]   = "Yukon ho!";
  int  prime[10] = {2,3,5,7,11,13,17,19,23,29};

  iterator_tuple<char*,int*> t1( str, prime );
  typedef iterator_tuple<char*,int*>::difference_type diff_type;

  BOOST_CHECK( get0(*t1) == 'Y' );
  BOOST_CHECK( get1(*t1) == 2 );
  BOOST_CHECK( *(get0(t1)) == 'Y' );
  BOOST_CHECK( *(get1(t1)) == 2 );

  BOOST_CHECK( get0(t1[diff_type(2,3)]) == 'k' );
  BOOST_CHECK( get1(t1[diff_type(2,3)]) == 7 );

  ++t1;

  BOOST_CHECK( get0(t1) == str+1 );
  BOOST_CHECK( get1(t1) == prime+1 );

  BOOST_CHECK( get0(*t1) == 'u' );
  BOOST_CHECK( get1(*t1) == 3 );

  t1 += diff_type(2,3);

  BOOST_CHECK( get0(t1) == str+3 );
  BOOST_CHECK( get1(t1) == prime+4 );

  BOOST_CHECK( get0(*t1) == 'o' );
  BOOST_CHECK( get1(*t1) == 11 );


  t1[diff_type(0,0)] = tuple<char,int>('a',31);

  BOOST_CHECK( str[3] == 'a' );
  BOOST_CHECK( prime[4] == 31 );

  typedef n_fold_iterator<int*,3>::type triple_iter;
  typedef n_fold_tuple<int,3>::type triple_int;

//  triple_int  t  = add( 1, triple_int(0,2,3) );
  triple_int t(0,2,3); ++get0(t); ++get1(t); ++get2(t);
  triple_iter it( prime,prime,prime ); it += t;

  BOOST_CHECK( get0(it) == prime + 1 );
  BOOST_CHECK( get1(it) == prime + 3 );
  BOOST_CHECK( get2(it) == prime + 4 );
}


void container_test()
{
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

  std::vector< std::string > d( 5, "foo" );

  container_tuple< std::vector<char>, std::vector<double>, std::vector< std::string > > u( b,c,d );

  BOOST_CHECK( get0( u[one] ) == '-' );
  BOOST_CHECK( get1( u[one] ) == 3.14 );
  BOOST_CHECK( get2( u[one] ) == "foo" );

  get0( u[one] ) = '+';
  get1( u[one] ) = 2.78;
  get2( u[one] ) = "yo";

  BOOST_CHECK( get0( u[one] ) == '+' );
  BOOST_CHECK( get1( u[one] ) == 2.78 );
  BOOST_CHECK( get2( u[one] ) == "yo" );

  iterator_tuple< std::vector<char>::iterator,
                  std::vector<double>::iterator,
                  std::vector< std::string >::iterator > it = u.begin();

  BOOST_CHECK( get0( *it ) == '-' );
  BOOST_CHECK( get1( *it ) == 3.14 );
  BOOST_CHECK( get2( *it ) == "foo" );

  ++it;

  BOOST_CHECK( get0( *it ) == '+' );
  BOOST_CHECK( get1( *it ) == 2.78 );
  BOOST_CHECK( get2( *it ) == "yo" );

  *it = tuple< char, double, std::string >( '*', 1.33, "gasp" );

  BOOST_CHECK( get0( u[one] ) == '*' );
  BOOST_CHECK( get1( u[one] ) == 1.33 );
  BOOST_CHECK( get2( u[one] ) == "gasp" );

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

  iterator_test();
  container_test();


  return 0;
}

