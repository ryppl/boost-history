/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_DECL_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_DECL_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

namespace boost { namespace act { namespace interlocked { namespace detail {

#define BOOST_ACT_INTERLOCKED_DETAIL_IMPL_DECL_MACRO( r, data, elem )          \
template< typename ResultType, typename UnqualifiedType >                      \
struct BOOST_PP_CAT( elem, _impl );                                            \
                                                                               \
template< typename ResultType, typename UnqualifiedType >                      \
struct BOOST_PP_CAT( elem, _acquire_impl );                                    \
                                                                               \
template< typename ResultType, typename UnqualifiedType >                      \
struct BOOST_PP_CAT( elem, _release_impl );

BOOST_PP_SEQ_FOR_EACH( BOOST_ACT_INTERLOCKED_DETAIL_IMPL_DECL_MACRO
                     , ~
                     , (assign)
                       (assign_if_was)
                       (increment)
                       (decrement)
                       (add_assign)
                       (subtract_assign)
                       (multiply_assign)
                       (divide_assign)
                       (mod_assign)
                       (and_assign)
                       (or_assign)
                       (xor_assign)
                       (lshift_assign)
                       (rshift_assign)
                       (compl_assign)
                       (not_assign)
                     )

#undef BOOST_ACT_INTERLOCKED_DETAIL_IMPL_DECL_MACRO

} } } }

#endif
