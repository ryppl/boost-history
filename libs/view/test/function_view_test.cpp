
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
#include <utility>

#include <boost/function.hpp>

#include "boost/view/function_view.hpp"

#include "intpair.h"


struct generate_linear: public std::unary_function<int,int>
{
  int operator()( int x ) const
    { return 2*x; }
};

void linear_test()
{
  typedef boost::view::function_view< generate_linear > linear_view;
  linear_view v( 0, 10 );

  BOOST_TEST( v.size() == 10 );

  linear_view::const_iterator it = v.begin();
  generate_linear g;
  
  for( int i = 0; it != v.end(); ++i, ++it )
  {
    BOOST_TEST( *it  == g(i) );
    BOOST_TEST( v[i] == g(i) );
  }
}


int generate_square( int x )
  { return x*x; }


void square_test()
{
  typedef boost::view::function_view< boost::function<int,int> > square_view;
  square_view v( -5, 5, std::ptr_fun(generate_square) );

  BOOST_TEST( v.size() == 10 );

  square_view::const_iterator it = v.begin();
  
  for( int i = -5; it != v.end(); ++i, ++it )
  {
    BOOST_TEST( *it  == generate_square(i) );
    BOOST_TEST( v[i] == generate_square(i) );
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

  BOOST_TEST( ao.size() == 5 ); 

  int i = 0;
  char c = 'A';
  alpha_order_type::iterator it = ao.begin();

  for( ; c < 'F'; ++c, ++i, ++it )
  {
    BOOST_TEST( *it == i );
    BOOST_TEST( ao[c] == i );
  }
}


void two_dim_test()
{
  typedef boost::counting_iterator_generator< intpair >::type count_iter;

  intpair zero( std::make_pair(0,0), 3 );

  count_iter cit( zero );
  
  ++cit; BOOST_TEST( *cit == intpair( std::make_pair(1,0), 3 ) );
  ++cit; BOOST_TEST( *cit == intpair( std::make_pair(2,0), 3 ) );
  ++cit; BOOST_TEST( *cit == intpair( std::make_pair(0,1), 3 ) );
  ++cit; BOOST_TEST( *cit == intpair( std::make_pair(1,1), 3 ) );
  ++cit; BOOST_TEST( *cit == intpair( std::make_pair(2,1), 3 ) );

  typedef boost::view::function_view<norm> two_dim_type;
  
  two_dim_type td( zero, intpair( std::make_pair(2,1), 3 ), norm() );

  BOOST_TEST( td.size() == intpair( std::make_pair(2,1), 3 ) );

  BOOST_TEST( td[ zero ] == 0 );
  BOOST_TEST( td[ intpair(1,0,3) ] == 1 );
  BOOST_TEST( td[ intpair(2,0,3) ] == 2 );
  
  BOOST_TEST( td[ intpair(0,1,3) ] == 3 );
  BOOST_TEST( td[ intpair(1,1,3) ] == 4 );
  BOOST_TEST( td[ intpair(2,1,3) ] == 5 );
}


int test_main(int, char *[])
{
  linear_test();
  square_test();
  alpha_order_test();
  two_dim_test();

  bool error_on_purpose = false;
  //BOOST_TEST( error_on_purpose );

  return 0;
}

