/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP
#define BOOST_FUSION_ITERATOR_ITERATOR_FACADE_HPP

#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/internal/facade_generation.hpp>

#define BOOST_FUSION_ITERATOR_INTRINSIC_FUNCS                                   \
    ((advance, 1, 1))                                                           \
    ((deref_data, 0, 0))                                                        \
    ((deref, 0, 0))                                                             \
    ((distance, 1, 0))                                                          \
    ((equal_to, 1, 0))                                                          \
    ((key_of, 0, 0))                                                            \
    ((next, 0, 0))                                                              \
    ((prior, 0, 0))                                                             \
    ((value_of_data, 0, 0))                                                     \
    ((value_of, 0, 0))

BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_WRAPPER(
    iterator_facade_tag, BOOST_FUSION_ITERATOR_INTRINSIC_FUNCS)

namespace boost { namespace fusion
{
    template<typename Derived, typename Category>
    struct iterator_facade
      : iterator_base<Derived>
    {
        typedef iterator_facade_tag fusion_tag;
        typedef Derived derived_type;
        typedef Category category;

        //TODO doc

        BOOST_FUSION_FACADE_DEFINE_INTRINSIC_FUNCS_FORWARDER(
            BOOST_FUSION_ITERATOR_INTRINSIC_FUNCS)
    };
}}

#undef BOOST_FUSION_ITERATOR_INTRINSIC_FUNCS

#endif
