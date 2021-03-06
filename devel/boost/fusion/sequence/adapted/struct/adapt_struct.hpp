/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_ADAPT_STRUCT_APRIL_2_2007_1158AM)
#define BOOST_FUSION_ADAPT_STRUCT_APRIL_2_2007_1158AM

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/sequence/adapted/struct/extension.hpp>
#include <boost/fusion/sequence/adapted/struct/struct_iterator.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/is_view_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/is_sequence_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/category_of_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/begin_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/end_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/size_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/at_impl.hpp>
#include <boost/fusion/sequence/adapted/struct/detail/value_at_impl.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/int.hpp>
#include <utility>

#define BOOST_FUSION_ADAPT_STRUCT(name, bseq)                                   \
    BOOST_FUSION_ADAPT_STRUCT_I(                                                \
        name, BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_X bseq, 0))                \
    /***/

#define BOOST_FUSION_ADAPT_STRUCT_X(x, y) ((x, y)) BOOST_FUSION_ADAPT_STRUCT_Y
#define BOOST_FUSION_ADAPT_STRUCT_Y(x, y) ((x, y)) BOOST_FUSION_ADAPT_STRUCT_X
#define BOOST_FUSION_ADAPT_STRUCT_X0
#define BOOST_FUSION_ADAPT_STRUCT_Y0

// BOOST_FUSION_ADAPT_STRUCT_I generates the overarching structure and uses
// SEQ_FOR_EACH_I to generate the "linear" substructures.
// Thanks to Paul Mensonides for the PP macro help

#define BOOST_FUSION_ADAPT_STRUCT_I(name, seq)                                  \
    namespace boost { namespace fusion { namespace traits                       \
    {                                                                           \
        template <>                                                             \
        struct tag_of<name>                                                     \
        {                                                                       \
            typedef struct_tag type;                                            \
        };                                                                      \
    }}}                                                                         \
    namespace boost { namespace fusion { namespace extension                    \
    {                                                                           \
        template <>                                                             \
        struct struct_size<name> : mpl::int_<BOOST_PP_SEQ_SIZE(seq)> {};        \
        BOOST_PP_SEQ_FOR_EACH_I(BOOST_FUSION_ADAPT_STRUCT_C, name, seq)         \
    }}}                                                                         \
    /***/

#define BOOST_FUSION_ADAPT_STRUCT_C(r, name, i, xy)                             \
    template <>                                                                 \
    struct struct_member<name, i>                                               \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(2, 0, xy) type;                             \
        static type& call(name& struct_)                                        \
        {                                                                       \
            return struct_.BOOST_PP_TUPLE_ELEM(2, 1, xy);                       \
        };                                                                      \
    };                                                                          \
    /***/

#endif
