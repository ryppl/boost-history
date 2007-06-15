/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_OPERAND_VALIDATORS_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_INTERLOCKED_OPERAND_VALIDATORS_HPP

#include <climits>

#if CHAR_BIT == 8

// ToDo: Change
#include <boost/act/interlocked/integer/has_interlocked_operations.hpp>

// #include <boost/act/interlocked/integer/is_interlocked_type.hpp>
// #include <boost/act/interlocked/integer/is_interlocked_arithmetic_type.hpp>
// #include <boost/act/interlocked/integer/is_interlocked_integer_type.hpp>

#include <boost/act/interlocked/integer/detail/interlocked_bool.hpp>
#include <boost/act/interlocked/integer/detail/convert_interlocked_bool_operand_to_bool.hpp>

#include <boost/detail/interlocked.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_const.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_integral.hpp>

#include <boost/act/detail/is_nonfunction_pointer.hpp>
#include <cstddef>

namespace boost { namespace act { namespace interlocked { namespace detail {

template< unsigned NumBytes
        , typename TargetType, typename SourceType
        , typename ComparisonType = TargetType
        >
struct are_valid_arithmeticn_params
  : mpl::and_
    <
      mpl::and_< has_interlocked_operations< TargetType >
               , is_integral< TargetType >
               , mpl::bool_< ( sizeof( TargetType ) == NumBytes ) >
               , mpl::not_< is_const< TargetType > >
               >
    , is_convertible< SourceType const, TargetType >
    , is_convertible< ComparisonType const, TargetType >
    > {};

template< unsigned NumBytes
        , typename TargetType, typename SourceType
        >
struct are_valid_additiven_params
  : mpl::or_
    <
      are_valid_arithmeticn_params< NumBytes, TargetType, SourceType >
    , mpl::and_
      <
        act::detail::is_nonfunction_pointer< TargetType >
      , mpl::bool_< ( sizeof( TargetType ) == NumBytes ) >
      , mpl::not_< is_const< TargetType > >
      , is_convertible< SourceType const, std::ptrdiff_t >
      >
    > {};

template< typename TargetType, typename SourceType
        , typename ComparisonType = TargetType
        >
struct are_valid_pointer_params
  : mpl::and_
    <
      mpl::and_< has_interlocked_operations< TargetType >
               , act::detail::is_nonfunction_pointer< TargetType >
               , mpl::not_< is_const< TargetType > >
               >
    , is_convertible< SourceType const, TargetType >
    , is_convertible< ComparisonType const, TargetType >
    > {};

template< typename TargetType, typename SourceType
        , typename ComparisonType = TargetType
        >
struct are_valid_bool_params
  : mpl::and_
    <
      mpl::and_< has_interlocked_operations< TargetType >
               , detail::is_interlocked_bool< TargetType >
               , mpl::not_< is_const< TargetType > >
               >
    , mpl::or_< is_convertible< SourceType const, bool >
              , is_convertible< SourceType const, TargetType >
              >
    , mpl::or_< is_convertible< ComparisonType const, bool >
              , is_convertible< ComparisonType const, TargetType >
              >
    > {};

} } } }

#endif

#endif
