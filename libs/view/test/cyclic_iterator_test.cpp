
#include <boost/config.hpp>
#include <boost/test/minimal.hpp>

#include <vector>

#include <boost/view/cyclic_iterator.hpp>


void cyclic_iterator_test()
{
  int numbers[] = { 1,2,3,4,5,6,7 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

#ifdef BOOST_MSVC  
  typedef boost::cyclic_iterator< std::vector<int>::iterator
                                , boost::use_default 
                                , boost::use_default
                                , std::vector<int>::reference >
          cyclic_int_iter;

  cyclic_int_iter begin( v.begin(), v.end() );
#else
  typedef boost::cyclic_iterator< std::vector<int>::iterator >
          cyclic_int_iter;

  cyclic_int_iter begin = boost::make_cyclic_iterator( v );
#endif

  cyclic_int_iter forward( begin );
  cyclic_int_iter backward( begin );

  int i = 0; int j = 0;

  for( ; i < 2*N; ++i, --j )
  {
    BOOST_CHECK( std::distance( begin, forward ) == i );
    BOOST_CHECK( forward.pastTheEnd == (i/N) );
    BOOST_CHECK( *forward == v[i%N] );

    BOOST_CHECK( std::distance( begin, backward ) == j );
    BOOST_CHECK( backward.pastTheEnd == ( (j-N+1)/N ) );
    BOOST_CHECK( *backward == v[ (j+2*N)%N ] );

    ++forward;
    --backward;
  }

  forward = begin;
  backward = begin;

  for( i = 0, j = 0; i < 4*N; i += 2, j -= 2 )
  {
    BOOST_CHECK( std::distance( begin, forward ) == i );
    BOOST_CHECK( forward.pastTheEnd == (i/N) );
    BOOST_CHECK( *forward == v[i%N] );

    BOOST_CHECK( std::distance( begin, backward ) == j );
    BOOST_CHECK( backward.pastTheEnd == ( (j-N+1)/N ) );
    BOOST_CHECK( *backward == v[ (j+4*N)%N ] );

    std::advance( forward, 2 );
    std::advance( backward, -2 );
  }
}


void reverse_cyclic_iterator_test()
{
  int numbers[] = { 1,2,3,4,5,6,7 };
  const int N = sizeof(numbers)/sizeof(int);

  std::vector<int> v( numbers, numbers + N );

#if BOOST_MSVC  
  typedef boost::cyclic_iterator< std::vector<int>::iterator
                                , int, boost::use_default, int& >
          cyclic_int_iter;

  cyclic_int_iter rbegin( v.begin(), v.end(), true );
#else
  typedef boost::cyclic_iterator< std::vector<int>::iterator >
          cyclic_int_iter;

  cyclic_int_iter rbegin = boost::make_cyclic_iterator( v, true );
#endif

  cyclic_int_iter rforward( rbegin );
  cyclic_int_iter rbackward( rbegin );

  int i = 0; int j = 0;

  for( ; i < 2*N; ++i, --j )
  {
    BOOST_CHECK( std::distance( rbegin, rforward ) == i );
    BOOST_CHECK( rforward.pastTheEnd == ( (-i-N+1)/N ) );
    BOOST_CHECK( *rforward == v[(-i+2*N)%N] );

    BOOST_CHECK( std::distance( rbegin, rbackward ) == j );
    BOOST_CHECK( rbackward.pastTheEnd == ( -j/N ) );
    BOOST_CHECK( *rbackward == v[ -j%N ] );

    ++rforward;
    --rbackward;
  }

  rforward = rbegin;
  rbackward = rbegin;

  for( i = 0, j = 0; i < 4*N; i += 2, j -= 2 )
  {
    BOOST_CHECK( std::distance( rbegin, rforward ) == i );
    BOOST_CHECK( rforward.pastTheEnd == ((-i-N+1)/N) );
    BOOST_CHECK( *rforward == v[(-i+4*N)%N] );

    BOOST_CHECK( std::distance( rbegin, rbackward ) == j );
    BOOST_CHECK( rbackward.pastTheEnd == ( -j/N ) );
    BOOST_CHECK( *rbackward == v[ -j%N ] );

    std::advance( rforward, 2 );
    std::advance( rbackward, -2 );
  }
}


int test_main(int, char *[])
{
  cyclic_iterator_test();
  reverse_cyclic_iterator_test();

  bool error_on_purpose = false;
  //BOOST_CHECK( error_on_purpose );

  return 0;
}
