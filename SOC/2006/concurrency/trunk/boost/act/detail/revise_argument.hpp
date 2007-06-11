/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

#ifndef BOOST_ACT_DETAIL_REVISE_ARGUMENT_HPP
#define BOOST_ACT_DETAIL_REVISE_ARGUMENT_HPP

namespace boost { namespace act { namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF( revised_type )

template< typename Arg >
struct revise_argument_impl
{
  typedef typename Arg::revised_type type;
};

template< typename Arg >
struct revise_argument
  : mpl::eval_if< has_revised_type< Arg >
                , revise_argument_impl< Arg >
                , mpl::identity< Arg >
                >
{
};

} } }

#endif
