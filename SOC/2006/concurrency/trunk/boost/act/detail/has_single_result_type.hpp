/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_HAS_SINGLE_RESULT_TYPE_HPP
#define BOOST_ACT_DETAIL_HAS_SINGLE_RESULT_TYPE_HPP

#include <boost/type_traits/is_function.hpp>

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/or.hpp>

// ToDo: Don't rely on this -- figure out why has_xxx is broken in msvc 7.1
#include <boost/utility/result_of.hpp>

namespace boost { namespace act { namespace detail {

template< typename Type >
struct has_single_result_type
  : mpl::or_< is_function< Type >, boost::detail::has_result_type< Type > > {};

} } }

#endif
