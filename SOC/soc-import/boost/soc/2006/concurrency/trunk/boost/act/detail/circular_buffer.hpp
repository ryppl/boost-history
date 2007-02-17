/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_CIRCULAR_BUFFER_HPP
#define BOOST_ACT_DETAIL_CIRCULAR_BUFFER_HPP

#include <cstddef>

#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <boost/utility/in_place_factory.hpp>

namespace boost
{
namespace act
{
namespace detail
{

// ToDo: Error if BufferSize = 0
template< typename Type, ::std::size_t BufferSize >
class circular_buffer
{
public:
  typedef ::std::size_t size_type;
public:
  circular_buffer()
    : begin_m( reinterpret_cast< Type* >( &buffer_m ) ), end_m( begin_m ) {}
private:
  struct destroyer
  {
    void operator ()( Type& target ) const { target.~Type(); }
  };
public:
  ~circular_buffer()
  {
    for_each( destroyer() );
  }
public:
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
  bool empty() const { return begin_m == end_m; }
  static size_type max_size() { return BufferSize; }
  bool full() const { return end_m == begin_m + 1; }
public:
  void push_back( Type const& source )
  {
    push_back( in_place( source ) );
  }

  template< typename InPlaceFactory >
  void push_back( InPlaceFactory factory )
  {
    if( full() ) { throw; }
      // ToDo: Throw exception or assert

    Type* next_end;

    next_end = ( ( end_m == pivot_end() )
               ? raw_begin() + 1
               : end_m + 1
               );

    factory.template apply< Type >( next_end - 1 );

    end_m = next_end;
  }

  void pop_front()
  {
    if( empty() ) { throw; }
      // ToDo: Throw exception or assert

    Type* next_begin;

    next_begin = ( ( begin_m == raw_begin() + BufferSize - 1 )
                 ? raw_begin()
                 : begin_m + 1
                 );

    begin_m->~Type();

    begin_m = next_begin;
  }

  Type& front() { return *begin_m; }
  Type const& front() const { return *begin_m; }
private:
  Type* raw_begin() { return reinterpret_cast< Type* >( &buffer_m ); }
  Type const* raw_begin() const { return reinterpret_cast<Type*>( &buffer_m ); }

  Type* pivot_end() { return raw_begin() + BufferSize; }
  Type const* pivot_end() const { return raw_begin() + BufferSize; }
private:
  typedef typename aligned_storage< sizeof( Type[ BufferSize ] )
                                  , alignment_of< Type >::value
                                  >
                                  ::type buffer_type;
                  
private:
  buffer_type buffer_m;
  Type* begin_m,
      * end_m;
};

}
}
}

#endif
