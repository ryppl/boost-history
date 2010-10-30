/*==============================================================================
    Copyright (c) 2001-2009 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_ADT_ADAPT_ASSOC_ADT_HPP
#define BOOST_FUSION_ADAPTED_ADT_ADAPT_ASSOC_ADT_HPP

#include <boost/fusion/support/internal/base.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

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
#include <boost/fusion/adapted/detail/struct/deref_data_impl.hpp>
#include <boost/fusion/adapted/detail/struct/key_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_of_data_impl.hpp>
#include <boost/fusion/adapted/detail/adt/extension.hpp>
#include <boost/fusion/adapted/detail/adt/adapt_base.hpp>

#define BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0(A, B, C, D, E)\
    ((A, B, C, D, E)) BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_1
#define BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_1(A, B, C, D, E)\
    ((A, B, C, D, E)) BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0
#define BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0_END
#define BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_1_END

#define BOOST_FUSION_ADAPT_ASSOC_ADT_C(                                         \
    TEMPLATE_PARAMS_SEQ, NAME_SEQ, I, ATTRIBUTE)                                \
                                                                                \
    BOOST_FUSION_ADAPT_ADT_C_BASE(TEMPLATE_PARAMS_SEQ,NAME_SEQ,I,ATTRIBUTE,5)   \
                                                                                \
    template<                                                                   \
        BOOST_FUSION_ADAPT_STRUCT_UNPACK_TEMPLATE_PARAMS(TEMPLATE_PARAMS_SEQ)   \
    >                                                                           \
    struct struct_assoc_key<BOOST_FUSION_ADAPT_STRUCT_UNPACK_NAME(NAME_SEQ), I> \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(5, 4, ATTRIBUTE) type;                      \
    };

#define BOOST_FUSION_ADAPT_ASSOC_TPL_ADT(                                       \
    TEMPLATE_PARAMS_SEQ, NAME_SEQ, ATTRIBUTES)                                  \
                                                                                \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (1)TEMPLATE_PARAMS_SEQ,                                                 \
        (1)NAME_SEQ,                                                            \
        assoc_struct_tag,                                                       \
        0,                                                                      \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0 ATTRIBUTES,_END),    \
        BOOST_FUSION_ADAPT_ASSOC_ADT_C)

#define BOOST_FUSION_ADAPT_ASSOC_ADT(NAME, ATTRIBUTES)                        \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (0),                                                                    \
        (0)(NAME),                                                              \
        assoc_struct_tag,                                                       \
        0,                                                                      \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0 ATTRIBUTES,_END),  \
        BOOST_FUSION_ADAPT_ASSOC_ADT_C)

#define BOOST_FUSION_ADAPT_ASSOC_ADT_AS_VIEW(NAME, ATTRIBUTES)                  \
    BOOST_FUSION_ADAPT_STRUCT_BASE(                                             \
        (0),                                                                    \
        (0)(NAME),                                                              \
        assoc_struct_tag,                                                       \
        1,                                                                      \
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_ADT_FILLER_0 ATTRIBUTES,_END),    \
        BOOST_FUSION_ADAPT_ASSOC_ADT_C)

#endif
