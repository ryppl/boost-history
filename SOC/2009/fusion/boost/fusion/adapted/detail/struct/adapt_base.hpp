/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_BASE_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_BASE_HPP

#include <boost/config.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>

#include <boost/mpl/tag.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#ifdef BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#   define BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(MODIFIER, DATA)      \
    template <>                                                                 \
    struct tag_of<BOOST_PP_TUPLE_ELEM(2,0,DATA) MODIFIER,void>                  \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(2,1,DATA) type;                             \
    };
#else
#   define BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(MODIFIER, DATA)      \
    template <>                                                                 \
    struct tag_of<BOOST_PP_TUPLE_ELEM(2,0,DATA) MODIFIER>                       \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(2,1,DATA) type;                             \
    };
#endif

#define BOOST_FUSION_ADAPT_STRUCT_BASE(NAME,TAG,SEQ,CALLBACK)                   \
    BOOST_PP_SEQ_FOR_EACH_I(CALLBACK,NAME,SEQ)                                  \
                                                                                \
namespace boost                                                                 \
{                                                                               \
    namespace fusion                                                            \
    {                                                                           \
        namespace traits                                                        \
        {                                                                       \
            BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(                       \
                BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION,(NAME,TAG))     \
        }                                                                       \
                                                                                \
        namespace extension                                                     \
        {                                                                       \
            template <>                                                         \
            struct struct_size<NAME>                                            \
              : mpl::int_<BOOST_PP_SEQ_SIZE(SEQ)>                               \
            {};                                                                 \
        }                                                                       \
    }                                                                           \
                                                                                \
    namespace mpl                                                               \
    {                                                                           \
        template<typename>                                                      \
        struct sequence_tag;                                                    \
                                                                                \
        template <>                                                             \
        struct sequence_tag<NAME>                                               \
        {                                                                       \
            typedef fusion::fusion_sequence_tag type;                           \
        };                                                                      \
                                                                                \
        template <>                                                             \
        struct sequence_tag<NAME const>                                         \
        {                                                                       \
            typedef fusion::fusion_sequence_tag type;                           \
        };                                                                      \
    }                                                                           \
}

#define BOOST_FUSION_ADAPT_STRUCT_C_BASE(NAME, I, PREFIX, TUPLE, MAX_SIZE)      \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template <>                                                                 \
    struct struct_member<NAME, I>                                               \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(MAX_SIZE, 0, TUPLE) type;                   \
                                                                                \
        template<typename Seq>                                                  \
        static typename detail::forward_as<Seq&,type>::type                     \
        call(Seq& seq)                                                          \
        {                                                                       \
            return seq.PREFIX BOOST_PP_TUPLE_ELEM(MAX_SIZE, 1, TUPLE);          \
        }                                                                       \
    };                                                                          \
}}}

#endif
