/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_BASE_HPP
#define BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_BOUNDED_QUEUE_BASE_HPP

#include <cstddef>
#include <new>
#include <memory>

#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <boost/utility/in_place_factory.hpp>

#include <cstddef>

namespace boost { namespace act { namespace detail {

// ToDo: Implement move semantics
template< typename ChildType
        , typename Type, ::std::size_t MaxSize, typename Allocator
        >
struct bounded_queue_base
{
public:
  typedef ValueType         value_type;
  typedef ::std::size_t     size_type;

  typedef typename add_reference< value_type >::type       reference;
  typedef typename add_reference< value_type const >::type const_reference;

  typedef value_type*       pointer;
  typedef value_type const* const_pointer;
public:
  bounded_queue_base()
    : begin_m( reinterpret_cast< Type* >( &buffer_m ) ), end_m( begin_m ) {}
private:
  struct destroyer
  {
    void operator ()( Type& target ) const { target.~Type(); }
  };
public:
  ~bounded_queue_base()
  {
    for_each( destroyer() );
  }
private:
  // ToDo: Factor out to separate functions
  template< typename FunctionType >
  void for_each( FunctionType function )
  {
    Type* const pivot_end_val = pivot_end();

    if( end_m < pivot_end_val )
      for( Type* curr = begin_m; curr != end_m; ++curr )
        function( *curr );
    else
    {
      // First segment
      for( Type* curr = begin_m; curr != pivot_end_val; ++curr )
        function( *curr );

      // Second segment
      for( Type* curr = raw_begin(); curr != end_m; ++curr )
        function( *curr );
    }
  }
public:
  static size_type max_size() { return MaxSize; }
private:
  struct no_arg_factory
  {
    template< typename Type >
    void apply( void* address ) const
    {
      new( address ) Type;
    }
  };
public:
  void push()
  {
    push_back( no_arg_factory() );
  }

  void push( Type const& source )
  {
    push_back( in_place( source ) );
  }
protected:
  Type& front() { return *begin_m; }
  Type const& front() const { return *begin_m; }

  Type& back() { return *( end_m - 1 ); }
  Type const& back() const { return *( end_m - 1 ); }
protected:
  Type* raw_begin() { return reinterpret_cast< Type* >( &buffer_m ); }
  Type const* raw_begin() const { return reinterpret_cast<Type*>( &buffer_m ); }

  Type* pivot_end() { return raw_begin() + max_size_c; }
  Type const* pivot_end() const { return raw_begin() + max_size_c; }
protected:
  Type* next( Type* curr )
  {
    return   ( curr == raw_begin() + max_size_c - 1 )
           ? raw_begin()
           : begin_m + 1;
  }

  Type* next_end()
  {
    return   ( end_m == pivot_end() )
           ? raw_begin() + 1
           : end_m + 1;

  }
private:
  typedef typename aligned_storage< sizeof( Type[ max_size_c + 1 ] )
                                  , alignment_of< Type >::value
                                  >
                                  ::type buffer_type;
protected:
  buffer_type buffer_m;
  Type* begin_m,
      * end_m;
};

} } }

#endif
