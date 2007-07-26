/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_FWD_HPP
#define BOOST_ACT_INTERLOCKED_MODIFY_MODIFY_FWD_HPP

#include <boost/act/interlocked/detail/cas_support.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/act/interlocked/semantics/default.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename TargetType, typename OperationType >
typename lazy_enable_if
<
  detail::are_valid_store_style_params< TargetType >
, remove_cv< TargetType >
>
::type
modify( TargetType& destination, OperationType operation );

template< typename Semantics, typename TargetType, typename OperationType >
typename lazy_enable_if
<
  mpl::and_< is_same< Semantics, default_ >
           , detail::are_valid_store_style_params< TargetType >
           >
, remove_cv< TargetType >
>
::type
modify( TargetType& destination, OperationType operation );

} } }

#endif
