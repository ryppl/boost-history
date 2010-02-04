/*=============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STRUCT_ADAPT_ASSOC_STRUCT_NAMED_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_ADAPT_ASSOC_STRUCT_NAMED_HPP

#include <boost/fusion/adapted/struct/adapt_assoc_struct.hpp>
#include <boost/fusion/adapted/detail/struct/proxy_type.hpp>

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_NAMED(WRAPPED_TYPE, NAME, SEQ)\
    BOOST_FUSION_ADAPT_ASSOC_STRUCT_NAMED_NS(\
        WRAPPED_TYPE,\
        (boost)(fusion)(adapted),\
        NAME,\
        SEQ)

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_NAMED_NS(\
    WRAPPED_TYPE, NAMESPACE, NAME, SEQ)\
    \
    BOOST_FUSION_DEFINE_PROXY_TYPE(WRAPPED_TYPE,NAMESPACE,NAME)\
    \
    BOOST_FUSION_ADAPT_ASSOC_STRUCT_BASE(\
        BOOST_FUSION_NAMESPACE_DECLARATION(NAMESPACE)NAME,\
        struct_tag,\
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_FILLER_0 SEQ,_END),\
        BOOST_FUSION_ADAPT_ASSOC_STRUCT_NAMED_C)

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_NAMED_C(R, NAME, I, SEQ)\
    BOOST_FUSION_ADAPT_ASSOC_STRUCT_C_BASE(NAME, I, obj., SEQ)

#endif
