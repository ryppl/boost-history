//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTION_FUNCTION_HPP
#define BOOST_ACT_COMMON_ALGO_MODELS_DETAIL_CONCURRENT_ACTION_FUNCTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

#include "concurrent_action_internals.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename FunctionPackageType >
class concurrent_action_function;

template< typename FunctionPackageType >
static
concurrent_action_function< FunctionPackageType >
make_concurrent_action_function
(
  shared_ptr
  <
    concurrent_action_internals< typename FunctionPackageType::result_type >
  >
  const& internals
, FunctionPackageType const& function
);

template< typename FunctionPackageType >
class concurrent_action_function_type_base_base
{
private:
  typedef typename FunctionPackageType::result_type result_type;

  typedef concurrent_action_internals< result_type > internals_type;

  typedef typename FunctionPackageType::stored_function_type
            stored_function_type;
public:
  concurrent_action_function_type_base_base
  (
    shared_ptr< internals_type > const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : action_internals_m( concurrent_action_internals_init )
    , function_m( function_init.store_function() )
  {
  }
protected:
  weak_ptr< internals_type > const action_internals_m;
  mutable stored_function_type function_m; // ToDo: Possibly remove mutable
};

template< typename FunctionPackageType >
class concurrent_action_function_type_nonvoid_base
  : concurrent_action_function_type_base_base< FunctionPackageType >
{
private:
  typedef concurrent_action_function_type_base_base< FunctionPackageType >
            base;

  typedef typename FunctionPackageType::result_type result_type;

  typedef concurrent_action_internals< result_type > internals_type;

  typedef shared_ptr< internals_type > shared_internals_type;
public:
  concurrent_action_function_type_nonvoid_base
  (
    shared_internals_type const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : base( concurrent_action_internals_init, function_init )
  {
  }
public:
  void operator ()() const
  {
    result_type const result = base::function_m();

    // ToDo: Move this to the base
    shared_internals_type const internals( base::action_internals_m );

    if( internals )
    {
      internals->value = result;

      internals->set_trigger();
    }
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    result_type const result = base::function_m( target );

    // ToDo: Move this to the base
    shared_internals_type const internals( base::action_internals_m );

    if( internals )
    {
      internals->value = result;

      internals->set_trigger();
    }
  }
};

template< typename FunctionPackageType >
class concurrent_action_function_type_void_base
  : concurrent_action_function_type_base_base< FunctionPackageType >
{
private:
  typedef concurrent_action_function_type_base_base< FunctionPackageType > base;

  typedef typename FunctionPackageType::result_type result_type;

  typedef concurrent_action_internals< result_type > internals_type;

  typedef shared_ptr< internals_type > shared_internals_type;
public:
  concurrent_action_function_type_void_base
  (
    shared_internals_type const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : base( concurrent_action_internals_init, function_init )
  {
  }
public:
  void operator ()() const
  {
    base::function_m();

    // ToDo: Move this to the base
    shared_internals_type const internals( base::action_internals_m );

    if( internals )
      internals->completion_trigger->set_trigger();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    base::function_m( target );

    // ToDo: Move this to the base
    shared_internals_type const internals( base::action_internals_m );

    if( internals )
      internals->set_trigger();
  }
};

template< typename FunctionPackageType >
class concurrent_action_function
{
public:
  typedef typename FunctionPackageType::result_type result_type;

  typedef concurrent_action_internals< result_type > internals_type;

  typedef shared_ptr< internals_type > shared_internals_type;
private:
  typedef typename mpl::if_
          <
            is_void< result_type >
          , concurrent_action_function_type_void_base< FunctionPackageType >
          , concurrent_action_function_type_nonvoid_base< FunctionPackageType >
          >
          ::type function_type_base;
private:
  class function_type
    : public function_type_base // ToDo: Change to private
  {
  public:
    function_type( typename concurrent_action_function::shared_internals_type
                     const& concurrent_action_internals_init
                 , FunctionPackageType const& function_init
                 )
      : function_type_base( concurrent_action_internals_init
                          , function_init
                          )
    {
    }
  public:
    using function_type_base::operator ();
  };
public:
  concurrent_action_function
  ( shared_internals_type const& concurrent_action_internals_init
  , FunctionPackageType const& function_init
  )
    : function_m( new function_type( concurrent_action_internals_init
                                   , function_init
                                   )
                )
  {
  }
public:
  void operator ()() const
  {
    (*function_m)();
  }

  template< typename TargetType >
  void operator ()( TargetType* const target ) const
  {
    (*function_m)( target );
  }
private:
  shared_ptr< function_type > function_m;
};

template< typename FunctionPackageType >
concurrent_action_function< FunctionPackageType >
make_concurrent_action_function
(
  shared_ptr
  <
    concurrent_action_internals< typename FunctionPackageType::result_type >
  >
  const& internals
, FunctionPackageType const& function
)
{
  return concurrent_action_function< FunctionPackageType >( internals
                                                          , function
                                                          );
}

}
}
}

#endif
