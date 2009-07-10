/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_ASSOC_STRUCT_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_ASSOC_STRUCT_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT(name, bseq)                             \
    BOOST_FUSION_ADAPT_ASSOC_STRUCT_I(                                          \
        name, BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_STRUCT_X bseq, 0))

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_X(x, y, z)\
    ((x, y, z)) BOOST_FUSION_ADAPT_ASSOC_STRUCT_Y
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_Y(x, y, z)\
    ((x, y, z)) BOOST_FUSION_ADAPT_ASSOC_STRUCT_X
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_X0
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_Y0

// BOOST_FUSION_ADAPT_ASSOC_STRUCT_I generates the overarching structure and
// uses SEQ_FOR_EACH_I to generate the "linear" substructures.
// Thanks to Paul Mensonides for the PP macro help

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_I(name, seq)                            \
    BOOST_FUSION_ADAPT_STRUCT_BASE(name, seq)                                   \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_FUSION_ADAPT_ASSOC_STRUCT_C, name, seq)

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_C(r, name, i, xy)                       \
    BOOST_FUSION_ADAPT_STRUCT_C_BASE(r, name, i, xy, 3)                         \
                                                                                \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template<>                                                                  \
    struct struct_assoc_member<name, BOOST_PP_TUPLE_ELEM(3, 2, xy)>             \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(3, 0, xy) type;                             \
                                                                                \
        template<typename Seq>                                                  \
        static typename detail::forward_as<Seq&,type>::type                     \
        call(Seq& seq)                                                          \
        {                                                                       \
            return seq.BOOST_PP_TUPLE_ELEM(3, 1, xy);                           \
        };                                                                      \
    };                                                                          \
}}}

#endif
