/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_SEMANTICS_HPP
#define BOOST_ACT_INTERLOCKED_TYPE_TRAITS_IS_SEMANTICS_HPP

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/act/interlocked/semantics_fwd.hpp>

namespace boost { namespace act { namespace interlocked {

template< typename Type >
struct is_semantics
  : mpl::or_< is_same< Type, acquire >, is_same< Type, release >
            , is_same< Type, acq_rel >, is_same< Type, unordered >
            , is_same< Type, default_ >
            > {};

} } }

#endif
