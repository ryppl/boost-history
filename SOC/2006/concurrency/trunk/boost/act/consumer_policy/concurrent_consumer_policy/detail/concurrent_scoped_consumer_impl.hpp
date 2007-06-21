/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_SCOPED_C_HPP
#define BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_SCOPED_C_HPP

#include <boost/act/interlocked/assign/assign.hpp>
#include <boost/act/interlocked/retrieve.hpp>

#include <boost/thread/thread.hpp>

namespace boost { namespace act { namespace detail {

template< typename ConcurrentScopedConsumerType >
class wrapped_processor_void_shared_data
{
private:
  typedef typename ConcurrentScopedConsumerType::processor_type
            processor_type;
public:
  wrapped_processor_void_shared_data
  ( processor_type& processor_init
  , ConcurrentScopedConsumerType& body_init
  )
    : processor_m( processor_init ) {}
public:
  template< typename DenormalizedType >
  void operator ()( DenormalizedType& target ) const
  {
    processor_m( target );
  }
private:
  processor_type& processor_m;
};

template< typename ConcurrentScopedConsumerType >
struct wrapped_processor_meta
  : mpl::if_< is_void< typename ConcurrentScopedConsumerType
                      ::processor_shared_data_type
                     >
            , wrapped_processor_void_shared_data< ConcurrentScopedConsumerType >
            , void // ToDo: make nonvoid version
            > {};

template< typename ConcurrentScopedConsumerType >
class concurrent_consumer_nonvoid_main
{
private:
  typedef typename ConcurrentScopedConsumerType::processor_shared_data_type
            processor_shared_data_type;

  typedef typename ConcurrentScopedConsumerType::processor_type
            processor_type;
public:
  explicit concurrent_consumer_nonvoid_main
  ( ConcurrentScopedConsumerType& body_init )
    : body_m( body_init ) {}
public:
  void operator ()()
  {
    typedef typename ConcurrentScopedConsumerType::size_type size_type;

    typedef typename wrapped_processor_meta
            < ConcurrentScopedConsumerType >
            ::type wrapped_processor_type;

    for(;;)
    {
      bool const is_triggered_to_close
        = interlocked::retrieve( body_m.is_triggered_to_close_m );

      size_type queue_size = body_m.queue_m.size();

      if( queue_size == 0
          && is_triggered_to_close
        )
        break;

      if( queue_size > 0 )
        do
        {
          typedef typename ConcurrentScopedConsumerType
                  ::internal_value_type internal_value_type;

          internal_value_type curr_value = body_m.queue_m.front();
          body_m.queue_m.pop();
          wrapped_processor_type wrapped_processor( processor_m
                                                  //, curr_value.signaller
                                                  , body_m
                                                  );
          denormalizer_m( wrapped_processor, curr_value.value );
        }
        while( --queue_size > 0 );
      else
        thread::yield();
    }
  }
private:
  typedef typename ConcurrentScopedConsumerType::denormalizer_type
            denormalizer_type;
private:
  ConcurrentScopedConsumerType& body_m;
  processor_type processor_m;
  denormalizer_type denormalizer_m;
};

template< typename ConcurrentScopedConsumerType >
struct concurrent_consumer_nonvoid_main_meta
  : mpl::if_< is_void< typename ConcurrentScopedConsumerType::normalized_type >
            , void // ToDo: make void version
            , concurrent_consumer_nonvoid_main< ConcurrentScopedConsumerType >
            > {};


template< typename ConcurrentScopedConsumerType >
typename concurrent_consumer_nonvoid_main_meta
< ConcurrentScopedConsumerType >
::type
make_concurrent_consumer_nonvoid_main( ConcurrentScopedConsumerType& target )
{
  typedef typename concurrent_consumer_nonvoid_main_meta
          < ConcurrentScopedConsumerType >
          ::type result_type;

  return result_type( target );
}

template< typename ThreadPolicy, typename QueuePolicy
        , typename ProcessorType, typename ProcessorSharedDataType
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
struct concurrent_scoped_consumer_nonvoid_impl_data
{
public:
  typedef ProcessorType           processor_type;
  typedef ProcessorSharedDataType processor_shared_data_type;
  typedef NormalizerType          normalizer_type;
  typedef DenormalizerType        denormalizer_type;
  typedef NormalizedType          normalized_type;
public:
  concurrent_scoped_consumer_nonvoid_impl_data()
    : is_triggered_to_close_m( false ) {}
public:
  struct internal_value_type
  {
    template< typename ParamType >
    explicit internal_value_type
    ( reference_wrapper< normalizer_type > normalizer
    , reference_wrapper< ParamType > arg
    )
      : value( normalizer.get()( arg.get() ) ) {}

