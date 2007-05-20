/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_CON_TRA_HPP
#define BOOST_ACT_CONSUMER_POLICY_SCOPED_CONCURRENT_CONSUMER_DETAIL_CON_TRA_HPP

#include <boost/act/consumer_policy/concurrent_consumer_policy/detail/concurrent_tracking_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>

#include <boost/shared_ptr.hpp>

#include <boost/act/detail/unspecified_bool.hpp>

namespace boost {

class in_place_factory_base;

namespace act { namespace detail {

template< typename ValueType >
class concurrent_signaller_impl;

template< typename ValueType >
class concurrent_tracker_impl
{
  friend class concurrent_signaller_impl< ValueType >;
private:
  struct unspecified_bool_tag;
public:
  typedef ValueType value_type;
private:
  typedef detail::unknown_concurrent_tracking_info unknown_tracking_info;
  typedef detail::concurrent_tracking_info< value_type > tracking_info;
public:
  concurrent_tracker_impl() {}
  explicit concurrent_tracker_impl( concurrent_signaller_impl< ValueType > const& source )
    : tracking_info_m( source.tracking_info_m ) {}
 private:
  explicit concurrent_tracker_impl( shared_ptr< tracking_info > const& tracking_info_init )
    : tracking_info_m( tracking_info_init ) ) {}
public:
  bool has_value() const
  {
    return tracking_info_m->is_signalled();
  }

  void wait() const
  {
    tracking_info_m->wait();
  }

  value_type value() const
  {
    return tracking_info_m->wait_for_value();
  }
public:
  operator BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_T( (unspecified_bool_tag) )() const
  {
    return BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_T( (unspecified_bool_tag)
                                                    , has_value()
                                                    );
  }
private:
  shared_ptr< tracking_info > tracking_info_m;
};

} } }

#endif
