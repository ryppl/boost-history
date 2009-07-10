// Copyright 2001-2007 Joel de Guzman, Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/support/tag_of_fwd.hpp>

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#ifndef BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_BASE_HPP
#define BOOST_FUSION_ADAPTED_DETAIL_STRUCT_ADAPT_BASE_HPP

#define BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION(COMBINATION, name)      \
    template <>                                                                 \
    struct tag_of<name COMBINATION>                                             \
    {                                                                           \
        typedef struct_tag type;                                                \
    };


#define BOOST_FUSION_ADAPT_STRUCT_BASE(name, seq)                               \
namespace boost { namespace fusion { namespace traits                           \
{                                                                               \
    BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(                               \
        BOOST_FUSION_ADAPT_STRUCT_TAG_OF_SPECIALIZATION,name)                   \
}}}                                                                             \
                                                                                \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template <>                                                                 \
    struct struct_size<name>                                                    \
      : mpl::int_<BOOST_PP_SEQ_SIZE(seq)> {};                                   \
}}}

#define BOOST_FUSION_ADAPT_STRUCT_C_BASE(r, name, i, xy, max_size)              \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template <>                                                                 \
    struct struct_member<name, i>                                               \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(max_size, 0, xy) type;                      \
                                                                                \
        template<typename Seq>                                                  \
        static typename detail::forward_as<Seq&,type>::type                     \
        call(Seq& seq)                                                          \
        {                                                                       \
            return seq.BOOST_PP_TUPLE_ELEM(max_size, 1, xy);                    \
        }                                                                       \
    };                                                                          \
}}}

#endif
