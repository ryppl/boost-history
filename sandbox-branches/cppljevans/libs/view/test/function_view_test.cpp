
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <functional>
#include <utility>

#include <boost/function.hpp>

#include "boost/view/function_view.hpp"

#include "intpair.h"


struct linear: public std::unary_function<double,double>
{
  linear( double theK = 1., double theD = 0. )
    : k( theK ), d( theD )
  { }

  double operator()( double x ) const
    { return k*x + d; }

  double k;
  double d;
};


void linear_test()
{
  typedef boost::view::function_view< linear, int, double > linear_view;
  linear_view v( 0, 10, linear( 2., -3. ) );

  BOOST_CHECK( v.size() == 10 );

  linear_view::const_iterator it = v.begin();
  linear g( 2., -3. );

  for( int i = 0; it != v.end(); ++i, ++it )
  {
    BOOST_CHECK( *it  == g(i) );
    BOOST_CHECK( v[i] == g(i) );
  }
}


int generate_square( int x )
  { return x*x; }


void square_test()
{
  typedef boost::view::function_view< boost::function1<int,int> > square_view;
  square_view v( -5, 5, std::ptr_fun(generate_square) );

  BOOST_CHECK( v.size() == 10 );

  square_view::const_iterator it = v.begin();

  for( int i = -5; it != v.end(); ++i, ++it )
  {
    BOOST_CHECK( *it  == generate_square(i) );
    BOOST_CHECK( v[i] == generate_square(i) );
  }
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
  typedef boost::view::function_view<alpha_order> alpha_order_type;

  alpha_order_type ao( 'A', 'F', alpha_order('A') );

  BOOST_CHECK( ao.size() == 5 );

  int i = 0;
  char c = 'A';
  alpha_order_type::iterator it = ao.begin();

  for( ; c < 'F'; ++c, ++i, ++it )
  {
    BOOST_CHECK( *it == i );
    BOOST_CHECK( ao[c] == i );
  }
}


void two_dim_test()
{
  typedef boost::counting_iterator< intpair > count_iter;

  intpair zero( std::make_pair(0,0), 3 );

  count_iter cit( zero );

  ++cit; BOOST_CHECK( *cit == intpair( std::make_pair(1,0), 3 ) );
  ++cit; BOOST_CHECK( *cit == intpair( std::make_pair(2,0), 3 ) );
  ++cit; BOOST_CHECK( *cit == intpair( std::make_pair(0,1), 3 ) );
  ++cit; BOOST_CHECK( *cit == intpair( std::make_pair(1,1), 3 ) );
  ++cit; BOOST_CHECK( *cit == intpair( std::make_pair(2,1), 3 ) );

  typedef boost::view::function_view<norm> two_dim_type;

  two_dim_type td( zero, intpair( std::make_pair(2,1), 3 ), norm() );

  BOOST_CHECK( td.size() == intpair( std::make_pair(2,1), 3 ) );

  BOOST_CHECK( td[ zero ] == 0 );
  BOOST_CHECK( td[ intpair(1,0,3) ] == 1 );
  BOOST_CHECK( td[ intpair(2,0,3) ] == 2 );

  BOOST_CHECK( td[ intpair(0,1,3) ] == 3 );
  BOOST_CHECK( td[ intpair(1,1,3) ] == 4 );
  BOOST_CHECK( td[ intpair(2,1,3) ] == 5 );
}


int test_main(int, char *[])
{
  linear_test();
  square_test();
  alpha_order_test();
  two_dim_test();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}

