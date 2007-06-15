/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_TEST_INTERLOCKED_ASSIGN_IF_WAS_HELPER_HPP
#define BOOST_ACT_TEST_INTERLOCKED_ASSIGN_IF_WAS_HELPER_HPP

#include <boost/test/minimal.hpp>
#include <boost/act/interlocked/retrieve.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/act/interlocked/assign.hpp>
#include <boost/act/interlocked/integer.hpp>
#include <boost/type_traits/remove_cv.hpp>

// Note: Logic of how test will be performed (any better ideas?):
//
//       Start with a non-volatile variable.
//
//       Create a separate thread, in new thread, set value with
//       interlocked::assign
//       
//       In primary thread, wait several seconds to give the opposing thread
//       a decent amount of time to set the value, then yield,
//       then use retrieve.
//
//       Repeat similar tests with different threads.
//
//       Test will potentially have both false positives and false negatives,
//       though improper results should hopefully keep to a minimum,
//       particularly in a multicore environment.

template< typename GetType, typename SetType >
void test_assign_retrieve_no_threads( GetType get, SetType set )
{
  using namespace boost::act::interlocked;

  int_fast8_t test_var = 0;

  set( test_var, 5 );
  BOOST_CHECK( get( test_var ) == 5 );

  set( test_var, -20 );
  BOOST_CHECK( get( test_var ) == -20 );
  BOOST_CHECK( get( test_var ) == -20 );

  set( test_var, 112 );
  set( test_var, 36 );
  BOOST_CHECK( get( test_var ) == 36 );
  BOOST_CHECK( get( test_var ) == 36 );

  set( test_var, -14 );
  BOOST_CHECK( get( test_var ) == -14 );
}

struct single_thread_basic_set
{
  template< typename VariableType, typename SourceType >
  void operator ()( VariableType& var, SourceType new_val ) const
  {
    var = new_val;
  }
};

struct single_thread_basic_get
{
  template< typename VariableType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var ) const
  {
    return var;
  }
};

struct interlocked_assign_set
{
  template< typename VariableType, typename SourceType >
  void operator ()( VariableType& var, SourceType new_val ) const
  {
    boost::act::interlocked::assign( var, new_val );
  }
};

struct interlocked_retrieve_get
{
  template< typename VariableType >
  typename boost::remove_cv< VariableType >::type
  operator ()( VariableType& var ) const
  {
    return boost::act::interlocked::retrieve( var );
  }
};

template< typename VariableType, typename SourceType
        , typename GetType, typename SetType
        >
class assign_thread_fun
{
public:
  assign_thread_fun( VariableType& var_init, SourceType new_val_init
                   , GetType get_init, SetType set_init
                   )
    : var_m( var_init ), new_val_m( new_val_init )
    , get_m( get_init ), set_m( set_init ) {}
public:
  void operator ()() const
  {
    set_m( var_m, new_val_m );
  }
private:
  VariableType& var_m,
                new_val_m;
  GetType get_m;
  SetType set_m;
};

template< typename VariableType, typename SourceType
        , typename GetType, typename SetType
        >
assign_thread_fun< VariableType, SourceType, GetType, SetType >
make_assign_thread_fun( VariableType& var_init, SourceType new_val_init
                      , GetType get, SetType set_init
                      )
{
  return assign_thread_fun< VariableType, SourceType, GetType, SetType >
         ( var_init, new_val_init
         , get, set_init
         );
}

template< typename VariableType, typename SourceType
        , typename GetType, typename SetType
        >
void assign_in_new_thread( VariableType& var, SourceType new_val
                         , GetType get, SetType set
                         )
{
  boost::thread( make_assign_thread_fun( var, new_val, get, set ) );

  boost::xtime time_of_completion;

  boost::xtime_get( &time_of_completion, boost::TIME_UTC );

  time_of_completion.sec += 2;

  boost::thread::sleep( time_of_completion );

  boost::thread::yield();

  BOOST_CHECK( get( var ) == new_val );
}

template< typename GetType, typename SetType >
void test_assign_retrieve_with_threads( GetType get, SetType set )
{
  using namespace boost::act::interlocked;

  int_fast8_t test_var = 0;

  assign_in_new_thread( test_var, 5,   get, set );
  assign_in_new_thread( test_var, -20, get, set );
  assign_in_new_thread( test_var, 112, get, set );
  assign_in_new_thread( test_var, 36,  get, set );
  assign_in_new_thread( test_var, -14, get, set );
}

#endif
