/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_ADAPT_STRUCT_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>

#include <boost/fusion/adapted/detail/struct/extension.hpp>
#include <boost/fusion/adapted/detail/struct/adapt_base.hpp>
#include <boost/fusion/adapted/detail/struct/at_impl.hpp>
#include <boost/fusion/adapted/detail/struct/is_view_impl.hpp>
#include <boost/fusion/adapted/detail/struct/is_sequence_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_at_impl.hpp>
#include <boost/fusion/adapted/detail/struct/category_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/size_impl.hpp>
#include <boost/fusion/adapted/detail/struct/begin_impl.hpp>
#include <boost/fusion/adapted/detail/struct/end_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/deref_impl.hpp>

#define BOOST_FUSION_ADAPT_STRUCT_FILLER_0(X, Y)                                \
    ((X, Y)) BOOST_FUSION_ADAPT_STRUCT_FILLER_1
#define BOOST_FUSION_ADAPT_STRUCT_FILLER_1(X, Y)                                \
    ((X, Y)) BOOST_FUSION_ADAPT_STRUCT_FILLER_0
#define BOOST_FUSION_ADAPT_STRUCT_FILLER_0_END
#define BOOST_FUSION_ADAPT_STRUCT_FILLER_1_END

#define BOOST_FUSION_ADAPT_STRUCT_C(TEMPLATE_PARAMS_SEQ, NAME_SEQ, I, ATTRIBUTE)\
    BOOST_FUSION_ADAPT_STRUCT_C_BASE(                                           \
        TEMPLATE_PARAMS_SEQ,NAME_SEQ,I,BOOST_PP_EMPTY(),ATTRIBUTE,2)

#define BOOST_FUSION_ADAPT_TPL_STRUCT(TEMPLATE_PARAMS_SEQ,NAME_SEQ, ATTRIBUTES) \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (1)TEMPLATE_PARAMS_SEQ,                                                 \
        (1)NAME_SEQ,                                                            \
        struct_tag,                                                             \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_FILLER_0 ATTRIBUTES,_END),       \
        BOOST_FUSION_ADAPT_STRUCT_C)

#define BOOST_FUSION_ADAPT_STRUCT(NAME, ATTRIBUTES)                             \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (0),                                                                    \
        (0)(NAME),                                                              \
        struct_tag,                                                             \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_FILLER_0 ATTRIBUTES,_END),       \
        BOOST_FUSION_ADAPT_STRUCT_C)

#endif
