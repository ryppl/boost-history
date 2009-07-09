/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_STRUCT_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_STRUCT_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>

#include <boost/mpl/int.hpp>

#define BOOST_FUSION_ADAPT_STRUCT(name, bseq)                                   \
    BOOST_FUSION_ADAPT_STRUCT_I(                                                \
        name, BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_X bseq, 0))

#define BOOST_FUSION_ADAPT_STRUCT_X(x, y) ((x, y)) BOOST_FUSION_ADAPT_STRUCT_Y
#define BOOST_FUSION_ADAPT_STRUCT_Y(x, y) ((x, y)) BOOST_FUSION_ADAPT_STRUCT_X
#define BOOST_FUSION_ADAPT_STRUCT_X0
#define BOOST_FUSION_ADAPT_STRUCT_Y0

// BOOST_FUSION_ADAPT_STRUCT_I generates the overarching structure and uses
// SEQ_FOR_EACH_I to generate the "linear" substructures.
// Thanks to Paul Mensonides for the PP macro help

#define BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(COMBINATION, name)      \
    template <>                                                                 \
    struct tag_of<name COMBINATION>                                             \
    {                                                                           \
        typedef struct_tag type;                                                \
    };                                                                          \

#define BOOST_FUSION_ADAPT_STRUCT_I(name, seq)                                  \
    BOOST_FUSION_ADAPT_STRUCT_BASE(name, seq)                                   \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_FUSION_ADAPT_STRUCT_C, name, seq)

#define BOOST_FUSION_ADAPT_STRUCT_C(r, name, i, xy)                             \
    BOOST_FUSION_ADAPT_STRUCT_C_BASE(r, name, i, xy, 2)

#endif
