
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


void head_tail_test()
{
  tuple<null_type> t0;
  BOOST_CHECK( t0.size() == 0 );

  tuple<int, float> t(12, 5.5f);

  BOOST_CHECK( t.size() == 2 );
  t.head() = 4;
  BOOST_CHECK( get0(t) == 4 );

  ++t.head();
  BOOST_CHECK( get0(t) == 5 );

  ++get0(t);
  BOOST_CHECK( get0(t) == 6 );

  tuple<char,long,double> u('a',t);

  BOOST_CHECK( u.size() == 3 );
  BOOST_CHECK( get0(u) == 'a' );
  BOOST_CHECK( get1(u) == 6 );
  BOOST_CHECK( get2(u) > 5.4f && get2(u) < 5.6f );
  BOOST_CHECK( u.head() == 'a' );
  BOOST_CHECK( u.tail().head() == 6 );
  BOOST_CHECK( u.tail().tail().head() > 5.4f && u.tail().tail().head() < 5.6f );
  BOOST_CHECK( u.tail().tail().tail().size() == 0 );
}


void front_back_test()
{
  tuple<int, float, long> t(12, 5.5f, 77);

  BOOST_CHECK( t.size() == 3 );

  BOOST_CHECK( get0(t) == 12 );
  t.first() = 4;
  BOOST_CHECK( get0(t) == 4 );
  ++t.first();
  BOOST_CHECK( get0(t) == 5 );
  ++get0(t);
  BOOST_CHECK( t.first() == 6 );

  BOOST_CHECK( get2(t) == 77 );
  t.last() = 4;
  BOOST_CHECK( get2(t) == 4 );
  ++t.last();
  BOOST_CHECK( get2(t) == 5 );
  ++get2(t);
  BOOST_CHECK( t.last() == 6 );
}



void n_fold_test()
{
  tuple<int,int,int> a(1,2,3);
  n_fold_tuple<int,3>::type b(1,2,3);

  BOOST_CHECK( a == b );

  tuple<int,int,int> c(4,4,4);
  tuple<int,int,int> d = n_fold_tuple<int,3>::make(4);
  BOOST_CHECK( c == d );
}


void swap_test()
{
  tuple<char,int,std::string> a( '+', 3, "foo" ), aa( '+', 3, "foo" );
  tuple<char,int,std::string> b( '-', 7, "yo" ),  bb( '-', 7, "yo" );

  aa.swap( bb );

  BOOST_CHECK( a == bb );
  BOOST_CHECK( b == aa );

  std::swap( a,b );

  BOOST_CHECK( a == aa );
  BOOST_CHECK( b == bb );
}


double avg( int weight, int a, int b )
{ return( weight * a + ( 100 - weight ) * b ); }

void function_test()
{
  tuple<int,int,int> a(60,3,5);

  BOOST_CHECK( ptr_fct( avg )( a ) == 60 * 3 + 40 * 5 );

  // More to come
}


void min_max_test()
{
  tuple<int,int,int> t( 5, -2, 3 );

  BOOST_CHECK( minimal(t) == -2 );
  BOOST_CHECK( maximal(t) == 5 );
}

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<class TupleT, class OpT, class CondT>
TupleT do_if_first( TupleT t, OpT op, CondT cond )
{
  if( cond( t.head() ) )
    { return TupleT( op( t.head() ), t.tail() ); }
  else
    { return TupleT( t.head(), do_if_first( t.tail(), op, cond ) ); }
}


template<class OpT, class CondT>
tuple<null_type> do_if_first( tuple<null_type> t, OpT op, CondT cond )
{
  return tuple<null_type>();
}

#endif


void algo_test()
{
  tuple<int, char, double> t1(5, 'a', -3.14);
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
  tuple<int, char, double > t2 =
    do_if_first( t1, std::bind2nd( std::multiplies<double>(), -1.5 ), 
                     std::bind2nd( std::less<double>(), 0) );

  BOOST_CHECK( t2.get0() == 5 );
  BOOST_CHECK( t2.get1() == 'a' );
  BOOST_CHECK( t2.get2() == 3.14 * 1.5 );
#endif
}



int test_main(int, char *[])
{
  head_tail_test();
  front_back_test();
  n_fold_test();
  swap_test();
  function_test();

  min_max_test();
  algo_test();

  return 0;
}
