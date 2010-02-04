/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_CLASS_ADAPT_ASSOC_CLASS_NAMED_HPP
#define BOOST_FUSION_ADAPTED_CLASS_ADAPT_ASSOC_CLASS_NAMED_HPP

#define BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_0(A, B, C, D, E)\
    ((A, B, C, D, E)) BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_1
#define BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_1(A, B, C, D, E)\
    ((A, B, C, D, E)) BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_0
#define BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_0_END
#define BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_1_END

#define BOOST_FUSION_ADAPT_ASSOC_CLASS(NAME, SEQ)\
    BOOST_FUSION_ADAPT_STRUCT_BASE(\
        NAME,\
        assoc_class_tag,\
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_CLASS_FILLER_0 SEQ,_END),\
        BOOST_FUSION_ADAPT_ASSOC_CLASS_C)

#define BOOST_FUSION_ADAPT_ASSOC_CLASS_C(R, NAME, I, TUPLE)\
    BOOST_FUSION_ADAPT_STRUCT_C_BASE(NAME, I, PREFIX, TUPLE, 5)                 \
                                                                                \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template<>                                                                  \
    struct struct_assoc_key<NAME, I>                                            \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(5, 4, TUPLE) type;                          \
    };                                                                          \
}}}

#endif
