/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_OPERATION_HPP
#define BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_OPERATION_HPP

#include <boost/act/detail/next.hpp>
#include <boost/act/detail/prior.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

struct increment_operation
{
  struct new_from_old
  {
    template< typename ValueType >
    static ValueType execute( ValueType& value )
    {
      return act::detail::next( value );
    }
  };

  struct old_from_new
  {
    template< typename ValueType >
    static ValueType execute( ValueType& value )
    {
      return act::detail::prior( value );
    }
  };
};

} } } }

#endif
