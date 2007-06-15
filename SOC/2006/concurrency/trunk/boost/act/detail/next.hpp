/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_NEXT_HPP
#define BOOST_ACT_DETAIL_NEXT_HPP

namespace boost { namespace act { namespace detail {

template< typename Type >
Type next( Type source )
{
  ++source;

  return source;
}

} } }

#endif
