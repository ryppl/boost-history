/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_INTEGER_DETAIL_CONVERT_INTERBOOL_OP_TO_BOOL_HPP
#define BOOST_ACT_INTERLOCKED_INTEGER_DETAIL_CONVERT_INTERBOOL_OP_TO_BOOL_HPP

#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< typename InterlockedBoolType, typename SourceType >
struct interlocked_bool_operand_intermediate_conversion_type
  : mpl::if_< is_convertible< SourceType const, bool >
            , bool
            , InterlockedBoolType
            > {};

template< typename InterlockedBoolType, typename SourceType >
bool convert_interlocked_bool_operand_to_bool( SourceType const& source )
{
  typedef typename interlocked_bool_operand_intermediate_conversion_type
          <
            InterlockedBoolType
          , SourceType
          >
          ::type intermediate_type;

  return static_cast< bool >( static_cast< intermediate_type >( source ) );
}

} } } }

#endif
