/*==============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP
#define BOOST_FUSION_SEQUENCE_SEQUENCE_FACADE_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/internal/facade_generation.hpp>
#include <boost/mpl/bool.hpp>

#define BOOST_FUSION_SEQUENCE_INTRINSIC_FUNCS                                   \
    ((at_key, 1, 1))                                                            \
    ((at, 1, 0))                                                                \
    ((back, 0, 0))                                                              \
    ((begin, 0, 0))                                                             \
    ((empty, 0, 1))                                                             \
    ((end, 0, 0))                                                               \
    ((has_key, 1, 1))                                                           \
    ((size, 0, 1))                                                              \
    ((value_at_key, 1, 1))                                                      \
    ((value_at, 1, 0))

BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_WRAPPER(
    sequence_facade_tag, BOOST_FUSION_SEQUENCE_INTRINSIC_FUNCS)

namespace boost { namespace fusion
{
    template<
        typename Derived
      , typename Category
      , typename IsView = mpl::false_
    >
    struct sequence_facade
      : sequence_base<Derived>
    {
        typedef fusion_sequence_tag tag;
        typedef sequence_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;
        typedef IsView is_view;

        BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_FORWARDER(
            BOOST_FUSION_SEQUENCE_INTRINSIC_FUNCS)
    };
}}

#undef BOOST_FUSION_SEQUENCE_INTRINSIC_FUNCS

#endif
