/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_QUEUE_HPP
#define BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_QUEUE_HPP

#include <cstddef>
#include <new>

#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <boost/utility/in_place_factory.hpp>

#include <boost/act/queue_policy/bounded_queue_policy/queue_fwd.hpp>

#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy.hpp>
#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy_arguments.hpp>

// ToDo: Implement move semantics
//       Handle reference types

namespace boost { namespace act {

namespace detail
{

template< ::std::size_t MaxSize, typename ValueType >
struct bounded_queue
{
public:
  typedef ValueType         value_type;
  typedef ::std::size_t     size_type;
  typedef ::std::ptrdiff_t  difference_type;

  typedef value_type&       reference;
  typedef value_type const& const_reference;

  typedef value_type*       pointer;
  typedef value_type const* const_pointer;
public:
  bounded_queue()
    : begin_m( reinterpret_cast< pointer >( &buffer_m ) ), end_m( begin_m ) {}
private:
  struct destroyer
  {
    void operator ()( const_reference target ) const { target.~value_type(); }
  };
public:
  ~bounded_queue()
  {
    for_each( destroyer() );
  }
private:
  // ToDo: Factor out to separate functions
  template< typename FunctionType >
  void for_each( FunctionType function )
  {
    pointer const pivot_end_val = pivot_end();

    if( end_m < pivot_end_val )
      for( pointer curr = begin_m; curr != end_m; ++curr )
        function( *curr );
    else
    {
      // First segment
      for( pointer curr = begin_m; curr != pivot_end_val; ++curr )
        function( *curr );

      // Second segment
      for( pointer curr = raw_begin(); curr != end_m; ++curr )
        function( *curr );
    }
  }
public:
  bool empty() const { return begin_m == end_m; }
  static size_type max_size() { return max_size_c; }
  bool full() const { return end_m == begin_m + 1; }
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

  void push( const_reference source )
  {
    push_back( in_place( source ) );
  }

  template< typename InPlaceFactory >
  void push( InPlaceFactory const& factory )
  {
    if( full() ) { throw; }
      // ToDo: Throw exception or assert

    pointer const next_end = this->next_end();

    factory.template apply< Type >( next_end - 1 );

    end_m = next_end;
  }

  void pop()
  {
    if( empty() ) { throw; }
      // ToDo: Throw exception or assert

    pointer next_begin = next( begin_m );

    begin_m->~value_type();

    begin_m = next_begin;
  }

  reference front() { return *begin_m; }
  const_reference front() const { return *begin_m; }

  reference back() { return *( end_m - 1 ); }
  const_reference back() const { return *( end_m - 1 ); }
private:
  pointer raw_begin() { return reinterpret_cast< pointer >( &buffer_m ); }
  const_pointer raw_begin() const { return reinterpret_cast<Type*>( &buffer_m ); }

  pointer pivot_end() { return raw_begin() + max_size_c; }
  const_pointer pivot_end() const { return raw_begin() + max_size_c; }
private:
  pointer next( pointer curr )
  {
    return   ( curr == raw_begin() + max_size_c - 1 )
           ? raw_begin()
           : begin_m + 1;
  }

  pointer next_end()
  {
    return   ( end_m == pivot_end() )
           ? raw_begin() + 1
           : end_m + 1;

  }
private:
  typedef typename aligned_storage< sizeof( value_type[ MaxSize + 1 ] )
                                  , alignment_of< value_type >::value
                                  >
                                  ::type buffer_type;
private:
  buffer_type buffer_m;
  pointer begin_m,
          end_m;
};

}

namespace queue_policy
{

template< BOOST_ACT_BOUNDED_QUEUE_POLICY_PARAMS(), typename ValueType >
struct queue< bounded_queue_policy< BOOST_ACT_BOUNDED_QUEUE_POLICY_ARGS() >
            , ValueType
            >
{
  typedef detail::bounded_queue
          <
            BOOST_ACT_BOUNDED_QUEUE_POLICY_TRAITS()::max_size_t::value
          , ValueType
          >
          type;
};

}

} }

#endif
