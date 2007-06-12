/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_OPERATION_HPP
#define BOOST_ACT_INTERLOCKED_INCREMENT_DETAIL_INCREMENT_OPERATION_HPP

namespace boost { namespace act { namespace interlocked { namespace detail {

struct increment_operation
{
  struct new_from_old
  {
    template< typename ValueType >
    static ValueType execute( ValueType& value )
    {
      ValueType copy = value;

      return ++copy;
    }
  };

  struct old_from_new
  {
    template< typename ValueType >
    static ValueType execute( ValueType& value )
    {
      ValueType copy = value;

      return --copy;
    }
  };
};

} } } }

#endif
