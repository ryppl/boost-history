/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_DETAIL_SCOPED_CONCURRENT_CONSUMER_IMPL_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_DETAIL_SCOPED_CONCURRENT_CONSUMER_IMPL_HPP

#include <cstddef>
#include <new>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/type_traits/is_void.hpp>

#include <boost/mpl/apply.hpp>

#include <boost/act/detail/circular_buffer.hpp>

#include <boost/act/detail/raw_storage.hpp>

#include <boost/act/queue_policy/safe_single_push_pop_queue.hpp>

#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy.hpp>
//#include <boost/act/consumer_policy/concurrent_consumer_policy/tracker.hpp>
#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_tracking_info.hpp>

#include <boost/act/consumer_policy/consumer_arguments.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/bind.hpp>

#include <memory>

namespace boost { namespace act { namespace detail {

// ToDo: Template for buffer, not buffer size
//       Optimize away locking when possible (normalized type has trivial copy)
//       Move code to base that disregards order of template args then promote
template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        , typename Enabler = void
        >
class scoped_concurrent_consumer_impl;

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class scoped_concurrent_consumer_impl_base;

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        , typename Enabler = void
        >
struct internal_concurrent_consumer_value_type
{
  typedef shared_ptr< detail::unknown_concurrent_tracking_info >
            tracking_info_ptr;

  template< typename InputType >
  internal_concurrent_consumer_value_type
  ( reference_wrapper< NormalizerType > normalizer
  , reference_wrapper< InputType > input
  )
    : value( normalizer.get()( input.get() ) ) {}

  template< typename InputType >
  internal_concurrent_consumer_value_type
  ( reference_wrapper< NormalizerType > normalizer
  , reference_wrapper< InputType > input
  , reference_wrapper
    <
      std::auto_ptr< detail::unknown_concurrent_tracking_info >
    >
    tracking_info_init
  )
    : value( normalizer.get()( input.get() ) )
    , tracking_info( tracking_info_init.get().release() ) {}