    normalized_type value;
  };
public:
  typedef typename queue_policy::safe_single_push_pop_queue
            < QueuePolicy, internal_value_type >::type queue_type;
public:
  typedef typename queue_type::size_type size_type;
public:
  interlocked::bool_fast_t is_triggered_to_close_m;
  normalizer_type normalizer_m;
  queue_type queue_m;
};

template< typename ThreadPolicy, typename QueuePolicy
        , typename Constraint
        , typename ProcessorType, typename ProcessorSharedDataType
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class concurrent_scoped_consumer_nonvoid_impl
{
public:
  typedef concurrent_consumer_policy< ThreadPolicy, QueuePolicy >
            consumer_policy;
public:
  typedef Constraint              constraint;
  typedef ProcessorType           processor_type;
  typedef ProcessorSharedDataType processor_shared_data_type;
  typedef NormalizerType          normalizer_type;
  typedef DenormalizerType        denormalizer_type;
  typedef NormalizedType          normalized_type;
public:
  concurrent_scoped_consumer_nonvoid_impl()
    : consumer_thread_m
      (
        make_concurrent_consumer_nonvoid_main( internal_data_m )
      )
  {
  }

  ~concurrent_scoped_consumer_nonvoid_impl()
  {
    interlocked::assign( internal_data_m.is_triggered_to_close_m
                       , true
                       );

    consumer_thread_m.join();
  }
public:
  template< typename Type >
  typename enable_if
  <
    typename mpl::apply< constraint, Type >::type
  >
  ::type operator ()( Type& arg )
  {
    consume_impl( arg );
  }

  template< typename Type >
  typename enable_if
  <
    typename mpl::apply< constraint, Type const >::type
  >
  ::type operator ()( Type const& arg )
  {
    consume_impl( arg );
  }
private:
  template< typename Type >
  void consume_impl( Type& arg )
  {
    internal_data_m.queue_m.push( in_place( ref( internal_data_m.normalizer_m )
                                          , ref( arg )
                                          )
                                );
  }
private:
  typedef typename ThreadPolicy::unmanaged_worker unmanaged_worker_type;
private:
  typedef concurrent_scoped_consumer_nonvoid_impl_data
          < ThreadPolicy, QueuePolicy
          , ProcessorType, ProcessorSharedDataType
          , NormalizerType, DenormalizerType
          , NormalizedType
          >
          concurrent_scoped_consumer_nonvoid_impl_data_type;
private:
  concurrent_scoped_consumer_nonvoid_impl_data_type internal_data_m;
  unmanaged_worker_type consumer_thread_m;
};

template< typename ThreadPolicy, typename QueuePolicy
        , typename Constraint
        , typename ProcessorType, typename ProcessorSharedDataType
        , typename NormalizerType, typename DenormalizerType
        , typename NormalizedType
        >
class concurrent_scoped_consumer_impl
  : public mpl::if_< mpl::false_ /*is_is_void< Constraint > */
                   , void
                   , concurrent_scoped_consumer_nonvoid_impl
                     < ThreadPolicy, QueuePolicy
                     , Constraint
                     , ProcessorType, ProcessorSharedDataType
                     , NormalizerType, DenormalizerType
                     , NormalizedType
                     >
                   >
                   ::type
{
};

} } }

#endif
