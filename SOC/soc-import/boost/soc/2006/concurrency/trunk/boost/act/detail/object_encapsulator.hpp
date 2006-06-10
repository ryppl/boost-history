#ifndef BOOST_ACT_DETAIL_OBJECT_ENCAPSULATOR_HPP
#define BOOST_ACT_DETAIL_OBJECT_ENCAPSULATOR_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/mpl/if.hpp>

namespace boost
{
namespace act
{
namespace detail
{

struct dummy_stored_object_type
{//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)


};

struct call_nullary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType& target )
  {
    return function();
  }
};

struct call_unary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType& target )
  {
    return function( target );
  }
};

struct call_void_nullary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType& target )
  {
    function();
  }
};

struct call_void_unary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType& target )
  {
    function( target );
  }

  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function
                           , dummy_stored_object_type target
                           )
  {
    function();
  }
};

// ToDo: Handle empty types via inheritance
template< typename StoredObjectType, typename Enabler = void >
class object_encapsulator
{
public:
  // ToDo: Optimize function return
  typedef StoredObjectType inactive_value_type;
public:
  object_encapsulator()
  {
  }

  template< typename FunctionPackageType >
  object_encapsulator( FunctionPackageType const& function_package )
    : stored_object( function_package.store_function()() )
  {
  }

  template< typename ObjectType, typename FunctionPackageType >
  object_encapsulator( ObjectType& target
                     , FunctionPackageType const& function_package
                     )
    : stored_object
      (
        ::boost::mpl::if_< ::boost::is_void
                           < typename ObjectType::inactive_value_type >
                         , call_nullary_function_package_function
                         , call_unary_function_package_function
                         >
                         ::type
                         ::template execute< inactive_value_type >
                         ( function_package.store_function()
                         , target.stored_object
                         )
      )
  {
  }
public:
  inactive_value_type inactive_value() const
  {
    return stored_object;
  }
public:
  mutable StoredObjectType stored_object;
};

template< typename StoredObjectType >
class object_encapsulator< StoredObjectType
                         , typename ::boost::enable_if
                             < ::boost::is_void< StoredObjectType > >::type
                         >
{
public:
  typedef StoredObjectType inactive_value_type;
public:
  object_encapsulator()
  {
  }

  template< typename FunctionPackageType >
  object_encapsulator( FunctionPackageType const& function_package )
  {
    function_package.store_function()();
  }

  // ToDo: Account for non-void function
  template< typename ObjectType, typename FunctionPackageType >
  object_encapsulator( ObjectType& target
                     , FunctionPackageType const& function_package
                     )
  {
    ::boost::mpl::if_< ::boost::is_void
                       < typename ObjectType::inactive_value_type >
                     , call_nullary_function_package_function
                     , call_unary_function_package_function
                     >
                     ::type
                     ::template execute< inactive_value_type >
                     ( function_package.store_function()
                     , target.stored_object
                     );
  }
public:
  inactive_value_type inactive_value() const
  {
  }

  static dummy_stored_object_type const stored_object;
};

template< typename StoredObjectType >
dummy_stored_object_type const
object_encapsulator< StoredObjectType
                   , typename ::boost::enable_if
                       < ::boost::is_void< StoredObjectType > >::type
                   >
                   ::stored_object = dummy_stored_object_type();

}
}
}

#endif
