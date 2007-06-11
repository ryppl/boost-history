/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_OPERATION_HPP
#define BOOST_ACT_INTERLOCKED_ASSIGN_DETAIL_ASSIGN_OPERATION_HPP

namespace boost { namespace act { namespace interlocked { namespace detail {

struct assign_operation
{
  struct new_from_old
  {
    template< typename ValueType, typename OperandType >
    static ValueType execute( ValueType& value, OperandType& operand )
    {
      return static_cast< ValueType >( operand );
    }
  };

  struct old_from_new;
};

} } } }

#endif
