
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <functional>
#include <cmath>

#include <vector>
#include <deque>

#include <string>
#include <algorithm>

#include <boost/function.hpp>

#include <boost/view/function_view.hpp>
#include <boost/view/permutation_view.hpp>


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
  
  BOOST_CHECK( rv.size() == N );

  std::vector<int>::reverse_iterator rit = v.rbegin();
  reverse_view::iterator it = rv.begin();

  for( ; it != rv.end(); ++it, ++rit )
  {
    BOOST_CHECK( *it == *rit );
  }

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  rv[0] = 22;
  BOOST_CHECK( v[N-1] == 22 );
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

  BOOST_CHECK( rv.size() == N );
  
  std::vector<int>::const_iterator vit = v.begin();
  reverse_view::iterator it = rv.begin();

  // Since the reverse_view rv contains its own copy of the original v,
  // we might modify v down here.
  std::reverse( v.begin(), v.end() );

  for( ; it != rv.end(); ++it, ++vit )
  {
    BOOST_CHECK( *it == *vit );
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

  BOOST_CHECK( v.size() == rv.size() );

  BOOST_CHECK( rv[0] == v[0] );
  BOOST_CHECK( rv[1] == v[2] );
  BOOST_CHECK( rv[2] == v[4] );
  BOOST_CHECK( rv[3] == v[6] );
  BOOST_CHECK( rv[4] == v[1] );
  BOOST_CHECK( rv[5] == v[3] );
  BOOST_CHECK( rv[6] == v[5] );
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
  typedef boost::view::permutation_view< std::vector<int>, alpha_order_type > alpha_order_view;

  alpha_order_view aov( v, alpha_order_type( 'A', 'F', alpha_order('A') ) );

  BOOST_CHECK( aov.size() == 5 );

  BOOST_CHECK( aov.front() == v[0] );
  BOOST_CHECK( aov.back()  == v[4] );
  
  int c = 'A'; int i = 0;
  for( ; c < 'F'; ++c, ++i )
  {
    BOOST_CHECK( aov[c] == v[i] );
  }
}


int test_main(int, char *[])
{
  reverse_test();
  reverse_function_test();
  resample_test();
  alpha_order_test();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}

