
// iterator_tupple_test.cpp

// Additional tests for iterator_tuple

#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#ifdef __GNUC__
#  if __GNUC__ == 2
#    define TUPPLE_SKIP_UNEQUAL
#  endif
#endif

#include <vector>
#include <string>

#include <algorithm>
#include <numeric>
#include <functional>

#include <boost/tupple/iterator_tupple.hpp>


void traits_detail_test()
{
  using namespace boost::tupple;
  //### There is still a const problem here! *sigh*

  BOOST_CHECK( typeid( detail::iterator_traits<char*>::value_type )        == typeid( char ) );
  BOOST_CHECK( typeid( detail::iterator_traits<char*>::reference )         == typeid( char& ) );
  //BOOST_CHECK( typeid( detail::iterator_traits<char*>::const_reference )   == typeid( char& ) );
  BOOST_CHECK( typeid( detail::iterator_traits<char*>::pointer         )   == typeid( char* ) );
  BOOST_CHECK( typeid( detail::iterator_traits<char*>::iterator )          == typeid( char* ) );
  //BOOST_CHECK( typeid( detail::iterator_traits<char*>::const_iterator )    == typeid( char* ) );
  BOOST_CHECK( typeid( detail::iterator_traits<char*>::difference_type )   == typeid( std::ptrdiff_t ) );
  BOOST_CHECK( typeid( detail::iterator_traits<char*>::iterator_category ) == typeid( std::random_access_iterator_tag ) );

  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::value_type )        == typeid( std::string::value_type ) );
  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::reference )         == typeid( std::string::reference ) );
  //BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::const_reference )   == typeid( std::string::const_reference ) );
  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::pointer         )   == typeid( std::string::pointer ) );
  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::iterator )          == typeid( std::string::iterator ) );
  //BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::const_iterator )    == typeid( std::string::const_iterator ) );
  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::difference_type )   == typeid( std::string::difference_type ) );
  BOOST_CHECK( typeid( detail::iterator_traits<std::string::iterator>::iterator_category ) == typeid( std::random_access_iterator_tag ) );

}


void stl_iterator_test()
{
  using namespace boost::tupple;

  std::string str = "Yukon ho!";
  int  prime_array[10] = {2,3,5,7,11,13,17,19,23,29};
  std::vector<int> prime( prime_array, prime_array+10 );

  typedef std::string::iterator      char_iter;
  typedef std::vector<int>::iterator int_iter;

  iterator_tuple< char_iter, int_iter > t1( str.begin(), prime.begin() );
  typedef iterator_tuple< char_iter, int_iter >::difference_type diff_type;

  BOOST_CHECK( get0(*t1) == 'Y' );
  BOOST_CHECK( get1(*t1) == 2 );
  BOOST_CHECK( *(get0(t1)) == 'Y' );
  BOOST_CHECK( *(get1(t1)) == 2 );
  //BOOST_CHECK( t1->get0() == 'Y' );
  //BOOST_CHECK( t1->get1() == 2 );

  //BOOST_CHECK( (*t1).get0() == 'Y' );

  BOOST_CHECK( get0(t1[diff_type(2,3)]) == 'k' );
  BOOST_CHECK( get1(t1[diff_type(2,3)]) == 7 );

  ++t1;

  BOOST_CHECK( get0(*t1) == 'u' );
  BOOST_CHECK( get1(*t1) == 3 );

  t1 += diff_type(2,3);

  BOOST_CHECK( get0(*t1) == 'o' );
  BOOST_CHECK( get1(*t1) == 11 );

  typedef tuple<char,int> char_int_type;
  BOOST_CHECK( t1[diff_type(0,0)] == char_int_type('o',11) );
  t1[diff_type(0,0)] = tuple<char,int>('a',31);

  BOOST_CHECK( str[3] == 'a' );
  BOOST_CHECK( prime[4] == 31 );

  typedef n_fold_iterator_tuple<int_iter,3>::type triple_iter;
  typedef triple_iter::difference_type triple_diff;

//  triple_int  t  = add( 1, triple_int(0,2,3) );
  triple_iter it( prime.begin(), prime.begin(), prime.begin() );

  triple_diff t(0,2,3); ++get0(t); ++get1(t); ++get2(t);
  it += t;

  BOOST_CHECK( get0(*it) == prime[1] );
  BOOST_CHECK( get1(*it) == prime[3] );
  BOOST_CHECK( get2(*it) == prime[4] );
}



void pointer_test()
{
  using namespace boost::tupple;

  char str[10]   = "Yukon ho!";
  int  prime[10] = {2,3,5,7,11,13,17,19,23,29};

  typedef char*  char_iter;
  typedef int*   int_iter;

  iterator_tuple< char_iter, int_iter > t1( str, prime );
  typedef iterator_tuple< char_iter, int_iter >::difference_type diff_type;

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

  typedef n_fold_iterator_tuple<int*,3>::type triple_iter;
  typedef n_fold_tuple<int,3>::type triple_int;

//  triple_int  t  = add( 1, triple_int(0,2,3) );
  triple_int t(0,2,3); ++get0(t); ++get1(t); ++get2(t);
  triple_iter it( prime,prime,prime ); it += t;

  BOOST_CHECK( get0(it) == prime + 1 );
  BOOST_CHECK( get1(it) == prime + 3 );
  BOOST_CHECK( get2(it) == prime + 4 );
}


int test_main(int, char *[])
{
  traits_detail_test();

  stl_iterator_test();
  pointer_test();

  return 0;
}
