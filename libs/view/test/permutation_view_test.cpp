
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

#include <functional>
#include <cmath>

#include <vector>
#include <deque>

#include <string>
#include <algorithm>

#include <boost/function.hpp>

#include <boost/view/function_view.hpp>
#include <boost/view/permutation_view.hpp>

#include "intpair.h"


void reverse_test()
{
  const int N = 7;
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 }; // the contents
  std::vector<int> v( numbers, numbers + N );

  std::deque<int> r; // the reindexing scheme
  for( int i = 0; i < v.size(); ++i )
    { r.push_front( i ); }

  typedef boost::view::permutation_view< 
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
     boost::view::ownership::external< std::vector<int> >, 
#else
     std::vector<int>,
#endif
     std::deque<int> > reverse_view;

  reverse_view rv( v, r );
  
  BOOST_TEST( rv.size() == N );

  std::vector<int>::reverse_iterator rit = v.rbegin();
  reverse_view::iterator it = rv.begin();

  for( ; it != rv.end(); ++it, ++rit )
  {
    BOOST_TEST( *it == *rit );
  }

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  rv[0] = 22;
  BOOST_TEST( v[N-1] == 22 );
#endif
}


struct do_reverse: public std::unary_function<int,int>
{
  do_reverse( int theK ): k( theK ) {}
  int operator()( int i ) const { return k-1-i; }

private:
  int k;
};


void reverse_function_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 }; const int N = 7;
  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::function_view<do_reverse> reindex_type;
  typedef boost::view::permutation_view< std::vector<int>, reindex_type > reverse_view;

  reverse_view rv( v, reindex_type( 0, N, do_reverse(N) ) );

  BOOST_TEST( rv.size() == N );
  
  std::vector<int>::const_iterator vit = v.begin();
  reverse_view::iterator it = rv.begin();

  // Since the reverse_view rv contains its own copy of the original v,
  // we might modify v down here.
  std::reverse( v.begin(), v.end() );

  for( ; it != rv.end(); ++it, ++vit )
  {
    BOOST_TEST( *it == *vit );
  }
}


void resample_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 }; const int N = 7;
  std::vector<int> v( numbers, numbers + N );

  std::deque<int> r; // the re-indexing scheme
  for( int i = 0; i < v.size(); ++i )
    { r.push_back( (2*i) % N ); }

  typedef boost::view::permutation_view< std::vector<int>, std::deque<int> > resample_view;
  resample_view rv( v, r );

  BOOST_TEST( v.size() == rv.size() );

  BOOST_TEST( rv[0] == v[0] );
  BOOST_TEST( rv[1] == v[2] );
  BOOST_TEST( rv[2] == v[4] );
  BOOST_TEST( rv[3] == v[6] );
  BOOST_TEST( rv[4] == v[1] );
  BOOST_TEST( rv[5] == v[3] );
  BOOST_TEST( rv[6] == v[5] );
}


struct alpha_order: public std::unary_function<char,int>
{
  alpha_order( char theFirst ): first( theFirst ) {}

  int operator()( char c ) const
    { return (int)(c-first); }

private:
  char first;
};


void alpha_order_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 }; const int N = 7;
  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::function_view<alpha_order> alpha_order_type;
  typedef boost::view::permutation_view< std::vector<int>,  alpha_order_type > alpha_order_view;

  alpha_order_view aov( v, alpha_order_type( 'A', 'F', alpha_order('A') ) );

  BOOST_TEST( aov.size() == 5 );

  BOOST_TEST( aov.front() == v[0] );
  BOOST_TEST( aov.back()  == v[4] );
  
  int c = 'A'; int i = 0;
  for( ; c < 'F'; ++c, ++i )
  {
    BOOST_TEST( aov[c] == v[i] );
  }
}


void two_dim_test()
{
  int numbers[] = { 0, -1, 4, -3, 5, 8, -2 }; const int N = 7;
  std::vector<int> v( numbers, numbers + N );

  typedef boost::view::function_view<norm> two_dim_type;
  typedef boost::view::permutation_view< std::vector<int>,  two_dim_type > two_dim_view;

  intpair zero( std::make_pair(0,0), 3 );
  intpair e( std::make_pair(2,1), 3 );

  two_dim_view tdv( v, two_dim_type( zero, e, norm() ) );

  typedef boost::counting_iterator_generator< intpair >::type count_iter;
  count_iter cit( zero );

  BOOST_TEST( tdv.size() == intpair( std::make_pair(2,1), 3 ) );

  for( std::vector<int>::iterator it = v.begin(); *cit != e; ++it, ++cit )
  {
    BOOST_TEST( tdv[ *cit ] == *it );
  }
}



int test_main(int, char *[])
{
  reverse_test();
  reverse_function_test();
  resample_test();
  alpha_order_test();
  two_dim_test();

  bool error_on_purpose = false;
  //BOOST_TEST( error_on_purpose );

  return 0;
}

