/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_DEFAULT_IMPL_HPP
#define BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_DEFAULT_IMPL_HPP

#include <boost/act/interlocked/assign_if_was/assign_if_was.hpp>
#include <boost/act/interlocked/retrieve.hpp>
#include <boost/act/detail/prior.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename ResultType, typename UnqualifiedType >
struct increment_default_impl
{
  template< typename TargetType >
  static ResultType execute( TargetType& target )
  {
    UnqualifiedType new_value;

    for( UnqualifiedType curr_value = retrieve( target )
       ;    ( new_value = assign_if_was( target
                                       , act::detail::next( curr_value )
                                       , curr_value
                                       )
                            .old_value()
            )
         != curr_value
       ; curr_value = new_value
       );

    return new_value;
  }
};


} } } }

#endif
