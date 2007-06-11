/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_HAS_RESULT_TYPE_HPP
#define BOOST_ACT_DETAIL_HAS_RESULT_TYPE_HPP

#include <boost/type_traits/is_function.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/or.hpp>

namespace boost
{
namespace act
{
namespace detail
{

BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF( has_result_type_member, result_type, false )

template< typename Type >
struct has_result_type
  : mpl::or_< is_function< Type >, has_result_type_member< Type > > {};

}

}
}

#endif
