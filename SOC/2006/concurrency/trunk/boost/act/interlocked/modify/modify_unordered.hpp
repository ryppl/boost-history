/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_UNORDERED_HPP
#define BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_UNORDERED_HPP

#include <boost/act/interlocked/assign_if_was/assign_if_was_unordered.hpp>
#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/act/interlocked/semantics/unordered.hpp>
#include <boost/act/interlocked/load/load_unordered.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Semantics, typename TargetType, typename OperationType >
typename lazy_enable_if
<
  mpl::and_< is_same< Semantics, unordered >
           , detail::are_valid_store_style_params< TargetType >
           >
, remove_cv< TargetType >
>
::type
modify( TargetType& destination, OperationType operation )
{
  typedef typename remove_cv< TargetType >::type unqualified_type;
  unqualified_type old_value = interlocked::load< unordered >( destination ),
                   expected_value;

  do
  {
    expected_value = old_value;

    old_value = interlocked::assign_if_was< unordered >
                ( destination
                , operation( expected_value )
                , expected_value
                );
  }
  while( old_value != expected_value );

  return old_value;
}

} } }

#endif
