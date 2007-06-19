/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_RESULT_META_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_RESULT_META_HPP

namespace boost { namespace act { namespace interlocked { namespace detail {

struct result_meta_void;

template< typename ImplType
        , typename Type0
        , typename Type1 = result_meta_void, typename Type2 = result_meta_void
        >
struct result_meta
  : ImplType::template result< Type0, Type1, Type2 >
{
};

template< typename ImplType
        , typename Type0, typename Type1
        >
struct result_meta< ImplType, Type0, Type1 >
  : ImplType::template result< Type0, Type1 >
{
};

template< typename ImplType
        , typename Type0
        >
struct result_meta< ImplType, Type0 >
  : ImplType::template result< Type0 >
{
};

} } } }

#endif
