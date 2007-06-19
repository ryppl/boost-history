/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_META_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_META_HPP

namespace boost { namespace act { namespace interlocked { namespace detail {

// Note: This template exists to delay instantiation of the impl type such that
//       the definition of the impl type does not need to appear until after
//       the definition of a template which uses it.
template< typename ImplType, typename DummyType >
struct impl_meta : ImplType {};

} } } }

#endif
