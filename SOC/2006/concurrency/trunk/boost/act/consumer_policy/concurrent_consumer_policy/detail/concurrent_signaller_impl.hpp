/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_CON_SIG_HPP
#define BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_CON_SIG_HPP

#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_tracking_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/shared_ptr.hpp>

namespace boost {

class in_place_factory_base;

namespace act { namespace detail {

template< typename ValueType >
class concurrent_tracker_impl;

template< typename ValueType >
class concurrent_signaller_impl
{
  friend class concurrent_tracker_impl< ValueType >;
public:
  typedef ValueType value_type;
private:
  typedef detail::concurrent_tracking_info< value_type > tracking_info;
public:
  concurrent_signaller_impl() : tracking_info_m( new tracking_info ) {}
public:
  template< typename InPlaceFactory >
  typename enable_if
  <
   is_convertible< InPlaceFactory const volatile*
                 , in_place_factory_base const volatile*
                 >
  , bool
  >
  ::type
  signal( InPlaceFactory const& factory )
  {
    return tracking_info_m->signal_factory( factory );
  }

  template< typename FunctionObject >
  typename disable_if
  <
    is_convertible< FunctionObject const volatile*
                  , in_place_factory_base const volatile*
                  >
  , bool
  >
  ::type
  signal( FunctionObject function )
  {
    return tracking_info_m->signal_function( function );
  }

  template< typename SourceValueType >
  typename enable_if
  <
    is_convertible< SourceValueType const
                  , value_type
                  >
  , bool
  >
  ::type
  signal( SourceValueType const& value )
  {
    return tracking_info_m->signal( value );
  }
public:
  concurrent_tracker_impl< value_type > tracker() const
  {
    return concurrent_tracker_impl< value_type >( *this );
  }
public:
  bool is_signalled() const
  {
    return tracking_info_m->is_signalled();
  }
private:
  shared_ptr< tracking_info > tracking_info_m;
};

} } }

#endif
