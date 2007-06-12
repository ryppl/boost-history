//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_DETAIL_OBJECT_ENCAPSULATOR_HPP
#define BOOST_ACT_DETAIL_OBJECT_ENCAPSULATOR_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/mpl/if.hpp>

#include <new>

namespace boost
{
namespace act
{
namespace detail
{

struct dummy_stored_object_type
{
};

struct call_nullary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType* const target )
  {
    return function();
  }
};

struct call_unary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType* const target )
  {
    return function( target );
  }
};

struct call_void_nullary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType* const target )
  {
    function();
  }
};

struct call_void_unary_function_package_function
{
  template< typename ReturnType, typename FunctionType, typename TargetType >
  static ReturnType execute( FunctionType const& function, TargetType* const target )
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

struct no_object_encapsulator_construction
{
};

// ToDo: Handle empty types via inheritance
template< typename StoredObjectType, typename Enabler = void >
class object_encapsulator
{
public:
  // ToDo: Optimize function return
  typedef StoredObjectType inactive_value_type;
public:
  object_encapsulator( no_object_encapsulator_construction )
  {
  }

  template< typename FunctionPackageType >
  object_encapsulator( FunctionPackageType const& function_package )
  {
    function_package.store_function()( &stored_object_m );
  }

  template< typename ObjectType, typename FunctionPackageType >
  object_encapsulator( ObjectType& target
                     , FunctionPackageType const& function_package
                     )
  {
    new ( &stored_object_m ) StoredObjectType
          ( ::boost::mpl::if_< ::boost::is_void
                               < typename ObjectType::inactive_value_type >
                             , call_nullary_function_package_function
                             , call_unary_function_package_function
                             >
                             ::type
                             ::template execute< inactive_value_type >
                             ( function_package.store_function()
                             , &target.stored_object()
                             )
          );
  }
public:
  inactive_value_type inactive_value() const
  {
    return stored_object();
  }

  typedef StoredObjectType& stored_object_return_type;
private:
  typedef typename aligned_storage
          <
            sizeof( StoredObjectType )
          , alignment_of< StoredObjectType >::value
          >
          ::type raw_stored_object;
public:
  stored_object_return_type stored_object() const
  {
    return *reinterpret_cast< StoredObjectType* >( &stored_object_m );
  }

  void destroy_object() const
  {
    stored_object().~StoredObjectType();
  }
public:
  void* raw_object_pointer() const
  {
    return &stored_object_m;
  }
private:
  mutable raw_stored_object stored_object_m;
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
  object_encapsulator( no_object_encapsulator_construction )
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
                     , &target.stored_object()
                     );
  }
public:
  inactive_value_type inactive_value() const
  {
  }

  typedef dummy_stored_object_type& stored_object_return_type;

  stored_object_return_type stored_object() const
  {
    static dummy_stored_object_type dummy;
    return dummy;
  }

  void* raw_object_pointer() const
  {
    return &stored_object();
  }

  void destroy_object() const
  {
  }
private:
};

}
}
}

#endif