  NormalizedType value;
  tracking_info_ptr const tracking_info;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
struct internal_concurrent_consumer_value_type
       <
         ThreadAllocator, QueuePolicy
       , Restraint, ProcessorType, ProcessorSharedData
       , NormalizerType, DenormalizerType
       , NormalizedType
       , typename enable_if< is_void< NormalizedType > >::type
       >
{

  typedef shared_ptr< detail::unknown_concurrent_tracking_info >
            tracking_info_ptr;

  explicit internal_concurrent_consumer_value_type
  ( reference_wrapper
    <
      std::auto_ptr< detail::unknown_concurrent_tracking_info >
    >
    tracking_info_init
  )
    : tracking_info( tracking_info_init.get().release() ) {}

  internal_concurrent_consumer_value_type() {}

  tracking_info_ptr const tracking_info;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class concurrent_processor_wrapper
{
private:
  typedef shared_ptr< detail::unknown_concurrent_tracking_info >
            tracking_info_ptr;
public:
  concurrent_processor_wrapper
  (
    ProcessorType& processor_init
  , tracking_info_ptr const& tracking_info_init
  )
    : processor_m( processor_init )
    , tracking_info_m( tracking_info_init ) {}
public:
  template< typename Type, typename Tag >
  void operator ()( Type& target, Tag ) const
  {
    // ToDo: Optimize away copy (call processor inside of signal)
    if( tracking_info_m )
    {
      typedef typename Tag::type result_type;
      tracking_info_m
        ->signal< result_type >( bind< result_type >( ref( processor_m ), ref( target ) ) );
    }
    else
      processor_m( target );
  }
private:
  ProcessorType& processor_m;
  tracking_info_ptr const tracking_info_m;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        , typename Enabler = void
        >
class concurrent_consumer_main
{
private:
  typedef scoped_concurrent_consumer_impl_base
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          scoped_concurrent_consumer_type;

  typedef concurrent_processor_wrapper
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          processor_wrapper_type;
public:
  concurrent_consumer_main
  ( scoped_concurrent_consumer_type& consumer_body_init )
    : consumer_body_m( consumer_body_init ) {}
public:
  void operator ()() const
  {
    // ToDo: Optimize use of curr_value

    // ToDo: Catch exceptions and report to scoped_consumer in debug mode
    while( !(    ( interlocked::value( consumer_body_m.closing_m ) != 0 )
              && consumer_body_m.buffer_m.empty()
            )
         )
      // ToDo: Change to use size_type of queue
      for( ::std:size_t size = consumer_body_m.buffer_m.size()
         ; size != 0
         ; --size
         )
      {
        NormalizedType curr_value = consumer_body_m.buffer_m.front().value;

        processor_wrapper_type processor_wrapper
          ( consumer_body_m.processor_m
          , consumer_body_m.buffer_m.front().tracking_info
          );

        consumer_body_m.buffer_m.pop_front();

        // ToDo: Use denormalizer
        denormalizer_m( processor_wrapper, curr_value );
      }
  }
private:
  scoped_concurrent_consumer_type& consumer_body_m;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class concurrent_consumer_main
      < ThreadAllocator, QueuePolicy
      , Restraint, ProcessorType, ProcessorSharedData
      , NormalizerType, DenormalizerType, NormalizedType
      , typename enable_if< is_void< NormalizedType > >::type
      >
{
private:
  typedef scoped_concurrent_consumer_impl_base
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          scoped_concurrent_consumer_type;
public:
  concurrent_consumer_main
  ( scoped_concurrent_consumer_type& consumer_body_init )
    : consumer_body_m( consumer_body_init ) {}
public:
  void operator ()() const
  {
    // ToDo: Catch exceptions and report to scoped_consumer in debug mode
    mutex::scoped_lock lock( consumer_body_m.mutex_m );
    while( !(    consumer_body_m.closing_m
              && consumer_body_m.buffer_m.empty()
            )
         )
    {
      if( consumer_body_m.buffer_m.empty() )
        consumer_body_m.condition_m.wait( lock );

      // Note: The only time the buffer can be empty here is
      //       if the consumer was triggered to close
      if( consumer_body_m.buffer_m.empty() )
        break;
      else
      {
        typedef shared_ptr< detail::unknown_concurrent_tracking_info >
                  tracking_info_ptr;

        tracking_info_ptr const tracking_info
          = consumer_body_m.buffer_m.front().tracking_info;

        bool const buffer_was_full = consumer_body_m.buffer_m.full();

        consumer_body_m.buffer_m.pop_front();

        lock.unlock();

        // Notify the producer that the buffer isn't full when necessary
        if( buffer_was_full )
          consumer_body_m.condition_m.notify_one();

        // ToDo: Use more proper result_type check (using result_of)
        typedef typename detail::result_type
                  < ProcessorType, void >::type result_type;

        if( tracking_info )
          tracking_info->signal< result_type >( consumer_body_m.processor_m );
        else
          consumer_body_m.processor_m();

        lock.lock();
      }
    }
  }
private:
  scoped_concurrent_consumer_type& consumer_body_m;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class scoped_concurrent_consumer_impl_base
{
public:
  typedef ProcessorType    processor_type;
  typedef Restraint        restraint;
  typedef NormalizerType   normalizer_type;
  typedef DenormalizerType denormalizer_type;
  typedef NormalizedType   normalized_type;
public:/*
  template< typename ParamType, typename Enabler = void >
  struct tracker_from_param
  {
    typedef typename concurrent_consumer_policy< ThreadAllocator,BufferSize >
            ::template tracker
            <
              typename result_of< processor_type( ParamType ) >::type
            >
            type;
  };

  template< typename ParamType >
  struct tracker_from_param< ParamType
                           , typename enable_if< is_void< ParamType > >::type
                           >
  {
    typedef typename concurrent_consumer_policy< ThreadAllocator,BufferSize >
            ::template tracker
            <
              typename result_of< processor_type() >::type
            >
            type;
  };*/
private:
  typedef typename ThreadAllocator::unmanaged_worker unmanaged_worker;
public:
  scoped_concurrent_consumer_impl_base() { intialize(); }
private:
  void intialize()
  {
    typedef concurrent_consumer_main< ThreadAllocator, BufferSize
                                    , ProcessorType, Restraint, NormalizerType
                                    , NormalizedType
                                    >
                                    consumer_main_type;

    // ToDo: Possible change memory barrier
    interlocked::set( closing_m, 0 );
    new( &thread_m ) unmanaged_worker( consumer_main_type( *this ) );
  }
private:
  unmanaged_worker& thread_object()
  {
    return reinterpret_cast< unmanaged_worker& >( thread_m );
  }
public:
  ~scoped_concurrent_consumer_impl_base()
  {
    // ToDo: Possible change memory barrier
    interlocked::set( closing_m, 1 );

    thread_object().join();
    thread_object().~unmanaged_worker();
  }
private:
  typedef internal_concurrent_consumer_value_type
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          internal_value_type;
private:
  typedef typename queue_policy::safe_single_push_pop_queue
          < QueuePolicy, normalized_type >
          ::type buffer_type;
public:
  buffer_type buffer_m;
  processor_type processor_m;
  normalizer_type normalizer_m;
  denormalizer_type denormalizer_m;
  typename detail::raw_storage< unmanaged_worker >::type thread_m;
  long closing_m;
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        , typename Enabler
        >
class scoped_concurrent_consumer_impl
  : scoped_concurrent_consumer_impl_base
    < ThreadAllocator, QueuePolicy
    , Restraint, ProcessorType, ProcessorSharedData
    , NormalizerType, DenormalizerType, NormalizedType
    >
{
public:
  typedef ProcessorType processor_type;
  typedef Restraint restraint;
  typedef NormalizerType normalizer_type;
  typedef NormalizedType normalized_type;
private: // ToDo: Change to private and befriend consumer main
  typedef scoped_concurrent_consumer_impl_base
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          impl_base;

  using impl_base::buffer_m;
  using impl_base::processor_m;
  using impl_base::normalizer_m;
  using impl_base::thread_m;
  using impl_base::mutex_m;
  using impl_base::condition_m;
  using impl_base::closing_m;
public:/*
  template< typename ParamType = void >
  struct tracker_from_param
    : impl_base::template tracker_from_param< ParamType >
  {
  };*/
private:
  typedef typename ThreadAllocator::unmanaged_worker unmanaged_worker;
public:
  scoped_concurrent_consumer_impl() {}
public:
  // ToDo: Allow in-place construction
  template< typename InputType >
  typename enable_if
  <
    typename mpl::apply< restraint, InputType >::type
  >
  ::type
  operator ()( InputType& input )
  {
    consume_impl( input );
  }

  template< typename InputType >
  typename enable_if
  <
    typename mpl::apply< restraint, InputType const >::type
  >
  ::type
  operator ()( InputType const& input )
  {
    consume_impl( input );
  }
/*
  // ToDo: Account for reference wrapped params
  template< typename InputType >
  typename lazy_enable_if
  <
    typename mpl::apply< restraint, InputType >::type
  , tracker_from_param< InputType >
  >
  ::type
  tracked_consume( InputType& input )
  {
    return tracked_consume_impl( input );
  }

  // ToDo: Account for reference wrapped params
  template< typename InputType >
  typename lazy_enable_if
  <
    typename mpl::apply< restraint, InputType const >::type
  , tracker_from_param< InputType >
  >
  ::type
  tracked_consume( InputType const& input )
  {
    return tracked_consume_impl( input );
  }*/
private:
  template< typename InputType >
  void consume_impl( InputType& input )
  {
    buffer_m.push_back( in_place( ref( normalizer_m ), ref( input ) ) );
  }
/*
  // TODO: Fix this function
  template< typename InputType >
  typename tracker_from_param< InputType >::type
  tracked_consume_impl( InputType& input )
  {
    typedef typename result_of< processor_type( InputType ) >::type result_type;
    typedef detail::concurrent_tracking_info< result_type > tracking_info_type;

    // ToDo: Fix type
    signaller< result_type > result_signaller;

    buffer_m.push_back( in_place( ref( normalizer_m )
                                , ref( input )
                                , result_signaller
                                )
                      );

    typedef typename tracker_from_param< InputType >::type tracker_type;

    return result_signaller.tracker();
  }*/
};

template< typename ThreadAllocator, typename QueuePolicy
        , typename Restraint
        , typename ProcessorType, typename ProcessorSharedData
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class scoped_concurrent_consumer_impl
      < ThreadAllocator, QueuePolicy
      , Restraint, ProcessorType, ProcessorSharedData
      , NormalizerType, DenormalizerType, NormalizedType
      , typename enable_if< is_void< NormalizedType > >::type
      >
  : scoped_concurrent_consumer_impl_base
    < ThreadAllocator, QueuePolicy
    , Restraint, ProcessorType, ProcessorSharedData
    , NormalizerType, DenormalizerType, NormalizedType
    >
{
public:
  typedef ProcessorType processor_type;
  typedef Restraint restraint;
  typedef NormalizerType normalizer_type;
  typedef NormalizedType normalized_type;
public: // ToDo: Change to private and befriend consumer main
  typedef scoped_concurrent_consumer_impl_base
          < ThreadAllocator, QueuePolicy
          , Restraint, ProcessorType, ProcessorSharedData
          , NormalizerType, DenormalizerType, NormalizedType
          >
          impl_base;

  using impl_base::buffer_m;
  using impl_base::processor_m;
  using impl_base::normalizer_m;
  using impl_base::thread_m;
  using impl_base::mutex_m;
  using impl_base::condition_m;
  using impl_base::closing_m;
public:/*
  template< typename ParamType = void >
  struct tracker_from_param
    : impl_base::template tracker_from_param< ParamType >
  {
  };*/
private:
  typedef typename ThreadAllocator::unmanaged_worker unmanaged_worker;
public:
  explicit scoped_concurrent_consumer_impl
  ( concurrent_consumer_policy< ThreadAllocator
                             , QueuePolicy
                             > const& allocator_init
  )
      : scoped_concurrent_consumer_impl( allocator_init ) {}

  explicit scoped_concurrent_consumer_impl
  ( processor_type const& processor_init )
    : scoped_concurrent_consumer_impl( processor_init ) {}

  scoped_concurrent_consumer_impl() {}
public:
  // ToDo: Allow in-place construction
  void operator ()()
  {
    consume_impl();
  }
/*
  // ToDo: Account for reference wrapped params
  typename tracker_from_param< void >::type
  operator ()()
  {
    return tracked_consume_impl();
  }*/
private:
  void consume_impl()
  {
    mutex::scoped_lock lock( mutex_m );

    if( buffer_m.full() )
      condition_m.wait( lock );

    bool const buffer_was_empty = buffer_m.empty();

    buffer_m.push_back();

    lock.unlock();

    // Notify the consumer of the function when necessary
    if( buffer_was_empty )
      condition_m.notify_one();
  }
/*
  typename tracker_from_param< void >::type
  tracked_consume_impl()
  {
    // ToDo: Adjust to accoutn for no result_of info (default to void)
    typedef typename result_of< processor_type( void ) >::type result_type;
    typedef detail::concurrent_tracking_info< result_type > tracking_info_type;

    std::auto_ptr< detail::unknown_concurrent_tracking_info >
      tracking_info( new tracking_info_type );

    mutex::scoped_lock lock( mutex_m );

    if( buffer_m.full() )
      condition_m.wait( lock );

    bool const buffer_was_empty = buffer_m.empty();

    buffer_m.push_back( in_place( ref( tracking_info ) ) );

    typedef typename tracker_from_param< void >::type tracker_type;

    tracker_type const result( buffer_m.back().tracking_info );

    lock.unlock();

    // Notify the consumer of the function when necessary
    if( buffer_was_empty )
      condition_m.notify_one();

    return result;
  }
*/
};

} } }

#endif
