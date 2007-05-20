/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_SPP_QUEUE_HPP
#define BOOST_ACT_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_SPP_QUEUE_HPP

#include <boost/config.hpp>

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#include <boost/cstdint.hpp>

#ifdef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

// Interlocked operations for lock-free programming
#include <boost/act/interlocked/increment/increment_release.hpp>
#include <boost/act/interlocked/decrement/decrement_release.hpp>
#include <boost/act/interlocked/integer/types.hpp>
#include <boost/act/interlocked/safe_get.hpp>

#include <cstddef>
#include <new>

#include <boost/type_traits/aligned_storage.hpp>
#include <boost/type_traits/alignment_of.hpp>

#include <boost/utility/in_place_factory.hpp>

#include <boost/act/queue_policy/bounded_queue_policy/safe_single_push_pop_queue_fwd.hpp>

#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy.hpp>
#include <boost/act/queue_policy/bounded_queue_policy/bounded_queue_policy_arguments.hpp>

// For yield
#include <boost/thread/thread.hpp>

#include <cassert>

namespace boost { namespace act {

namespace detail
{

// Note: This is a template whose instantiations represent a thread-safe
//       and lock-free queue where there is one "pusher thread" (producer) and
//       one "popper thread" (consumer).
//
//       The long, descriptive name "bounded_safe_single_push_pop_queue" is
//       never exposed to users of the code (which is why it is in a detail
//       namespace). This template is used internally in producer/consumer
//       templates which are exposed to users, and indirectly in active objects.

// Template args: Max size of a given bounded queue (stored on the stack)
//                Type to be stored in the queue (can be any copyable type)
template< ::std::size_t MaxSize, typename ValueType >
struct bounded_safe_single_push_pop_queue
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
  bounded_safe_single_push_pop_queue()
    : begin_m( reinterpret_cast< pointer >( &buffer_m ) ), end_m( begin_m )
    , curr_size_m( 0 ) {}
public:
  ~bounded_safe_single_push_pop_queue() { clear(); }
public:
  void clear()
  {
    for( size_type curr_size = size(); curr_size > 0; --curr_size )
      pop();
  }
public:
  size_type size() const
  {
    // Safe get insures an added volatile qualifier and that the type can safely
    // be read in whole when another thread is writing to it.
    return static_cast< size_type >( interlocked::safe_get( curr_size_m ) );
  }

  bool empty() const { return size() == 0; }
  static size_type max_size() { return MaxSize; }
  bool full() const { return size() == MaxSize; }
private:
  // Special case for a factory with 0 args (boost doesn't yet support 0 args)
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
    push( in_place( source ) );
  }

  template< typename InPlaceFactory >
  void push( InPlaceFactory const& factory )
  {
    while( full() ) thread::yield();

    end_m = next_end();

    // In-place construction.
    factory.template apply< value_type >( end_m - 1 );

    // Thread-safe incrememnt with release memory semantics.
    interlocked::increment_release( curr_size_m );
  }

  void pop()
  {
    // Note: Assert here as opposed to an exception because such an operation is
    //       always a logical error.
    assert( !empty() );

    begin_m->~value_type();

    begin_m = next( begin_m );

    // Thread-safe decrement with release memory semantics.
    interlocked::decrement_release( curr_size_m );
  }

  reference front() { return *begin_m; }
  const_reference front() const { return *begin_m; }

  reference back() { return *( end_m - 1 ); }
  const_reference back() const { return *( end_m - 1 ); }
private:
  pointer raw_begin() { return reinterpret_cast< pointer >( &buffer_m ); }
  const_pointer raw_begin() const { return reinterpret_cast<Type*>( &buffer_m ); }

  pointer pivot_end() { return raw_begin() + MaxSize; }
  const_pointer pivot_end() const { return raw_begin() + MaxSize; }
private:
  // The logical size of the range [range_begin,range_end)
  difference_type difference( pointer range_begin, pointer range_end )
  {
    return   range_begin <= range_end
           ? range_end - range_begin
           :   ( pivot_end() - range_begin )
             + ( range_end - raw_begin() );
  }
private:
  // The next logical element address
  pointer next( pointer curr )
  {
    return   ( curr == raw_begin() + MaxSize - 1 )
           ? raw_begin()
           : begin_m + 1;
  }

  // The next logical end address
  pointer next_end()
  {
    return   ( end_m == pivot_end() )
           ? raw_begin() + 1
           : end_m + 1;

  }
private:
  // Use properly-aligned raw storage.
  typedef typename aligned_storage< sizeof( value_type[ MaxSize + 1 ] )
                                  , alignment_of< value_type >::value
                                  >
                                  ::type buffer_type;
private:
  buffer_type buffer_m;
  pointer begin_m,
          end_m;

  // interlocked::size_t is a type which can store all the values that a size_t
  // may hold and which is guaranteed to support interlocked operations. If the
  // platform does not support such a type, an error would be generated, though
  // in practice, platforms which support interlocked operations support
  // interlocked size_t.
  interlocked::size_t curr_size_m;
};

}

namespace queue_policy
{

// Macro parameter allows for named argument passing such that users do not have
// to remember the order of the arguments in a potentially large argument list.
template< BOOST_ACT_BOUNDED_QUEUE_POLICY_PARAMS(), typename ValueType >
struct safe_single_push_pop_queue
         < bounded_queue_policy< BOOST_ACT_BOUNDED_QUEUE_POLICY_ARGS() >
         , ValueType
         >
{
  typedef detail::bounded_safe_single_push_pop_queue
          <
            BOOST_ACT_BOUNDED_QUEUE_POLICY_TRAITS()::max_size_t::value
          , ValueType
          >
          type;
};

}

} }

#endif

#endif
