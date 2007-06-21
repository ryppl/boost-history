/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_ASSIGN_ASSIGN_HELPER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_ASSIGN_ASSIGN_HELPER_HPP

#include <boost/test/minimal.hpp>
#include <boost/foreach.hpp>
#include <boost/act/interlocked/integer/types.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <set>
#include <boost/type_traits/remove_cv.hpp>

#include "../random_uint8.hpp"

template< typename ContainerType, typename BarrierType, typename AssignType
        , typename DestinationType, typename SourceType
        >
class perform_assign
{
public:
  perform_assign( ContainerType& container_init, boost::mutex& mutex_init
                , BarrierType& barrier_init, AssignType assign_init
                , DestinationType& destination_init, SourceType source_init
                )
    : container_m( container_init ), mutex_m( mutex_init )
    , barrier_m( barrier_init ), assign_m( assign_init )
    , destination_m( destination_init ), source_m( source_init ) {}
public:
  void operator ()() const
  {
    barrier_m.wait(); // Wait until all other perform_assigns are ready

    typedef typename boost::remove_cv< DestinationType >::type result_type;

    result_type const result = assign_m( destination_m, source_m );

    {
      boost::mutex::scoped_lock const lock( mutex_m );

      typename ContainerType::iterator it
        = container_m.find( result );

      BOOST_CHECK( it != container_m.end() );

      container_m.erase( it );
    }
  }
private:
  ContainerType& container_m;
  boost::mutex& mutex_m;
  BarrierType& barrier_m;
  AssignType assign_m;
  DestinationType& destination_m;
  SourceType source_m;
};

template< typename ContainerType, typename BarrierType, typename AssignType
        , typename DestinationType, typename SourceType
        >
perform_assign< ContainerType, BarrierType, AssignType
              , DestinationType, SourceType
              >
make_perform_assign( ContainerType& container, boost::mutex& mutex
                   , BarrierType& barrier, AssignType assign
                   , DestinationType& destination, SourceType source
                   )
{
  return perform_assign< ContainerType, BarrierType, AssignType
                       , DestinationType, SourceType
                       >
                       ( container, mutex, barrier, assign
                       , destination, source
                       );
}

template< typename AssignType >
void brute_assign_test( AssignType assign )
{
  boost::barrier barrier( static_cast< unsigned int >( random_uint8.size() ) );

  boost::thread_group thread_group;

  boost::mutex mutex;

  typedef boost::act::interlocked::uint_least8_t uint_least8_t;

  std::multiset< uint_least8_t > old_values
    ( boost::begin( random_uint8 ), boost::end( random_uint8 ) );

  uint_least8_t test_var = 0;

  old_values.insert( test_var );

  BOOST_FOREACH( uint_least8_t val, random_uint8 )
  {
    thread_group.add_thread
      ( new boost::thread( make_perform_assign( old_values, mutex, barrier
                                              , assign, test_var, val
                                              )
                         )
      );
  }

  thread_group.join_all();
}

#endif
