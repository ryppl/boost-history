/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_LOAD_LOAD_SEQUENTIAL_HPP
#define BOOST_ACT_INTERLOCKED_LOAD_LOAD_SEQUENTIAL_HPP

#include <boost/act/config/interlocked/has.hpp>

#if BOOST_ACT_INTERLOCKED_HAS( load, sequential )

#include <boost/act/interlocked/semantics/sequential.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/act/interlocked/detail/interlocked_operand_validators.hpp>
#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/act/interlocked/detail/impl_meta.hpp>

#include <boost/act/interlocked/detail/impl.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_INFO                                 \
( load, sequential )

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_BEGIN()

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, sequential >
  , detail::are_valid_store_style_params< TargetType >
  , mpl::not_< detail::is_interlocked_bool< TargetType > >
  >
, remove_cv< TargetType >
>
::type
load( TargetType& destination )
{
  return detail::impl_meta< detail::load_sequential_impl, TargetType >
         ::execute( destination );

}

template< typename Semantics, typename TargetType >
typename lazy_enable_if
<
  mpl::and_
  <
    is_same< Semantics, sequential >
  , detail::are_valid_store_style_params< TargetType >
  , detail::is_interlocked_bool< TargetType >
  >
, remove_cv< TargetType >
>
::type
load( TargetType& destination )
{
  typedef typename remove_cv< TargetType >::type result_type;

  return result_type
         (
           interlocked::load< sequential >
           ( interlocked_bool_internal_value( destination )
           )
           != 0
         );
}

} } }

#include BOOST_ACT_INTERLOCKED_DETAIL_IMPL_END()

#endif

#endif
